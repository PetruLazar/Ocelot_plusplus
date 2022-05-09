#include "../message.h"

#include "../../debug/mcexceptions.h"
#include "../../debug/debug.h"
#include "../../debug/log.h"
#include "../../server/options.h"
#include "../../types/gametags.h"
#include "../../types/chat.h"
#include "../../types/utils.h"
#include "../../types/node.h"
#include "../../types/enums.h"
#include "../../types/basic.h"
#include "../../types/window.h"
#include "../command.h"

void message::preparePacket(Player* p, char*& data, ull& size, char*& toDelete)
{
	toDelete = data - 6;

	//compress data if it's the case
	if (p->compressionEnabled)
	{
		//test thershold
		if (size < p->compressionThreshold)
		{
			//allocate a smaller block of memory
			char* toDeleteInit = toDelete;
			toDelete = new char[size + 6];
			char* initData = data;
			data = toDelete + 6;

			//copy data into the smaller block and deallocate the big block
			for (uint i = 0; i < (uint)size; i++) data[i] = initData[i];
			delete[] toDeleteInit;

			//prepend 0
			(--data)[0] = 0;
			size++;
		}
		else
		{
			//prepare for compression
			char* toDeleteUnc = toDelete;
			toDelete = new char[size + 6];
			char* uncompressedData = data;
			data = toDelete + 6;

			//compress
			uint uncompressedSize = (uint)size;
			if (!zlibCompressNoAlloc(uncompressedData, uncompressedSize, data, (uint&)size)) throw runtimeError("Zlib compression failed");
			delete[] toDeleteUnc;

			//prepend uncompressedSize
			int append = (int)(uint)uncompressedSize;
			int appendSize = (int)(uint)varInt::size(append);
			data -= appendSize;
			char* buffer = data;
			size += appendSize;
			varInt(append).write(buffer);

			//prepend compressed size before data outside the if
		}
	}

	//prepend size before data
	int append = (int)(uint)size;
	int appendSize = (int)(uint)varInt::size(append);
	data -= appendSize;
	char* buffer = data;
	size += appendSize;
	varInt(append).write(buffer);
}
/*void message::sendPacketData(Player* p, char* data, ull size, bool disconnectAfter)
{
	char* toDelete = data - 6;

	//compress data if it's the case
	if (p->compressionEnabled)
	{
		//test thershold
		if (size < p->compressionThreshold)
		{
			//do not compress
			//append 0
			(data--)[-1] = 0;
			size++;

			//append size before data outside the if
		}
		else
		{
			//prepare for compression
			char* toDeleteUnc = toDelete;
			toDelete = new char[size + 6];
			char* uncompressedData = data;
			data = toDelete + 6;

			//compress
			uint uncompressedSize = (uint)size;
			if (!zlibCompressNoAlloc(uncompressedData, uncompressedSize, data, (uint&)size)) throw runtimeError("Zlib compression failed");
			delete[] toDeleteUnc;

			//append uncompressedSize
			int append = (int)(uint)uncompressedSize;
			int appendSize = (int)(uint)varInt::size(append);
			data -= appendSize;
			char* buffer = data;
			size += appendSize;
			varInt(append).write(buffer);

			//append compressed size before data outside the if
		}
	}

	//append size before data
	int append = (int)(uint)size;
	int appendSize = (int)(uint)varInt::size(append);
	data -= appendSize;
	char* buffer = data;
	size += appendSize;
	varInt(append).write(buffer);

	//append compressed size when compressed

	p->schedulePacket(data, size, toDelete, disconnectAfter);
}*/
void message::dispatch(Player* p, char* data, uint compressedSize, uint decompressedSize)
{
	if (decompressedSize)
	{
		//decompress
		char* compressedData = data;
		if (!zlibDecompress(compressedData, compressedSize, data, decompressedSize)) throw runtimeError("Zlib decompression failed");
		//if (!zlibDecompress(compressedData, compressedSize - varInt::size(decompressedSize), data, decompressedSize)) throw runtimeError("Zlib decompression failed");
		try
		{
			dispatch(p, data, decompressedSize);
		}
		catch (...)
		{
			delete[] data;
			throw;
		}
		delete[] data;
	}
	else
	{
		dispatch(p, data, compressedSize);
		//dispatch(p, data, compressedSize - 1);
	}
}
void message::dispatch(Player* p, char* data, uint size)
{
	varInt id;
	id.read(data);
	switch (p->state)
	{
	case ConnectionState::handshake:
		switch ((handshake::id)(int)id)
		{
		case handshake::id::standard:
		{
			varInt protocolV, nextState;
			mcString address;
			bigEndian<Port> port;
			protocolV.read(data);
			address.read(data);
			port.read(data);
			nextState.read(data);
			handshake::receive::standard(p, protocolV, address, port, nextState);
		}
		break;
		default:
			p->disconnect();
			throw protocolError("Invalid packet id");
		}
		break;
	case ConnectionState::status:
		switch ((status::id)(int)id)
		{
		case status::id::request:
		{
			status::receive::request(p);
		}
		break;
		case status::id::ping:
		{
			blong payload;
			payload.read(data);
			status::receive::ping(p, payload);
		}
		break;
		default:
			p->disconnect();
			throw protocolError("Invalid packet id");
		}
		break;
	case ConnectionState::login:
		switch ((login::id)(int)id)
		{
		case login::id::start:
		{
			mcString username;
			username.read(data);
			login::receive::start(p, username);
		}
		break;
		case login::id::encryptionResponse:
		{
			login::receive::encryptionResponse(p, 0, nullptr, 0, nullptr);
		}
		break;
		case login::id::loginPluginResponse:
		{
			p->disconnect();
			throw protocolError("Login Plugin not supported.");
		}
		break;
		default:
			p->disconnect();
			throw protocolError("Invalid packet id");
		}
		break;
	case ConnectionState::play:
		switch ((play::id)(int)id)
		{
		case play::id::teleportConfirm:
		{
			varInt teleportId;
			teleportId.read(data);
			play::receive::teleportConfirm(p, teleportId);
		}
		break;
		case play::id::queryBlockNbt:
		{
			Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: query block nbt";
		}
		break;
		case play::id::setDifficulty:
		{
			bool difficulty;
			difficulty = *(data++);
			message::play::receive::setDifficulty(p, difficulty);
		}
		break;
		case play::id::chatMessage_serverbound:
		{
			mcString content;
			content.read(data);
			message::play::receive::chatMessage(p, content);
		}
		break;
		case play::id::clientStatus:
		{
			varInt actionId;
			actionId.read(data);
			play::receive::clientStatus(p, actionId);

			Log::debug(PROTOCOL_WARNINGS) << "Partially handled packet: client status" << Log::endl;
		}
		break;
		case play::id::clientSettings:
		{
			mcString locale;
			Byte viewDistance, displayedSkinParts;
			varInt chatMode, mainHand;
			bool chatColors, enableTextFiltering, allowServerListings;

			locale.read(data);
			viewDistance = *(data++);
			chatMode.read(data);
			chatColors = *(data++);
			displayedSkinParts = *(data++);
			mainHand.read(data);
			enableTextFiltering = *(data++);
			allowServerListings = *(data++);

			play::receive::clientSettings(p, locale, viewDistance, (ChatMode)(int)chatMode, chatColors, displayedSkinParts, mainHand, enableTextFiltering, allowServerListings);
		}
		break;
		case play::id::tabComplete_serverbound:
		{
			Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: tab complete";
		}
		break;
		case play::id::clickWindowButton:
		{
			Byte windowID, buttonID;
			windowID = *(data++);
			buttonID = *(data++);

			play::receive::clickWindowButton(p, windowID, buttonID);
		}
		break;
		case play::id::clickWindow:
		{
			Byte windowID, button;
			varInt stateID, mode, length;
			bshort clickedSlot;
			bshort* slotNumbers;
			Slot** slots;
			Slot* clickedItem = new Slot();

			windowID = *(data++);
			stateID.read(data);
			clickedSlot.read(data);
			button = *(data++);
			mode.read(data);
			length.read(data);
			slotNumbers = new bshort[length];
			slots = new Slot * [length];
			for (int i = 0; i < length; i++)
			{
				slotNumbers[i].read(data);
				slots[i] = new Slot();
				slots[i]->read(data);
			}
			if (mode != 4)
				clickedItem->read(data);

			play::receive::clickWindow(p, windowID, stateID, clickedSlot, button, mode, length, slotNumbers, slots, clickedItem);
		}
		break;
		case play::id::closeWindow_serverbound:
		{
			Byte windowID;
			windowID = *(data++);

			play::receive::closeWindow(p, windowID);
		}
		break;
		case play::id::pluginMessage_serverbound:
		{
			Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: plugin message" << Log::endl;
		}
		break;
		case play::id::editBook:
		{
			varInt hand;
			varInt count;
			std::vector<mcString> texts(count);
			bool hasTitle;
			mcString title;

			hand.read(data);
			count.read(data);
			for (int i = 0; i < count; i++)
			{
				mcString t;
				t.read(data);
				texts.emplace_back(t);
			}

			hasTitle = *(data++);
			if (hasTitle)
				title.read(data);

			message::play::receive::editBook(p, hand, count, texts, hasTitle, title);
		}
		break;
		case play::id::queryEntityNbt:
		{
			Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: query entity nbt" << Log::endl;
		}
		break;
		case play::id::interactEntity:
		{
			varInt eid, type, mainHand;
			bfloat targetX, targetY, targetZ;
			bool sneaking;

			eid.read(data);
			type.read(data);
			if (type == 2)
			{
				targetX.read(data);
				targetY.read(data);
				targetZ.read(data);
				mainHand.read(data);
			}
			sneaking = *(data++);

			message::play::receive::interactEntity(p, eid, type, targetX, targetY, targetZ, (Hand)(int)mainHand, sneaking);
			Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: interact entity" << Log::endl;
		}
		break;
		case play::id::generateStructure:
		{
			Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: generate structure" << Log::endl;
		}
		break;
		case play::id::keepAlive_serverbound:
		{
			blong keepAlive_id;
			keepAlive_id.read(data);
			play::receive::keepAlive(p, keepAlive_id);
		}
		break;
		case play::id::lockDifficulty:
		{
			bool locked;
			locked = *(data++);

			play::receive::lockDifficulty(p, locked);
		}
		break;
		case play::id::playerPosition:
		{
			bdouble x, y, z;
			bool onGround;
			x.read(data);
			y.read(data);
			z.read(data);
			onGround = *(data++);
			play::receive::playerPosition(p, x, y, z, onGround);
		}
		break;
		case play::id::playerPositionAndRotation_serverbound:
		{
			bdouble x, y, z;
			bfloat yaw, pitch;
			bool onGround;
			x.read(data);
			y.read(data);
			z.read(data);
			yaw.read(data);
			pitch.read(data);
			onGround = *(data++);
			play::receive::playerPositionAndRotation(p, x, y, z, yaw, pitch, onGround);
		}
		break;
		case play::id::playerRotation:
		{
			bfloat yaw, pitch;
			bool onGround;
			yaw.read(data);
			pitch.read(data);
			onGround = *(data++);
			play::receive::playerRotation(p, yaw, pitch, onGround);
		}
		break;
		case play::id::playerMovement:
		{
			Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: player movement" << Log::endl;
		}
		break;
		case play::id::vehicleMove:
		{
			Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: vehicle move" << Log::endl;
		}
		break;
		case play::id::steerBoat:
		{
			Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: steer boat" << Log::endl;
		}
		break;
		case play::id::pickItem:
		{
			varInt slotToUse;
			slotToUse.read(data);

			message::play::receive::pickItem(p, slotToUse);
		}
		break;
		case play::id::craftRecipeRequest:
		{
			Byte windowID;
			mcString recipe;
			bool makeAll;

			windowID = *(data++);
			recipe.read(data);
			makeAll = *(data++);

			message::play::receive::craftRecipeRequest(p, windowID, recipe, makeAll);

			Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: craft recipe request" << Log::endl;
		}
		break;
		case play::id::playerAbilities_serverbound:
		{
			Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: player abilities" << Log::endl;
		}
		break;
		case play::id::playerDigging:
		{
			varInt status;
			Position location;
			Byte face;
			status.read(data);
			location.read(data);
			face = *(data++);

			play::receive::playerDigging(p, status, location, face);
			Log::debug(PROTOCOL_WARNINGS) << "Partially handled packet: player digging" << Log::endl;
		}
		break;
		case play::id::entityAction:
		{
			varInt eid, actionID, jumpBoost;
			eid.read(data);
			actionID.read(data);
			jumpBoost.read(data);

			play::receive::entityAction(p, eid, actionID, jumpBoost);
			Log::debug(PROTOCOL_WARNINGS) << "Partially handled packet: entity action" << Log::endl;
		}
		break;
		case play::id::steerVehicle:
		{
			Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: steer vehicle" << Log::endl;
		}
		break;
		case play::id::pong:
		{
			bint id;
			id.read(data);

			play::receive::pong(p, id);
		}
		break;
		case play::id::setRecipeBookState:
		{
			Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: set recipe book state" << Log::endl;
		}
		break;
		case play::id::setDisplayedRecipe:
		{
			mcString recipeID;
			recipeID.read(data);

			play::receive::setDisplayedRecipe(p, recipeID);
			Log::debug(PROTOCOL_WARNINGS) << "Partially handled packet: set displayed recipe" << Log::endl;
		}
		break;
		case play::id::nameItem:
		{
			mcString newName;
			newName.read(data);

			play::receive::nameItem(p, newName);
			Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: name item" << Log::endl;
		}
		break;
		case play::id::resourcePackStatus:
		{
			varInt result;
			result.read(data);

			play::receive::resourcePackStatus(p, result);
			Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: resource pack status" << Log::endl;
		}
		break;
		case play::id::advancementTab:
		{
			varInt action;
			mcString tabID = "";

			action.read(data);
			if (action == 0)
				tabID.read(data);

			play::receive::advancementTab(p, action, tabID);
			Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: advancement tab" << Log::endl;
		}
		break;
		case play::id::selectTrade:
		{
			Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: select trade" << Log::endl;
		}
		break;
		case play::id::setBeaconEffect:
		{
			Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: set beacon effect" << Log::endl;
		}
		break;
		case play::id::heldItemChange_serverbound:
		{
			bshort slot;
			slot.read(data);

			play::receive::heldItemChange(p, slot);
		}
		break;
		case play::id::updateCommandBlock:
		{
			Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: update command block" << Log::endl;
		}
		break;
		case play::id::updateCommandBlockMinecart:
		{
			Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet : update command block minecart" << Log::endl;
		}
		break;
		case play::id::creativeInventoryAction:
		{
			bshort slot;
			slot.read(data);

			bool present;
			varInt itemId = 0;
			Byte count = 0;
			nbt_compound* item_data = new nbt_compound();

			present = *(data++);
			if (present)
			{
				itemId.read(data);
				count = *(data++);

				if (nbt::checkTag(data, nbt::tag::Compound))
					item_data->read(data);
			}

			Slot* item = new Slot(itemId, present, count, item_data);
			play::receive::creativeInventoryAction(p, slot, item);
		}
		break;
		case play::id::updateJigsawBlock:
		{
			Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: update jigsaw block" << Log::endl;
		}
		break;
		case play::id::updateStructureBlock:
		{
			Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: update structure block" << Log::endl;
		}
		break;
		case play::id::updateSign:
		{
			Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: update sign" << Log::endl;
		}
		break;
		case play::id::animation_serverbound:
		{
			varInt hand;
			hand.read(data);
			play::receive::animation(p, hand);
		}
		break;
		case play::id::spectate:
		{
			Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: spectate" << Log::endl;
		}
		break;
		case play::id::playerBlockPlacement:
		{
			varInt hand, face;
			Position location;
			bfloat curX, curY, curZ;
			bool insideBlock;

			hand.read(data);
			location.read(data);
			face.read(data);
			curX.read(data);
			curY.read(data);
			curZ.read(data);
			insideBlock = *(data++);

			play::receive::playerBlockPlacement(p, (Hand)(int)hand, location, (playerDigging::face)(int)face, curX, curY, curZ, insideBlock);
			Log::debug(PROTOCOL_WARNINGS) << "Partially handled packet: player block placement" << Log::endl;
		}
		break;
		case play::id::useItem:
		{
			varInt hand;
			hand.read(data);

			play::receive::useItem(p, (Hand)(int)hand);
		}
		break;
		default:
			p->disconnect();
			throw protocolError("Invalid packet id");
		}
		break;
	default:
		p->disconnect();
		throw protocolError("Invalid connection state");
	}
}