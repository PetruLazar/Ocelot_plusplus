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
#include "../../player/inventory.h"
#include "../command.h"

void message::handshake::receive::standard(Player* p, varInt protocolVersion, const mcString& serverAdress, Port port, varInt nextState)
{
	p->protocolVersion = protocolVersion;
	p->state = (ConnectionState)(int)nextState;
}
void message::handshake::receive::legacy(Player* p, Byte payload)
{
	throw "Legacy handshake not yet implemented";
}

void message::status::receive::request(Player* p)
{
	Log::info() << p->netId() << " is pinging the server.\n";

	//redo sample players
	message::status::send::respose(p, "{" + Options::version.to_string() + ",\"players\":{\"max\":" + std::to_string(Options::max_players()) + ",\"online\":" + std::to_string(Player::computeOnlinePlayers()) + "},\"description\":" + Options::motd() + "}");
}
void message::status::receive::ping(Player* p, blong payload)
{
	message::status::send::pong(p, payload);
}

void message::login::receive::start(Player* p, const mcString& username)
{
	if (!Options::allowJoin)
	{
		login::send::disconnect(p, "{\"text\":\"Not allowed to join!\"}");
		return;
	}
	if (p->protocolVersion != Options::currentProtocol())
	{
		login::send::disconnect(p, "{\"text\":\"Use version " + Options::currentVersion() + "!\"}");
		return;
	}

	//player initialization
	//general fields
	p->username = username;
	p->viewDistance = Options::viewDistance();
	p->simulationDistance = Options::simulationDistance();
	p->gm = gamemode::creative;
	//position, rotation ad world
	//p->world = World::worlds[World::spawnWorld];
	//p->X = p->world->spawn.X;
	//p->Y = p->world->spawn.Y;
	//p->Z = p->world->spawn.Z;
	//p->chunkX = p->world->spawn.ChunkX;
	//p->chunkZ = p->world->spawn.ChunkZ;
	//p->yaw = p->world->spawn.Yaw;
	//p->pitch = p->world->spawn.Pitch;
	//playerInfo data
	p->ping = -1;
	p->hasDisplayName = false;
	//p->displayName = new Chat(("[Tester] " + username).c_str());

	Log::info() << p->username << '(' << p->netId() << ") is logging in (eid: " << p->getEid() << ").\n";

	short netCThres = Options::networkCompression();
	if (netCThres != -1)
		login::send::setCompression(p, netCThres);

	login::send::success(p, *p->euuid, username);

	play::send::joinGame(p, (int)p->getEid(), false, gamemode::creative, gamemode::none, 0, nullptr, World::dimension_codec, World::spawnWorld->characteristics, World::spawnWorld->name, 0x5f19a34be6c9129a, 0, p->viewDistance, p->simulationDistance, false, false, false, World::spawnWorld->isFlat);

	play::send::pluginMessage(p, "minecraft:brand", 9, "\x8Ocelot++");

	play::send::serverDifficulty(p, 2, false);

	play::send::playerAbilities(p, true, true, true, p->gm == gamemode::creative, 0.05f, 0.1f);

	play::send::declareRecipes(p, (int)recipe::Manager::recipes.size(), &recipe::Manager::recipes);
	//unlock all recipes by default
	play::send::unlockRecipes(p, 0, false, false, false, false, false, false, false, false, (int)recipe::Manager::recipesIDs.size(), &recipe::Manager::recipesIDs, recipe::Manager::recipesIDs.size(), &recipe::Manager::recipesIDs);

	std::vector<Player*> inGamePlayers;
	for (Player* player : Player::players) if (player->state == ConnectionState::play && player->Connected()) inGamePlayers.emplace_back(player);
	Player** playerInfoList = inGamePlayers.data();
	play::send::playerInfo(p, playerInfo::addPlayer, (int)inGamePlayers.size(), playerInfoList);

	play::send::tags(p);

	play::send::declareCommands(p);

	Player::broadcastChat(Chat((p->username + " joined the game").c_str(), Chat::color::yellow()), p);
	broadcastMessageOmit(play::send::playerInfo(player_macro, playerInfo::addPlayer, 1, &p), p);

	p->setWorld(World::spawnWorld);

	/*play::send::updateViewPosition(p, p->chunkX, p->chunkZ);

	play::send::timeUpdate(p, 6000i64, 6000i64);

	play::send::spawnPosition(p, p->world->spawn.Absolute, 0.f);

	for (int x = p->chunkX - p->viewDistance; x <= p->chunkX + p->viewDistance; x++) for (int z = p->chunkZ - p->viewDistance; z <= p->chunkZ + p->viewDistance; z++)
	{
		play::send::sendFullChunk(p, x, z);
		//play::send::updateLight(p, x, z);
		//play::send::chunkData(p, x, z);
	}

	play::send::playerPosAndLook(p, p->X, p->Y, p->Z, p->yaw, p->pitch, 0, false);*/
}
void message::login::receive::encryptionResponse(Player*, varInt sharedSecretLength, Byte* sharedSecret, varInt verifyTokenLength, Byte* verifyToken)
{
	throw protocolError("Encryption not supported");
}

void message::play::receive::keepAlive(Player* p, blong keepAlive_id)
{
	if (keepAlive_id != p->lastKeepAliveId) return;
	p->ping = (int)((int64)cycleTime - (int64)keepAlive_id);
	broadcastMessage(send::playerInfo(player_macro, playerInfo::updateLatency, 1, &p));
	p->lastKeepAliveId = -1;
}
void message::play::receive::lockDifficulty(Player* p, bool locked)
{
	//unused
	Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: lock difficulty";
}
void message::play::receive::teleportConfirm(Player* p, varInt teleportId)
{
	if (teleportId == p->pendingTpId)
		p->pendingTpId = -1;
}
void message::play::receive::setDifficulty(Player* p, Byte difficulty)
{
	//unused
	Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: set difficulty";
}
void message::play::receive::clientStatus(Player*, varInt actionId)
{

}
void message::play::receive::clientSettings(Player* p, const mcString& locale, Byte viewDistance, ChatMode chatMode, bool chatColors, Byte displayedSkinParts, varInt mainHand, bool enableTextFiltering, bool allowServerListings)
{
	p->locale = locale;
	p->UpdateViewDistance(viewDistance);
	p->chatMode = chatMode;
	p->chatColors = chatColors;
	p->displayedSkinParts = displayedSkinParts;
	p->mainHand = mainHand;
	p->enableTextFiltering = enableTextFiltering;
	p->allowServerListings = allowServerListings;
}
void message::play::receive::clickWindowButton(Player*, Byte windowID, Byte buttonId)
{
	Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: clickWindowButton";
}
void message::play::receive::clickWindow(Player* p, Byte windowID, varInt stateID, bshort clickedSlotIndex, Byte button, varInt mode, varInt length, bshort* slotNumbers, Slot** slots, Slot* clickedItem)
{
	Log::debug(PROTOCOL_WARNINGS) << "Partially handled packet: clickWindow" << Log::endl;

	Log::info() << "WindowID: " << (int)windowID << " StateID: " << stateID << Log::endl;
	Log::info() << "Mode: " << mode << "\tButton: " << (int)button << "\tClickedSlot : " << (int)clickedSlotIndex << " Length: " << length << Log::endl;
	for (int i = 0; i < length; i++)
		Log::info() << "\tindex: " << i << " sn: " << slotNumbers[i] << Log::endl;
	Log::info() << Log::endl;

	int inventoryFirstSlotIndex = (windowID == 0) ? 9 : window::getWindowSlotCount(p->inventory->getLatestWindow(windowID));
	Log::info() << "iFSI: " << inventoryFirstSlotIndex << Log::endl;

	switch (mode)
	{
	case 0:
		if (button == 0)
		{
			if (clickedSlotIndex == -999)
			{ //drop whole slot
				Entity::item* theItem = new Entity::item(Entity::entity(p->world->getEidDispenser(), Entity::type::minecraft_item, p->x, p->y + 1.25, p->z, 0.7, 0.6), new Slot(*p->inventory->getFloatingSlot()));
				p->inventory->setFloatingSlot(Slot());

				p->world->addEntity(theItem);

				for (Player* seener : p->seenBy)
				{
					message::play::send::spawnEntity(seener, theItem, 100, 0, 100);
					message::play::send::entityMetadata(seener, theItem->getEid(), Entity::Metadata(8, Entity::Metadata::type::_Slot, theItem->theItem));
				}
				message::play::send::spawnEntity(p, theItem, 100, 0, 100);
				message::play::send::entityMetadata(p, theItem->getEid(), Entity::Metadata(8, Entity::Metadata::type::_Slot, theItem->theItem));
			}
			else
			{ //select or place slot
				if (clickedSlotIndex == -1)
					return;

				Slot* clicked = p->inventory->getSlotByIndex(clickedSlotIndex);
				Slot* floating = p->inventory->getFloatingSlot();

				if (clicked->getItemId() == floating->getItemId())
				{
					unsigned remained = p->inventory->addToSlot(*floating, clickedSlotIndex);

					if (remained != floating->count)
						floating->count = floating->count - remained;
					else
						p->inventory->setFloatingSlot(Slot());
				}
				else
					p->inventory->swapWithFloating(clickedSlotIndex);
			}
		}
		else
		{	//button == 1
			if (clickedSlotIndex == -999)
			{	//drop one item from slot
				Slot* floatingSlot = p->inventory->getFloatingSlot();
				floatingSlot->count--; //decrease the floating item count;

				Slot* dropSlot = new Slot(*floatingSlot);
				dropSlot->count = 1; //copy the floating item, but with 1 count

				Entity::item* theItem = new Entity::item(Entity::entity(p->world->getEidDispenser(), Entity::type::minecraft_item, p->x, p->y + 1.25, p->z, 0.7, 0.6), dropSlot);

				p->world->addEntity(theItem);

				for (Player* seener : p->seenBy)
				{
					message::play::send::spawnEntity(seener, theItem, 100, 0, 100);
					message::play::send::entityMetadata(seener, theItem->getEid(), Entity::Metadata(8, Entity::Metadata::type::_Slot, theItem->theItem));
				}
				message::play::send::spawnEntity(p, theItem, 100, 0, 100);
				message::play::send::entityMetadata(p, theItem->getEid(), Entity::Metadata(8, Entity::Metadata::type::_Slot, theItem->theItem));
			}
			else
			{	//select half from slot
				if (clickedSlotIndex == -1)
					return;

				Slot* selectedSlot = p->inventory->getSlotByIndex(clickedSlotIndex);
				Slot newFloatingSlot = *selectedSlot;

				newFloatingSlot.count /= 2;
				if (selectedSlot->count % 2 == 1)
					newFloatingSlot.count++;

				p->inventory->setFloatingSlot(newFloatingSlot);
				selectedSlot->count = selectedSlot->count / 2;
			}
		}
		break;
	case 1: {
		//button 0 and 1 are doing identical behaviors
		//move whole slot to appropiate slot
		if (length == 0)
			return; //pretty much just a notice that the slot wasnt moved
		else if (length != 2)
		{
			Log::warn() << "Click Window: Mode 1, length is not 2!" << Log::endl;
			return;
		}

		Byte toSlotIndex = slotNumbers[0];
		if (clickedSlotIndex == toSlotIndex)
			toSlotIndex = slotNumbers[1];

		//maybe do some sanitary checks?
		Slot* clickedSlot = p->inventory->getSlotByIndex(clickedSlotIndex);
		Byte pickedAmount = p->inventory->addToSlot(*clickedSlot, toSlotIndex);

		if (pickedAmount == clickedSlot->count)
			p->inventory->setSlotByIndex(clickedSlotIndex, Slot());
		else
			clickedSlot->count -= pickedAmount;
	}
		break;
	case 2:
		if (button == 40)
		{	//offhand swap
			Slot* selectedSlot = p->inventory->getSlotByIndex(clickedSlotIndex);
			Slot* floatingSlot = p->inventory->getFloatingSlot();

			std::swap(selectedSlot, floatingSlot);

			message::play::send::setSlot(p, 0, 0, 45, floatingSlot);
			message::play::send::setSlot(p, 0, 0, clickedSlotIndex, selectedSlot);
		}
		else
		{	//button is hotbar index from 0
			Slot* selectedSlot = p->inventory->getSlotByIndex(clickedSlotIndex);
			Slot* hotbarSlot = p->inventory->getHotbarSlot(button);

			std::swap(selectedSlot, hotbarSlot);

			message::play::send::setSlot(p, 0, 0, 36 + button, hotbarSlot);
			message::play::send::setSlot(p, 0, 0, clickedSlotIndex, selectedSlot);
		}
		break;
	case 3:
		//middle click, for creative players in non-player inventories
		if (p->gm == gamemode::creative)
		{
			Slot* clicked = p->inventory->getSlotByIndex(clickedSlotIndex);
			p->inventory->setFloatingSlot(*clicked);
		}
		break;
	case 4:
		if (clickedSlotIndex == -999 || clickedSlotIndex == -1)
			return;

		if (button == 0)
		{ //q, drop one from whole slot
			Slot* selectedSlot = p->inventory->getSlotByIndex(clickedSlotIndex);
			selectedSlot->count -= 1; //decrease the floating item count;

			Slot* dropSlot = new Slot(*selectedSlot);
			dropSlot->count = 1; //copy the floating item, but with 1 count

			Entity::item* theItem = new Entity::item(Entity::entity(p->world->getEidDispenser(), Entity::type::minecraft_item, p->x, p->y + 1.25, p->z, 0.7, 0.6), dropSlot);

			p->world->addEntity(theItem);

			for (Player* seener : p->seenBy)
			{
				message::play::send::spawnEntity(seener, theItem, 100, 0, 100);
				message::play::send::entityMetadata(seener, theItem->getEid(), Entity::Metadata(8, Entity::Metadata::type::_Slot, theItem->theItem));
			}
			message::play::send::spawnEntity(p, theItem, 100, 0, 100);
			message::play::send::entityMetadata(p, theItem->getEid(), Entity::Metadata(8, Entity::Metadata::type::_Slot, theItem->theItem));
		}
		else
		{ //ctrl + q, button == 1 drop whole slot
			Entity::item* theItem = new Entity::item(Entity::entity(p->world->getEidDispenser(), Entity::type::minecraft_item, p->x, p->y + 1.25, p->z, 0.7, 0.6), new Slot(*p->inventory->getSlotByIndex(clickedSlotIndex)));
			p->inventory->setSlotByIndex(clickedSlotIndex, Slot());

			p->world->addEntity(theItem);

			for (Player* seener : p->seenBy)
			{
				message::play::send::spawnEntity(seener, theItem, 100, 0, 100);
				message::play::send::entityMetadata(seener, theItem->getEid(), { Entity::Metadata(8, Entity::Metadata::type::_Slot, theItem->theItem) });
			}
			message::play::send::spawnEntity(p, theItem, 100, 0, 100);
			message::play::send::entityMetadata(p, theItem->getEid(), { Entity::Metadata(8, Entity::Metadata::type::_Slot, theItem->theItem) });
		}
		break;
	case 5:
		switch (button)
		{
		case 0: //start left mouse drag
			p->inventory->paintStart(inventoryPaint::left);
			break;
		case 4: //start right mouse drag
			p->inventory->paintStart(inventoryPaint::right);
			break;
		case 8: //start middle mouse drag (creative players in non-creative players inv)
			if(p->gm == gamemode::creative)
				p->inventory->paintStart(inventoryPaint::middle);
			break;
		case 1: //add slot left mouse drag
			p->inventory->paintProgress(inventoryPaint::left, (Byte)clickedSlotIndex);
			break;
		case 5: //add slot right mouse drag
			p->inventory->paintProgress(inventoryPaint::right, (Byte)clickedSlotIndex);
			break;
		case 9: //add slot middle mouse drag (creative players in non-creative players inv)
			if (p->gm == gamemode::creative)
				p->inventory->paintProgress(inventoryPaint::middle, (Byte)clickedSlotIndex);
			break;
		case 2: //end left mouse drag
			p->inventory->paintStop(inventoryPaint::left, slotNumbers, length);
			break;
		case 6: //end right mouse drag
			p->inventory->paintStop(inventoryPaint::right, slotNumbers, length);
			break;
		case 10: //end middle mouse drag (creative players in non-creative players inv)
			if (p->gm == gamemode::creative)
				p->inventory->paintStop(inventoryPaint::middle, slotNumbers, length);
			break;
		}
		break;
	case 6: {
		//double click
		Slot* clicked = p->inventory->getFloatingSlot();
		if (!clicked->isPresent())
			return;

		p->inventory->stackItem(clicked);
	}
		break;
	}
}
void message::play::receive::closeWindow(Player* p, Byte winId)
{
	if (winId != 0)
		p->inventory->closeWindow(winId);

	message::play::send::closeWindow(p, winId);
}
void message::play::receive::editBook(Player* p, varInt hand, varInt count, const std::vector<mcString>& pages, bool hasTitle, const mcString& title)
{
	nbt** pagesToNbt = new nbt * [count];
	for (int i = 0; i < count; i++)
		pagesToNbt[i] = new nbt_string(std::to_string(i), pages[i]);

	nbt_compound* bookData;

	if (hasTitle)
		bookData = new nbt_compound("", new nbt * [3]{
			new nbt_string("author", p->username),
			new nbt_string("title", title),
			new nbt_list("pages", pagesToNbt, count)
			}, 3);
	else
		bookData = new nbt_compound("", new nbt * [1]{ new nbt_list("pages", pagesToNbt, count) }, 1);

	if (hasTitle)
	{
		p->inventory->setSlotByIndex(p->inventory->getSelectedIndex(true), Slot(943, bookData));

		Equipment* eqp = new Equipment(0, p->inventory->getSelectedSlot());

		for (Player* seener : p->seenBy)
			message::play::send::entityEquipment(seener, p->getEid(), eqp);

		message::play::send::entityEquipment(p, p->getEid(), eqp);

		delete eqp;
	}
	else
		p->inventory->getSelectedSlot()->updateNBT(bookData);
}
void message::play::receive::interactEntity(Player* p, varInt eid, varInt type, bfloat targetX, bfloat targetY, bfloat targetZ, Hand mainHand, bool sneaking)
{

	if (type == 2)
	{

	}

}
void message::play::receive::chatMessage(Player* p, mcString& content)
{
	Log::info() << "[Chat] " << p->username << ": " << content << Log::endl;
	if (content[0] == '/')
	{
		try
		{
			content.erase(0, 1);
			try
			{
				if (!Commands::dispatch(p, content)) throw Chat("Unknown command", Chat::color::red());
			}
			catch (const Chat& msg)
			{
				send::chatMessage(p, msg, ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
			}
			catch (...)
			{
				send::chatMessage(p, Chat("Internal error occured", Chat::color::red()), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
			}
		}
		catch (const Chat& errormessage)
		{
			send::chatMessage(p, errormessage, 1, mcUUID(0, 0, 0, 0));
		}
		catch (...)
		{
			throw;
		}
	}
	else
	{
		Chat msg(('<' + p->username + "> " + content).c_str());

		for (Player* pl : Player::players)
			message::play::send::chatMessage(pl, msg, 0, *p->euuid);
	}
}
void processGroundItem(Player* p)
{
	//minecraft picks up the entire batch if its pickupable, here, it picks up a batch, then the remaining one
	for (Entity::entity* groundItem : p->world->getEntitiesByType(Entity::type::minecraft_item))
	{
		if (Position::inRange(groundItem->x, groundItem->y, groundItem->z, p->x, p->y, p->z, 2))
		{
			Entity::item* droppedItem = static_cast<Entity::item*>(groundItem);

			if (droppedItem->spawnedTimeStamp + 1500 > utility::time::timeSinceEpoch())
				continue;

			auto pickedDataArray = p->inventory->addToInventory(*droppedItem->theItem);
			Byte totalPickedCount = 0;

			for (auto& pickedData : pickedDataArray)
			{
				Byte pickedCount = pickedData.first;
				Byte pickedIndex = pickedData.second;

				message::play::send::collectItem(p, groundItem->getEid(), p->getEid(), pickedCount);

				Slot* pickedupSlot = p->inventory->getSlotByIndex(pickedIndex);
				message::play::send::setSlot(p, (pickedIndex > 35 ? 0 : -2), 0, pickedIndex, pickedupSlot);

				totalPickedCount += pickedCount;
			}

			varInt droppedItemCount = droppedItem->theItem->count;
			if (totalPickedCount == droppedItemCount)
			{ //all of the entity got picked up, destroy it
				message::play::send::destroyEntity(p, droppedItem->getEid()); //seeners too
				p->world->removeEntity(droppedItem->getEid());
			}
			else if (totalPickedCount < droppedItemCount)
			{ //not everything got picked up, update the data
				droppedItem->theItem->count = droppedItemCount - totalPickedCount;
				message::play::send::entityMetadata(p, droppedItem->getEid(), Entity::Metadata(8, Entity::Metadata::type::_Slot, droppedItem->theItem));
			}
		}
	}
}
void message::play::receive::playerPosition(Player* p, bdouble X, bdouble feetY, bdouble Z, bool onGround)
{
	if (p->pendingTpId != -1)
		return;

	p->updatePosition(X, feetY, Z);
	p->onGround = onGround;

	processGroundItem(p);
}
void message::play::receive::playerPositionAndRotation(Player* p, bdouble X, bdouble Y, bdouble Z, bfloat yaw, bfloat pitch, bool onGround)
{
	if (p->pendingTpId != -1)
		return;

	p->onGround = onGround;
	p->updatePosition(X, Y, Z);
	p->updateRotation(yaw, pitch);

	processGroundItem(p);
}
void message::play::receive::playerRotation(Player* p, bfloat yaw, bfloat pitch, bool onGround)
{
	p->onGround = onGround;
	p->updateRotation(yaw, pitch);
}
void message::play::receive::pickItem(Player* p, varInt slot)
{
	Log::debug(PROTOCOL_WARNINGS) << "Unhandled packet: pickItem";
	//Log::info() << "pickite";
	//Log::info() << "a: " << p->slots[slot]->getItemId();
	varInt foundSlot = 0;
	for (int i = 0; i < 9; i++)
	{ //first search should start from the current slot and loop around it
		if (!p->inventory->getHotbarSlot(i)->isPresent())
			foundSlot = i;
	}

	if (!foundSlot)
	{ //start second search

	}

	if (!foundSlot) //still no good slot found, use the current one
		foundSlot = slot;

	//message::play::send::setSlot(p, -2, 0, )

	//message::play::send::heldItemChange(p, foundSlot);
}
void message::play::receive::craftRecipeRequest(Player* p, Byte winId, const mcString& recipe, bool makeAll)
{

	//not finished!
	message::play::send::craftRecipeResponse(p, winId, recipe);
}
void message::play::receive::resourcePackStatus(Player* p, varInt result)
{

}
void message::play::receive::advancementTab(Player*, varInt action, const mcString& tabId)
{

}
void message::play::receive::heldItemChange(Player* p, bshort slot)
{
	p->inventory->setSelectedIndex((Byte)slot);

	Equipment* eqp = new Equipment(0, p->inventory->getSelectedSlot());

	for (Player* seener : p->seenBy)
		message::play::send::entityEquipment(seener, p->getEid(), eqp);

	delete eqp;
}
void message::play::receive::creativeInventoryAction(Player* p, bshort slot, Slot* clickedItem)
{
	if (slot == -1)
	{ //throw away from inventory, create entity
		Entity::item* theItem = new Entity::item(Entity::entity(p->world->getEidDispenser(), Entity::type::minecraft_item, p->x, p->y + 1.25, p->z, 0.7, 0.6), new Slot(*clickedItem));

		p->world->addEntity(theItem);

		for (Player* seener : p->seenBy)
		{
			message::play::send::spawnEntity(seener, theItem, 100, 0, 100);
			message::play::send::entityMetadata(seener, theItem->getEid(), Entity::Metadata(8, Entity::Metadata::type::_Slot, theItem->theItem));
		}
		message::play::send::spawnEntity(p, theItem, 100, 0, 100);
		message::play::send::entityMetadata(p, theItem->getEid(), Entity::Metadata(8, Entity::Metadata::type::_Slot, theItem->theItem));
	}
	else
	{ //put in inventory
		p->inventory->setSlotByIndex(slot, *clickedItem);

		if (p->inventory->getSelectedIndex() == slot)
		{
			Equipment* eqp = new Equipment(0, clickedItem);

			for (Player* seener : p->seenBy)
				message::play::send::entityEquipment(seener, p->getEid(), eqp);

			delete eqp;
		}
	}
}
void message::play::receive::animation(Player* p, varInt hand)
{
	Entity::animation animation = (hand == p->mainHand) ? Entity::animation::swingMainArm : Entity::animation::swingOffhand;

	for (Player* seener : p->seenBy)
		ignoreExceptions(message::play::send::entityAnimation(seener, p->getEid(), animation));
}
void message::play::receive::playerDigging(Player* p, varInt status, const Position& location, Byte face)
{
	switch (status)
	{
	case playerDigging::startedDigging:
	{
		if (p->gm != gamemode::creative) break;
		//startedDigging actually breaks the block if the block breaks instantly (creative, string haste or efficiency, probably (untested) grass/torch/etc)
		sf::Vector3i v = location.get();
		v.y = p->world->AbsToRelHeight(v.y);
		if (!p->world->checkCoordinates(v.y)) throw std::exception("playerDigging location outside world");
		p->world->setBlockNoBroadcast(v.x, v.y, v.z, 0);
		//to do: send "acknowledge player digging" instead of "block change"
		for (Player* other : p->world->players)
			if (other != p/* && other->positionInRange(location)*/)
				//send::blockChange(other, location, 0);
				send::acknowledgePlayerDigging(other, location, 0, status, true);
	}
	break;
	case playerDigging::cancelledDigging:

		break;
	case playerDigging::finishedDigging:
	{
		sf::Vector3i v = location.get();
		v.y = p->world->AbsToRelHeight(v.y);
		if (!p->world->checkCoordinates(v.y)) throw std::exception("playerDigging location outside world");
		p->world->setBlockNoBroadcast(v.x, v.y, v.z, 0);
		//to do: send "acknowledge player digging" instead of "block change"
		for (Player* other : p->world->players)
			if (other != p/* && other->positionInRange(location)*/)
				//send::blockChange(other, location, 0);
				send::acknowledgePlayerDigging(other, location, 0, status, true);
	}
	break;
	case playerDigging::dropItemStack:
	{
		Slot* playerItem = p->inventory->getSelectedSlot();

		if (playerItem->count == 0) //empty slot
			return;

		Entity::item* theItem = new Entity::item(Entity::entity(p->world->getEidDispenser(), Entity::type::minecraft_item, p->x, p->y + 1.25, p->z, 0.7, 0.6), new Slot(*playerItem));
		p->world->addEntity(theItem);

		for (Player* seener : p->seenBy)
		{
			message::play::send::spawnEntity(seener, theItem, 100, 0, 100);
			message::play::send::entityMetadata(seener, theItem->getEid(), Entity::Metadata(8, Entity::Metadata::type::_Slot, theItem->theItem));
		}
		message::play::send::spawnEntity(p, theItem, 100, 0, 100);
		message::play::send::entityMetadata(p, theItem->getEid(), Entity::Metadata(8, Entity::Metadata::type::_Slot, theItem->theItem));

		*playerItem = Slot();

		Equipment* eqp = new Equipment(0, p->inventory->getSelectedSlot());
		for (Player* seener : p->seenBy)
			message::play::send::entityEquipment(seener, p->getEid(), eqp);

		delete eqp;
	}
	break;
	case playerDigging::dropItem:
	{
		Slot* playerItem = p->inventory->getSelectedSlot();

		if (!playerItem->isPresent()) //empty slot
			return;

		Entity::item* theItem = new Entity::item(Entity::entity(p->world->getEidDispenser(), Entity::type::minecraft_item, p->x, p->y + 1.25, p->z, 0.7, 0.6), new Slot(*playerItem));
		theItem->theItem->count = 1;

		playerItem->count -= 1;
		if (playerItem->count == 0)
		{
			*playerItem = Slot();

			Equipment* eqp = new Equipment(0, p->inventory->getSelectedSlot());
			for (Player* seener : p->seenBy)
				message::play::send::entityEquipment(seener, p->getEid(), eqp);

			delete eqp;
		}

		p->world->addEntity(theItem);

		for (Player* seener : p->seenBy)
		{
			message::play::send::spawnEntity(seener, theItem, 100, 0, 100);
			message::play::send::entityMetadata(seener, theItem->getEid(), Entity::Metadata(8, Entity::Metadata::type::_Slot, theItem->theItem));
		}
		message::play::send::spawnEntity(p, theItem, 100, 0, 100);
		message::play::send::entityMetadata(p, theItem->getEid(), Entity::Metadata(8, Entity::Metadata::type::_Slot, theItem->theItem));
	}
	break;
	case playerDigging::shootArrow: //also finish eating

		break;
	case playerDigging::swapItemInHand:
	{
		bshort positionIndex = p->inventory->getSelectedIndex(true);

		p->inventory->swapSlots(45, positionIndex);

		message::play::send::setSlot(p, 0, 0, 45, p->inventory->getSlotByIndex(45));
		message::play::send::setSlot(p, 0, 0, positionIndex, p->inventory->getSlotByIndex(positionIndex));
	}
	break;
	}
}
void message::play::receive::entityAction(Player* p, varInt eid, varInt actionId, varInt jumpBoost)
{
	switch (actionId)
	{
	case 0: //start sneaking
		p->attributes |= 0x02;
		break;
	case 1: //stop sneaking
		p->attributes &= ~(0x02);
		break;
	case 2: //leave bed

		break;
	case 3: //start sprinting
		p->attributes |= 0x08;
		break;
	case 4: //stop sprinting
		p->attributes &= ~(0x08);
		break;
	case 5: //start jump with horse

		break;
	case 6: //stop jump with horse

		break;
	case 7: //open horse inventory

		break;
	case 8: //start flying with elytra
		p->attributes |= 0x80;
		break;
	}

	for (Player* seener : p->seenBy)
	{
		message::play::send::entityMetadata(seener, eid, Entity::Metadata(0, Entity::Metadata::type::_Byte, &p->attributes));

		varInt entityPose = p->attributes == 0x02 ? Entity::pose::sneaking : Entity::pose::standing;
		message::play::send::entityMetadata(seener, eid, Entity::Metadata(6, Entity::Metadata::type::_Pose, &entityPose));
	}
}
void message::play::receive::pong(Player* p, bint id)
{
	message::play::send::ping(p, id);
}
void message::play::receive::setDisplayedRecipe(Player*, const mcString& recipeId)
{

}
void message::play::receive::nameItem(Player*, const mcString& newName)
{
	//de facut cand o sa am anvilurile disponibile
}
void message::play::receive::playerBlockPlacement(Player* p, Hand hand, const Position& location, BlockFace face, bfloat curX, bfloat curY, bfloat curZ, bool insideBlock)
{
	//std::string text = "playerBlockPlacement: ";
	Slot* slot = nullptr;

	if (hand == Hand::main)
	{
		//text += "main ";
		slot = p->inventory->getSelectedSlot();
	}
	else
	{ //hand == Hand::offhand
		//text += "off";
		slot = p->inventory->getOffhandSlot();
	}

	/*text += "hand, ";

	switch (face)
	{
	case playerDigging::top:
		text += "top";
		break;
	case playerDigging::bottom:
		text += "bottom";
		break;
	case playerDigging::east:
		text += "east";
		break;
	case playerDigging::west:
		text += "west";
		break;
	case playerDigging::south:
		text += "south";
		break;
	case playerDigging::north:
		text += "north";
	}

	text += ", (" + std::to_string(curX) + ' ' + std::to_string(curY) + ' ' + std::to_string(curZ) + "), ";

	Log::info() << '\n' << p->username << " - " << text;*/
	//play::send::chatMessage(p, Chat(text.c_str()), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));

	p->world->setBlockByItem(p, slot, location, face, curX, curY, curZ);
}
void message::play::receive::useItem(Player* p, Hand hand)
{
	switch (p->inventory->getSelectedSlot()->getItemId())
	{
	case 943:
		message::play::send::openBook(p, hand);
		break;
	default:
		Log::info() << "useItem unhandled: " << p->inventory->getSelectedSlot()->getItemId() << Log::endl;
		break;
	}
}