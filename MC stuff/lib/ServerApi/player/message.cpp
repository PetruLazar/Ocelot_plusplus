#include "message.h"

#include "../server/options.h"
#include "../types/gametags.h"
#include "../types/chat.h"
#include "../types/error.h"
#include "../types/utils.h"
#include "../types/node.h"
#include "command.h"
#include "../types/enums.h"
#include "../types/basic.h"
#include "../server/log.h"
#include "../mcexception.h"
#include "../types/window.h"

#define prepareSendMacro(x) char* data = new char[x] + 6, *start = data
#define prepareSendMacroNoDecl(x) data = new char[x] + 6; start = data
#define finishSendMacro ull size = data - start;		\
	preparePacket(p, start, size, data);				\
	p->schedulePacket(start, size, data);
#define finishSendMacroNoDecl size = data - start;		\
	preparePacket(p, start, size, data);				\
	p->schedulePacket(start, size, data);
#define finishSendAndDisconnect ull size = data - start;	\
	preparePacket(p, start, size, data);					\
	p->schedulePacket(start, size, data, true);

void message::handshake::receive::standard(Player* p, varInt protocolVersion, const mcString& serverAdress, Port port, varInt nextState)
{
	p->protocolVersion = protocolVersion;
	p->state = (ConnectionState)(int)nextState;
}
void message::handshake::receive::legacy(Player* p, Byte payload)
{
	throw "Legacy handshake not yet implemented";
}

void message::status::send::respose(Player* p, const mcString& jsonResponse)
{
	varInt id_ = (int)id::response;
	prepareSendMacro(1024 * 1024);

	id_.write(data);
	jsonResponse.write(data);

	finishSendMacro;
}
void message::status::send::pong(Player* p, blong payload)
{
	varInt id = (int)id::pong;
	prepareSendMacro(64);

	id.write(data);
	payload.write(data);

	finishSendAndDisconnect;
}

void message::status::receive::request(Player* p)
{
	Log::info() << p->netId() << " is pinging the server.\n";
	message::status::send::respose(p, "{" + Options::version + ",\"players\":{\"max\":" + std::to_string(rand() % 20 + 20) + ",\"online\":" + std::to_string(rand() % 20) + ",\"sample\":[{\"name\":\"TheGoldenSnowman\",\"id\":\"4566e69f-c907-48ee-8d71-d7ba5aa00d20\"},{\"name\":\"CosminPerRam\",\"id\":\"4566e69f-c907-48ee-8d71-d7ba5aa00d21\"},{\"name\":\"NativeLog05\",\"id\":\"4566e69f-c907-48ee-8d71-d7ba5aa00d22\"},{\"name\":\"TimmyBrott\",\"id\":\"4566e69f-c907-48ee-8d71-d7ba5aa00d23\"}]},\"description\":" + Options::motd() + "}");
}
void message::status::receive::ping(Player* p, blong payload)
{
	message::status::send::pong(p, payload);
}

void message::login::send::disconnect(Player* p, const mcString& reason)
{
	varInt id = (int)id::disconnect;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	reason.write(data);

	finishSendAndDisconnect;
}
void message::login::send::encryptionRequest(Player* p, varInt publicKeyLength, Byte* publicKey, varInt verifyTokenLength, Byte* verifyToken)
{
	throw "Encryption not supported";
}
void message::login::send::setCompression(Player* p, varInt threshold)
{
	varInt id = (int)id::setCompression;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	threshold.write(data);

	finishSendMacro;

	p->compressionEnabled = true;
	p->compressionThreshold = threshold;
}
void message::login::send::success(Player* p, const mcUUID& uuid, const mcString& username)
{
	p->state = ConnectionState::play;

	varInt id = (int)id::success;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	uuid.write(data);
	username.write(data);

	finishSendMacro;
}

void message::login::receive::start(Player* p, const mcString& username)
{
	if (!Options::allowJoin)
	{
		login::send::disconnect(p, "{\"text\":\"Fuck off, " + (std::string)username + "!\",\"color\":\"dark_red\",\"bold\":\"true\"}");
		return;
	}
	if (p->protocolVersion != Options::currentProtocol)
	{
		login::send::disconnect(p, "{\"text\":\"Use 1.18.1, " + (std::string)username + ", you nitwit!\",\"color\":\"red\",\"bold\":\"true\"}");
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

	play::send::pluginMessage(p, "minecraft:brand", 10, "\x9lazorenii");

	play::send::serverDifficulty(p, 2, false);

	play::send::playerAbilities(p, true, true, true, p->gm == gamemode::creative, 0.05f, 0.1f);

	play::send::declareRecipes(p, recipe::Manager::recipes->size(), recipe::Manager::recipes);
	//unlock all recipes by default
	play::send::unlockRecipes(p, 0, false, false, false, false, false, false, false, false, recipe::Manager::recipesIDs->size(), recipe::Manager::recipesIDs, recipe::Manager::recipesIDs->size(), recipe::Manager::recipesIDs);

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

void message::play::send::spawnEntity(Player* p, Entity::entity* entity, bshort velocityX, bshort velocityY, bshort velocityZ)
{
	message::play::send::spawnEntity(p, entity->getEid(), *entity->euuid, entity->getType(), entity->x, entity->y, entity->z, entity->pitch, entity->yaw, entity->data, velocityX, velocityY, velocityZ);
}
void message::play::send::spawnEntity(Player* p, varInt eid, const mcUUID& uuid, Entity::type type, bdouble x, bdouble y, bdouble z, Angle pitch, Angle yaw, bint Data, bshort velocityX, bshort velocityY, bshort velocityZ)
{
	varInt id = (int)id::spawnEntity;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);
	uuid.write(data);
	varInt((int)type).write(data);
	x.write(data);
	y.write(data);
	z.write(data);
	*(data++) = (char&)pitch;
	*(data++) = (char&)yaw;
	Data.write(data);
	velocityX.write(data);
	velocityY.write(data);
	velocityZ.write(data);

	finishSendMacro;
}
void message::play::send::spawnXPorb(Player* p, varInt eid, bdouble x, bdouble y, bdouble z, bshort xpCount)
{
	varInt id = (int)id::spawnXPorb;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);
	x.write(data);
	y.write(data);
	z.write(data);
	xpCount.write(data);

	finishSendMacro;
}
void message::play::send::spawnLivingEntity(Player* p, varInt eid, const mcUUID& uuid, Entity::type type, bdouble x, bdouble y, bdouble z, Angle yaw, Angle pitch, Angle headPitch, bshort velocityX, bshort velocityY, bshort velocityZ)
{
	varInt id = (int)id::spawnLivingEntity;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);
	uuid.write(data);
	varInt((int)type).write(data);
	x.write(data);
	y.write(data);
	z.write(data);
	*(data++) = (char&)yaw;
	*(data++) = (char&)pitch;
	*(data++) = (char&)headPitch;
	velocityX.write(data);
	velocityY.write(data);
	velocityZ.write(data);

	finishSendMacro;
}
void message::play::send::spawnPainting(Player* p, varInt eid, const mcUUID& uuid, Entity::Painting::motive motive, const Position& location, Entity::direction direction)
{
	varInt id = (int)id::spawnPainting;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);
	uuid.write(data);
	varInt(motive).write(data);
	location.write(data);
	*(data++) = (Byte)direction;

	finishSendMacro;
}
void message::play::send::spawnPlayer(Player* p, varInt eid, const mcUUID& uuid, bdouble x, bdouble y, bdouble z, Angle yaw, Angle pitch)
{
	varInt id = (int)id::spawnPlayer;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);
	uuid.write(data);
	x.write(data);
	y.write(data);
	z.write(data);
	*(data++) = (char&)yaw;
	*(data++) = (char&)pitch;

	finishSendMacro;
}
void message::play::send::sculkVibrationSignal(Player* p, const Position& source, Entity::Sculk::destinationType destinationType, const Entity::Sculk::destination& destination, varInt arrivalTime)
{
	varInt id = (int)id::sculkVibrationSignal;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	source.write(data);
	if (destinationType == Entity::Sculk::block)
	{
		mcString("block").write(data);
		destination.position.write(data);
	}
	else
	{
		mcString("entity").write(data);
		destination.eid.write(data);
	}
	arrivalTime.write(data);

	finishSendMacro;
}
void message::play::send::entityAnimation(Player* p, varInt eid, Entity::animation animation)
{
	varInt id = (int)id::entityAnimation;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);
	*(data++) = (Byte)animation;

	finishSendMacro;
}
void message::play::send::acknowledgePlayerDigging(Player* p, const Position& location, varInt block, varInt status, bool successful)
{
	varInt id = (int)id::acknowledgePlayerDigging;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	location.write(data);
	block.write(data);
	status.write(data);
	*(data++) = successful;

	finishSendMacro;
}
void message::play::send::blockBreakAnimation(Player* p, varInt eid, const Position& location, Byte destroyStage)
{
	varInt id = (int)id::blockBreakAnimation;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);
	location.write(data);
	*(data++) = destroyStage;

	finishSendMacro;
}
void message::play::send::blockEntityData(Player* p, const Position& location, blockEntityData::action action, const nbt& blockData)
{
	varInt id = (int)id::blockEntityData;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	location.write(data);
	varInt((int)action).write(data);
	blockData.write(data);

	finishSendMacro;
}
void message::play::send::blockAction(Player* p, const Position& location, Byte actionId, Byte actionParam, varInt blockType)
{
	varInt id = (int)id::blockAction;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	location.write(data);
	*(data++) = actionId;
	*(data++) = actionParam;
	blockType.write(data);

	finishSendMacro;
}
void message::play::send::blockChange(Player* p, const Position& location, varInt blockId)
{
	varInt id = (int)id::blockChange;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	location.write(data);
	blockId.write(data);

	finishSendMacro;
}
void message::play::send::bossBar(Player* p, const mcUUID& uuid, bossBar::action action, bossBar::mode* actionField)
{
	varInt id = (int)id::bossBar;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	uuid.write(data);
	varInt((int)action).write(data);
	actionField->write(data);

	finishSendMacro;
}
void message::play::send::keepAlive(Player* p, blong keepAlive_id)
{
	varInt id = (int)id::keepAlive_clientbound;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	keepAlive_id.write(data);

	finishSendMacro;
}
void message::play::send::joinGame(Player* p, bint eid, bool isHardcore, gamemode gm, gamemode prev_gm, varInt worldCount, mcString* worldNames, const nbt_compound& dimensionCodec,
	const nbt_compound& dimension, const mcString& worldName, int64 hashedSeedHigh, varInt maxPlayers, varInt viewDistance, varInt simulationDistance, bool reducedDebugInfo, bool respawnScreen, bool isDebug, bool isFlat)
{
	varInt id = (int)id::joinGame;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);
	*(data++) = isHardcore;
	*(((gamemode*&)data)++) = gm;
	*(((gamemode*&)data)++) = prev_gm;
	worldCount.write(data);
	for (int i = 0; i < worldCount; i++) worldNames[i].write(data);
	dimensionCodec.write(data);
	dimension.write(data);
	worldName.write(data);
	*(((int64*&)data)++) = hashedSeedHigh;
	maxPlayers.write(data);
	viewDistance.write(data);
	simulationDistance.write(data);
	*(data++) = reducedDebugInfo;
	*(data++) = respawnScreen;
	*(data++) = isDebug;
	*(data++) = isFlat;

	finishSendMacro;
}
void message::play::send::mapData(Player* p, varInt mapId, Byte scale, bool locked, bool trackingPosition, varInt iconCount, map::icon* icons, Byte optColumns, Byte optRows, Byte optX, Byte optZ, varInt optLength, Byte* optData)
{ //untested!!!
	varInt id = (int)id::mapData;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	mapId.write(data);
	*(data++) = scale;
	*(data++) = locked;
	*(data++) = trackingPosition;
	iconCount.write(data);
	for (int i = 0; i < iconCount; i++)
		icons[i].write(data);

	*(data++) = optColumns;
	if (optColumns > 0)
	{
		*(data++) = optRows;
		*(data++) = optX;
		*(data++) = optZ;
		optLength.write(data);
		for (int i = 0; i < optLength; i++)
			*(data++) = optData[i];
	}

	finishSendMacro;
}
void message::play::send::tradeList(Player* p, varInt winId, Byte tradesCount, trade* trades, varInt villagerLevel, varInt experience, bool isRegularVillager, bool canRestock)
{
	varInt id = (int)id::tradeList;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	winId.write(data);
	for (int i = 0; i < winId; i++)
		trades[i].write(data);

	villagerLevel.write(data);
	experience.write(data);
	*(data++) = isRegularVillager;
	*(data++) = canRestock;

	finishSendMacro;
}
void message::play::send::playerInfo(Player* p, varInt action, varInt playerCount, Player** players)
{
	varInt id = (int)id::playerInfo;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	action.write(data);
	playerCount.write(data);
	switch (action)
	{
	case playerInfo::addPlayer:
		for (int i = 0; i < playerCount; i++)
		{
			Player*& player = players[i];
			player->euuid->write(data);
			player->username.write(data);
			varInt(0).write(data);
			varInt((Byte)player->gm).write(data);
			player->ping.write(data);
			*(data++) = player->hasDisplayName;
			if (player->hasDisplayName) player->displayName->write(data);
		}
		break;
	case playerInfo::updateGamemode:
		for (int i = 0; i < playerCount; i++)
		{
			Player*& player = players[i];
			player->euuid->write(data);
			varInt((Byte)player->gm).write(data);
		}
		break;
	case playerInfo::updateLatency:
		for (int i = 0; i < playerCount; i++)
		{
			Player*& player = players[i];
			player->euuid->write(data);
			player->ping.write(data);
		}
		break;
	case playerInfo::updateDisplayName:
		for (int i = 0; i < playerCount; i++)
		{
			Player*& player = players[i];
			player->euuid->write(data);
			*(data++) = player->hasDisplayName;
			if (p->hasDisplayName) p->displayName->write(data);
		}
		break;
	case playerInfo::removePlayer:
		for (int i = 0; i < playerCount; i++)
		{
			Player*& player = players[i];
			player->euuid->write(data);
		}
		break;
	}

	finishSendMacro;
}
void message::play::send::facePlayer(Player* p, varInt pivot, bdouble targetX, bdouble targetY, bdouble targetZ, bool isEntity, varInt eid, varInt targetPivot)
{
	varInt id = (int)id::facePlayer;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	pivot.write(data);
	targetX.write(data);
	targetY.write(data);
	targetZ.write(data);
	*(data++) = isEntity;
	if (isEntity)
	{
		eid.write(data);
		targetPivot.write(data);
	}

	finishSendMacro;
}
void message::play::send::chunkDataAndLight(Player* p, bint cX, bint cZ, const nbt_compound& heightMaps, varInt dataSize, char* chunkData, varInt nOfBlockEntities, blockEntity** blockEntities,
	bool trustEdges, const BitArray& skyLightMask, const BitArray& blockLightMask, const BitArray& emptySkyLightMask, const BitArray& emptyBlockLightMask, varInt skyLightArrayCount, BitArray** skyLightArrays,
	varInt blockLightArrayCount, BitArray** blockLightArrays)
{
	varInt id = (int)id::chunkDataAndLight;
	prepareSendMacro(2048 * 1024);

	id.write(data);
	cX.write(data);
	cZ.write(data);

	//chunk data
	heightMaps.write(data);
	dataSize.write(data);
	std::memmove(data, chunkData, dataSize);
	data += dataSize;
	nOfBlockEntities.write(data);
	for (int i = 0; i < nOfBlockEntities; i++)
	{
		blockEntities[i]->write(data);
	}

	//light data
	*(data++) = trustEdges;
	varInt((uint)skyLightMask.getCompactedSize()).write(data);
	skyLightMask.write(data);
	varInt((uint)blockLightMask.getCompactedSize()).write(data);
	blockLightMask.write(data);
	varInt((uint)emptySkyLightMask.getCompactedSize()).write(data);
	emptySkyLightMask.write(data);
	varInt((uint)emptyBlockLightMask.getCompactedSize()).write(data);
	emptyBlockLightMask.write(data);

	skyLightArrayCount.write(data);
	for (int i = 0; i < skyLightArrayCount; i++)
	{
		LightSection::lightArrayLength.write(data);
		skyLightArrays[i]->writeAsLight(data);
	}

	blockLightArrayCount.write(data);
	for (int i = 0; i < blockLightArrayCount; i++)
	{
		LightSection::lightArrayLength.write(data);
		blockLightArrays[i]->writeAsLight(data);
	}

	finishSendMacro;
}
void message::play::send::chunkDataAndLight(Player*p, bint cX, bint cZ, bool increaseLoadCount)
{
	Chunk* chunk = p->world->getChunk(cX, cZ, increaseLoadCount);
	uint sectionCount = (uint)chunk->sections.size();

	//build the heightmaps
	nbt_compound heightMaps("", new nbt * [1]{
		new nbt_long_array("MOTION_BLOCKING",*chunk->heightmaps)
		}, 1);

	//build the chunkData
	char* chunkData = new char[0xfffff],
		*chunkDataStart = chunkData;
	for (uint i = 0; i < sectionCount; i++)
	{
		Section& section = chunk->sections[i];
		section.blockCount.write(chunkData);
		//send blocks
		if (section.blockCount)
		{
			//there are blocks in the section, send blockStates
			*(chunkData++) = section.bitsPerBlock;
			if (!section.useGlobalPallete)
			{
				varInt((uint)section.palette.size()).write(chunkData);
				for (PaletteEntry& val : section.palette) val.block.id.write(chunkData);
			}
			varInt((uint)section.blockStates->getCompactedSize()).write(chunkData);
			section.blockStates->write(chunkData);
		}
		else
		{
			//there are no blocks in the section, send all air
			*(int*)chunkData = 0;
			chunkData += 3;
			// 0 bits per entry
			// palette is one entry: 0
			// data array length is 0
		}
		//send biomes
		*(chunkData++) = World::currentBiomeBitsPerEntry;
		varInt((uint)section.biomes->getCompactedSize()).write(chunkData);
		section.biomes->write(chunkData);
	}
	uint dataSize = (uint)(chunkData - chunkDataStart);

	//change value to be ready for light sections
	sectionCount += 2;
	std::vector<BitArray*> skyLightArrays, blockLightArrays;
	for (uint i = 0; i < sectionCount; i++)
	{
		if (chunk->skyLightMask->getElement(i)) skyLightArrays.emplace_back(chunk->lightData[i].skyLight);
		if (chunk->blockLightMask->getElement(i)) blockLightArrays.emplace_back(chunk->lightData[i].blockLight);
	}

	chunkDataAndLight(p, cX, cZ, heightMaps, dataSize, chunkDataStart, 0, nullptr, true, *chunk->skyLightMask, *chunk->blockLightMask, *chunk->emptySkyLightMask, *chunk->emptyBlockLightMask, (uint)skyLightArrays.size(), skyLightArrays.data(), (uint)blockLightArrays.size(), blockLightArrays.data());
	delete[] chunkDataStart;
}
void message::play::send::effect(Player* p, bint effectId, const Position& location, bint extraData, bool disableRelativeVolume)
{
	varInt id = (int)id::effect;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	location.write(data);
	extraData.write(data);
	*(data++) = disableRelativeVolume;

	finishSendMacro;
}
void message::play::send::particle(Player* p, bint particleId, bool longDistance, bdouble x, bdouble y, bdouble z, bfloat offsetX, bfloat offsetY, bfloat offsetZ, bfloat particleData, bint count, particle::Particle* particle = nullptr)
{
	varInt id = (int)id::particle;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	particleId.write(data);
	*(data++) = longDistance;
	x.write(data);
	y.write(data);
	z.write(data);
	offsetX.write(data);
	offsetY.write(data);
	offsetZ.write(data);
	particleData.write(data);
	count.write(data);

	if (particle)
		particle->write(data, false);

	finishSendMacro;
}
void message::play::send::playerPosAndLook(Player* p, bdouble x, bdouble y, bdouble z, bfloat yaw, bfloat pitch, Byte flags, bool dismountVehicle)
{
	varInt id = (int)id::playerPosAndLook_clientbound;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	x.write(data);
	y.write(data);
	z.write(data);
	yaw.write(data);
	pitch.write(data);
	*(data++) = flags;
	p->pendingTpId = p->nextTpId++;
	p->pendingTpId.write(data);
	*(data++) = dismountVehicle;

	finishSendMacro;
}
void message::play::send::unlockRecipes(Player* p, varInt action, bool bookOpen, bool filterActive, bool smeltingOpen, bool smeltingFilter, bool blastOpen, bool blastFilter, bool smokerOpen, bool smokerFilter, varInt size1, std::vector<mcString>* array1, varInt size2, std::vector<mcString>* array2)
{
	varInt id = (int)id::unlockRecipes;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	action.write(data);
	*(data++) = bookOpen;
	*(data++) = filterActive;
	*(data++) = smeltingOpen;
	*(data++) = smeltingFilter;
	*(data++) = blastOpen;
	*(data++) = blastFilter;
	*(data++) = smokerOpen;
	*(data++) = smokerFilter;
	size1.write(data);
	for (int i = 0; i < size1; i++)
		(*array1)[i].write(data);

	if (action == 0)
	{
		size2.write(data);
		for (int i = 0; i < size2; i++)
			(*array2)[i].write(data);
	}

	finishSendMacro;
}
void message::play::send::playerAbilities(Player* p, bool invulnerable, bool flying, bool allowFlying, bool creative, bfloat flyingSpeed, bfloat fovModifier)
{
	varInt id = (int)id::playerAbilities_clientbound;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	*(data++) = char(Byte(invulnerable) | (Byte(flying) << 1) | (Byte(allowFlying) << 2) | (Byte(creative) << 3));
	flyingSpeed.write(data);
	fovModifier.write(data);

	finishSendMacro;
}
void message::play::send::endCombatEvent(Player* p, varInt duration, bint eid)
{
	varInt id = (int)id::endCombatEvent;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	duration.write(data);
	eid.write(data);

	finishSendMacro;
}
void message::play::send::enterCombatEvent(Player* p)
{
	varInt id = (int)id::enterCombatEvent;
	prepareSendMacro(1024 * 1024);

	id.write(data);

	finishSendMacro;
}
void message::play::send::deathCombatEvent(Player* p, varInt playerId, bint eid, const Chat& message)
{
	varInt id = (int)id::deathCombatEvent;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	playerId.write(data);
	eid.write(data);
	message.write(data);

	finishSendMacro;
}
void message::play::send::timeUpdate(Player* p, blong worldAge, blong timeOfDay)
{
	varInt id = (int)id::timeUpdate;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	worldAge.write(data);
	timeOfDay.write(data);

	finishSendMacro;
}
void message::play::send::setTitleText(Player* p, const Chat& title)
{
	varInt id = (int)id::setTitleText;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	title.write(data);

	finishSendMacro;
}
void message::play::send::setTitleTimes(Player* p, bint fadeIn, bint stay, bint fadeOut)
{
	varInt id = (int)id::setTitleTimes;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	fadeIn.write(data);
	stay.write(data);
	fadeOut.write(data);

	finishSendMacro;
}
void message::play::send::entitySoundEffect(Player* p, varInt soundId, varInt category, varInt eid, bfloat volume, bfloat pitch)
{
	varInt id = (int)id::entitySoundEffect;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	soundId.write(data);
	category.write(data);
	eid.write(data);
	volume.write(data);
	pitch.write(data);

	finishSendMacro;
}
void message::play::send::soundEffect(Player* p, varInt soundId, varInt category, bint effectPosX, bint effectPosY, bint effectPosZ, bfloat volume, bfloat pitch)
{
	varInt id = (int)id::soundEffect;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	soundId.write(data);
	category.write(data);
	effectPosX.write(data);
	effectPosY.write(data);
	effectPosZ.write(data);
	volume.write(data);
	pitch.write(data);

	finishSendMacro;
}
void message::play::send::stopSound(Player* p, Byte flags, varInt source, const mcString& name)
{
	varInt id = (int)id::stopSound;
	prepareSendMacro(1024 * 1024);

	id.write(data);

	finishSendMacro;
}
void message::play::send::playerListHeaderAndFooter(Player* p, const Chat& header, const Chat& footer)
{
	varInt id = (int)id::playerListHeaderAndFooter;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	header.write(data);
	footer.write(data);

	finishSendMacro;
}
void message::play::send::nbtQueryResponse(Player* p, varInt transactionId, const nbt& tag)
{
	varInt id = (int)id::nbtQueryResponse;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	transactionId.write(data);
	tag.write(data);

	finishSendMacro;
}
void message::play::send::collectItem(Player* p, varInt collectedEid, varInt collectorEid, varInt pickupCount)
{
	varInt id = (int)id::collectItem;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	collectedEid.write(data);
	collectorEid.write(data);
	pickupCount.write(data);

	finishSendMacro;
}
void message::play::send::entityTeleport(Player* p, varInt eid, bdouble x, bdouble y, bdouble z, Angle yaw, Angle pitch, bool onGround)
{
	varInt id = (int)id::entityTeleport;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);
	x.write(data);
	y.write(data);
	z.write(data);
	*(data++) = (char&)yaw;
	*(data++) = (char&)pitch;
	*(data++) = onGround;

	finishSendMacro;
}
void message::play::send::advancements(Player* p, bool reset, varInt mappingSize, mcString* advancementsIdentifiers, advancement** advancements, varInt listSize, mcString* removedIdentifiers, varInt progressSize, mcString* progressIdentifiers, advancementProgress** advancementProgresses)
{ //untested!!!
	varInt id = (int)id::advancements;
	prepareSendMacro(1024 * 1024);

	*(data++) = reset;
	mappingSize.write(data);
	for (int i = 0; i < mappingSize; i++)
	{
		advancementsIdentifiers[i].write(data);
		advancements[i]->write(data);
	}

	listSize.write(data);
	for (int i = 0; i < listSize; i++)
		removedIdentifiers[i].write(data);

	progressSize.write(data);
	for (int i = 0; i < progressSize; i++)
	{
		progressIdentifiers[i].write(data);
		advancementProgresses[i]->write(data);
	}

	finishSendMacro;
}
void message::play::send::pluginMessage(Player* p, const mcString& channel, ull ByteCount, const char* Bytes)
{
	varInt id = (int)id::pluginMessage_clientbound;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	channel.write(data);
	for (ull i = 0; i < ByteCount; i++)
		*(data++) = Bytes[i];

	finishSendMacro;
}
void message::play::send::namedSoundEffect(Player* p, const mcString& soundName, varInt category, bint effectX, bint effectY, bint effectZ, bfloat volume, bfloat pitch)
{
	varInt id = (int)id::namedSoundEffect;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	soundName.write(data);
	category.write(data);
	effectX.write(data);
	effectY.write(data);
	effectZ.write(data);
	volume.write(data);
	pitch.write(data);

	finishSendMacro;
}
void message::play::send::heldItemChange(Player* p, Byte slot)
{
	varInt id = (int)id::heldItemChange_clientbound;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	*(data++) = slot;

	finishSendMacro;
}
void message::play::send::serverDifficulty(Player* p, Byte difficulty, bool isLocked)
{
	varInt id = (int)id::serverDifficulty;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	*(data++) = difficulty;
	*(data++) = isLocked;

	finishSendMacro;
}
void message::play::send::spawnPosition(Player* p, const Position& location, bfloat angle)
{
	varInt id = (int)id::spawnPosition;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	location.write(data);
	angle.write(data);

	finishSendMacro;
}
void message::play::send::displayScoreboard(Player* p, Byte position, const mcString& scoreName)
{
	varInt id = (int)id::displayScoreboard;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	*(data++) = position;
	scoreName.write(data);

	finishSendMacro;
}
void message::play::send::entityMetadata(Player* p, varInt eid, const std::vector<Entity::Metadata>& metadatas)
{
	varInt id = (int)id::entityMetadata;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);

	for (const Entity::Metadata& m : metadatas)
		m.write(data);

	*(data++) = (Byte)0xff;

	finishSendMacro;
}
void message::play::send::attachEntity(Player* p, bint attachedEid, bint holdingEid)
{
	varInt id = (int)id::attachEntity;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	attachedEid.write(data);
	holdingEid.write(data);

	finishSendMacro;
}
void message::play::send::entityVelocity(Player* p, varInt eid, bshort velocityX, bshort velocityY, bshort velocityZ)
{
	varInt id = (int)id::entityVelocity;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);
	velocityX.write(data);
	velocityY.write(data);
	velocityZ.write(data);

	finishSendMacro;
}
void message::play::send::entityEquipment(Player* p, varInt eid, Equipment* equipment)
{
	varInt id = (int)id::entityEquipment;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);

	equipment->unSet();
	equipment->write(data);

	finishSendMacro;
}
void message::play::send::entityEquipment(Player* p, varInt eid, Equipment** equipments)
{
	varInt id = (int)id::entityEquipment;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);

	equipments[0]->write(data);

	//				if the top bit is set, another entry follows
	for (int i = 0; (equipments[i]->getSlot() & 0x80) == 1; i++)
		equipments[i + 1]->write(data);

	finishSendMacro;
}
void message::play::send::entityEquipment(Player* p, varInt eid, const std::vector<Equipment*>& equipments)
{
	varInt id = (int)id::entityEquipment;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);

	for (int i = 0; i < equipments.size() - 1; i++)
		equipments[i]->write(data);

	equipments.back()->unSet();
	equipments.back()->write(data);

	finishSendMacro;
}
void message::play::send::declareRecipes(Player* p, varInt nOfRecipes, std::vector<recipe::Recipe*>* recipes)
{
	varInt id = (int)id::declareRecipes;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	nOfRecipes.write(data);
	for (int i = 0; i < nOfRecipes; i++)
		(*recipes)[i]->write(data);

	finishSendMacro;
}
void message::play::send::updateViewPosition(Player* p, varInt chunkX, varInt chunkZ)
{
	varInt id = (int)id::updateViewPosition;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	chunkX.write(data);
	chunkZ.write(data);

	finishSendMacro;
}
void message::play::send::updateViewDistance(Player* p, varInt distance)
{
	varInt id = (int)id::updateViewDistance;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	distance.write(data);

	finishSendMacro;
}
void message::play::send::updateLight(Player* p, varInt cX, varInt cZ, bool trustEdges, const BitArray& skyLightMask, const BitArray& blockLightMask, const BitArray& emptySkyLightMask, const BitArray& emptyBlockLightMask,
	varInt skyLightArrayCount, BitArray** skyLightArrays, varInt blockLightArrayCount, BitArray** blockLightArrays)
{
	varInt id = (int)id::updateLight;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	cX.write(data);
	cZ.write(data);
	*(data++) = trustEdges;

	varInt((uint)skyLightMask.getCompactedSize()).write(data);
	skyLightMask.write(data);
	varInt((uint)blockLightMask.getCompactedSize()).write(data);
	blockLightMask.write(data);
	varInt((uint)emptySkyLightMask.getCompactedSize()).write(data);
	emptySkyLightMask.write(data);
	varInt((uint)emptyBlockLightMask.getCompactedSize()).write(data);
	emptyBlockLightMask.write(data);

	skyLightArrayCount.write(data);
	for (int i = 0; i < skyLightArrayCount; i++)
	{
		LightSection::lightArrayLength.write(data);
		skyLightArrays[i]->writeAsLight(data);
	}

	blockLightArrayCount.write(data);
	for (int i = 0; i < blockLightArrayCount; i++)
	{
		LightSection::lightArrayLength.write(data);
		blockLightArrays[i]->writeAsLight(data);
	}

	finishSendMacro;
}
void message::play::send::updateLight(Player* p, varInt cX, varInt cZ)
{
	Chunk* chunk = p->world->getChunk(cX, cZ);
	uint sectionCount = (uint)chunk->lightData.size();

	std::vector<BitArray*> skyLightArrays, blockLightArrays;
	for (uint i = 0; i < sectionCount; i++)
	{
		if (chunk->skyLightMask->getElement(i)) skyLightArrays.emplace_back(chunk->lightData[i].skyLight);
		if (chunk->blockLightMask->getElement(i)) blockLightArrays.emplace_back(chunk->lightData[i].blockLight);
	}
	updateLight(p, cX, cZ, true, *chunk->skyLightMask, *chunk->blockLightMask, *chunk->emptySkyLightMask, *chunk->emptyBlockLightMask, (uint)skyLightArrays.size(), skyLightArrays.data(), (uint)blockLightArrays.size(), blockLightArrays.data());
}
void message::play::send::disconnect(Player* p, const Chat& reason)
{
	varInt id = (int)id::disconnect;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	reason.write(data);

	finishSendAndDisconnect;
}
void message::play::send::entityStatus(Player* p, bint eid, Byte status)
{
	varInt id = (int)id::entityStatus;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);
	*(data++) = status;

	finishSendAndDisconnect;
}
void message::play::send::explosion(Player* p, bfloat x, bfloat y, bfloat z, bfloat strength, varInt recordCount, Byte* records, bfloat playerMotionX, bfloat playerMotionY, bfloat playerMotionZ)
{
	varInt id = (int)id::explosion;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	x.write(data);
	y.write(data);
	z.write(data);
	strength.write(data);
	recordCount.write(data);
	for (int i = 0; i < recordCount; i++)
		*(data++) = records[i];
	playerMotionX.write(data);
	playerMotionY.write(data);
	playerMotionZ.write(data);

	finishSendAndDisconnect;
}
void message::play::send::chatMessage(Player* p, const Chat& msg, Byte position, const mcUUID& sender)
{
	varInt id = (int)id::chatMessage_clientbound;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	msg.write(data);
	*(data++) = position;
	sender.write(data);

	finishSendMacro;
}
void message::play::send::clearTitles(Player* p, bool reset)
{
	varInt id = (int)id::clearTitles;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	*(data++) = reset;

	finishSendMacro;
}
void message::play::send::changeGameState(Player* p, Byte reason, bfloat value)
{
	varInt id = (int)id::changeGameState;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	*(data++) = reason;
	value.write(data);

	finishSendMacro;
}
void message::play::send::openHorseWindow(Player* p, Byte winId, varInt slotCount, bint eid)
{
	varInt id = (int)id::openHorseWindow;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	*(data++) = winId;
	slotCount.write(data);
	eid.write(data);	//dont open gui if the entity is not a horse

	finishSendMacro;
}
void message::play::send::initializeWorldBorder(Player* p, bdouble x, bdouble z, bdouble oldDiameter, bdouble newDiameter, varLong speed, varInt portalTeleportBoundary, varInt warningBlocks, varInt warningTime)
{
	varInt id = (int)id::initializeWorldBorder;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	x.write(data);
	z.write(data);
	oldDiameter.write(data);
	newDiameter.write(data);
	speed.write(data);
	portalTeleportBoundary.write(data);
	warningBlocks.write(data);
	warningTime.write(data);

	finishSendMacro;
}
void message::play::send::unloadChunk(Player* p, bint x, bint z)
{
	varInt id = (int)id::unloadChunk;
	prepareSendMacro(1024);

	id.write(data);
	x.write(data);
	z.write(data);

	p->world->unload(x, z);

	finishSendMacro;
}
void message::play::send::tags(Player* p)
{
	tags(p, (int)TagGroup::defaultTags.size(), TagGroup::defaultTags.data());
}
void message::play::send::tags(Player* p, varInt tagCategoryCount, TagGroup* tags)
{
	varInt id = (int)id::tags;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	tagCategoryCount.write(data);
	for (int i = 0; i < tagCategoryCount; i++)
	{
		tags[i].tagType.write(data);
		int count = (int)tags[i].tags.size();
		varInt(count).write(data);
		//tags[i].tagCount.write(data);
		for (int j = 0; j < count; j++)
		{
			TagGroup::Tag& tag = tags[i].tags[j];
			tag.name.write(data);
			int entryCount = (int)tag.entries.size();
			varInt(entryCount).write(data);
			for (int k = 0; k < entryCount; k++) tag.entries[k].write(data);
		}
	}

	finishSendMacro;
}
void message::play::send::declareCommands(Player* p)
{
	declareCommands(p, Commands::Commands::commands, Commands::Commands::root);
}
void message::play::send::declareCommands(Player* p, const std::vector<Commands::Node*>& commandGraphNodes, const Commands::RootNode& rootNode)
{
	varInt id = (int)id::declareCommands;
	prepareSendMacro(1024 * 1024);

	varInt graphSize = (int)commandGraphNodes.size();
	id.write(data);

	varInt(graphSize + 1).write(data);
	for (const Commands::Node* node : commandGraphNodes) node->write(data);
	rootNode.write(data);
	graphSize.write(data);

	/*count.write(data);
	for (int i = 0; i < count; i++)
	{
		Node& node = nodes[i];
		*(data++) = node.flags();
		node.childrenCount.write(data);
		for (int j = 0; j < node.childrenCount; j++) node.children[j].write(data);
		if (node.hasRedirect) node.redirectNode.write(data);
		if (node.type == Node::argument || node.type == Node::literal) node.name->write(data);
		if (node.type == Node::argument)
		{
			node.parser->write(data);
			if (*node.parser == "brigadier:double")
			{
				Node::Properties::DoubleProperties& prop = node.properties.doubleProperties;
				*(data++) = prop.flags;
				if (prop.flags & prop.hasMin) prop.min.write(data);
				if (prop.flags & prop.hasMax) prop.max.write(data);
			}
			else if (*node.parser == "brigadier:float")
			{
				Node::Properties::FloatProperties& prop = node.properties.floatProperties;
				*(data++) = prop.flags;
				if (prop.flags & prop.hasMin) prop.min.write(data);
				if (prop.flags & prop.hasMax) prop.max.write(data);
			}
			else if (*node.parser == "brigadier:integer")
			{
				Node::Properties::IntegerProperties& prop = node.properties.integerProperties;
				*(data++) = prop.flags;
				if (prop.flags & prop.hasMin) prop.min.write(data);
				if (prop.flags & prop.hasMax) prop.max.write(data);
			}
			else if (*node.parser == "brigadier:long")
			{
				Node::Properties::LongProperties& prop = node.properties.longProperties;
				*(data++) = prop.flags;
				if (prop.flags & prop.hasMin) prop.min.write(data);
				if (prop.flags & prop.hasMax) prop.max.write(data);
			}
			else if (*node.parser == "brigadier:string")
			{
				Node::Properties::StringProperties& prop = node.properties.stringProperties;
				prop.flag.write(data);
			}
			else if (*node.parser == "brigadier:entity")
			{
				Node::Properties::EntityProperties& prop = node.properties.entityProperties;
				*(data++) = prop.flags;
			}
			else if (*node.parser == "brigadier:score_holder")
			{
				Node::Properties::ScoreHolderProperties& prop = node.properties.scoreHolderProperties;
				*(data++) = prop.flags;
			}
			else if (*node.parser == "brigadier:range")
			{
				Node::Properties::RangeProperties& prop = node.properties.rangeProperties;
				*(data++) = prop.decimals;
			}
		}
		if (node.hasSuggestionsType) node.suggestionsType->write(data);
	}

	root.write(data);*/

	finishSendMacro;
}
void message::play::send::closeWindow(Player* p, Byte winId)
{
	varInt id = (int)id::closeWindow; //to do: send this packet when "a window is forcibly closed, such as when a chest is destroyed while it's open."
	prepareSendMacro(1024 * 1024);

	id.write(data);
	*(data++) = winId;

	finishSendMacro;
}
void message::play::send::windowItems(Player* p, Byte winId, varInt stateId, varInt count, Slot** slots, Slot* carried)
{
	varInt id = (int)id::windowItems;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	*(data++) = winId;
	stateId.write(data);
	count.write(data);
	for (int i = 0; i < count; i++)
		slots[i]->write(data);
	carried->write(data);

	finishSendMacro;
}
void message::play::send::windowProperty(Player* p, Byte winId, bshort property, bshort value)
{
	varInt id = (int)id::windowProperty;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	*(data++) = winId;
	property.write(data);
	value.write(data);

	finishSendMacro;
}
void message::play::send::setSlot(Player* p, Byte winId, varInt stateId, bshort slot, Slot* slotData)
{
	varInt id = (int)id::setSlot;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	*(data++) = winId;
	stateId.write(data);
	slot.write(data);
	slotData->write(data);

	finishSendMacro;
}
void message::play::send::setCooldown(Player* p, varInt itemId, varInt cooldown)
{
	varInt id = (int)id::setCooldown;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	itemId.write(data);
	cooldown.write(data);

	finishSendMacro;
}
void message::play::send::respawn(Player* p, const nbt_compound& dimension, const mcString& worldName, blong hashedSeed, gamemode gm, gamemode prev_gm, bool isDebug, bool isFlat, bool copyMetadata)
{
	varInt id = (int)id::respawn;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	dimension.write(data);
	worldName.write(data);
	hashedSeed.write(data);
	*(data++) = (Byte)gm;
	*(data++) = (Byte)prev_gm;
	*(data++) = isDebug;
	*(data++) = isFlat;
	*(data++) = copyMetadata;

	finishSendMacro;
}
void message::play::send::entityProperties(Player* p, varInt eid, varInt nOfProperties, Entity::Property* properties)
{
	varInt id = (int)id::entityProperties;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);
	nOfProperties.write(data);
	for (int i = 0; i < nOfProperties; i++)
		properties[i].write(data);

	finishSendMacro;
}
void message::play::send::entityEffect(Player* p, varInt entityId, varInt effectId, char amplifier, varInt duration, Byte flags)
{
	varInt id = (int)id::entityEffect;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	entityId.write(data);
	effectId.write(data);
	*(data++) = amplifier;
	duration.write(data);
	*(data++) = flags;

	finishSendMacro;
}
void message::play::send::setXp(Player* p, bfloat xpBar, varInt level, varInt totalXp)
{
	varInt id = (int)id::updateHp;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	xpBar.write(data);
	level.write(data);
	totalXp.write(data);

	finishSendMacro;
}
void message::play::send::updateHp(Player* p, bfloat hp, varInt food, bfloat saturation)
{
	varInt id = (int)id::updateHp;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	hp.write(data);
	food.write(data);
	saturation.write(data);

	finishSendMacro;
}
void message::play::send::scoreboardObjective(Player* p, const mcString& name, Byte mode, const Chat& value, varInt type)
{
	varInt id = (int)id::scoreboardObjective;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	name.write(data);
	*(data++) = mode;

	if (mode != 1)
	{
		value.write(data);
		type.write(data);
	}

	finishSendMacro;
}
void message::play::send::setPassengers(Player* p, varInt vehicleEid, varInt count, varInt* passengers)
{
	varInt id = (int)id::setPassengers;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	vehicleEid.write(data);
	count.write(data);
	for (int i = 0; i < count; i++)
		passengers[i].write(data);

	finishSendMacro;
}
void message::play::send::teams(Player* p, const mcString& name, Byte mode, teamsUpdate::mode* teamUpdateMode)
{
	varInt id = (int)id::teams;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	name.write(data);
	*(data++) = mode;
	teamUpdateMode->write(data);

	finishSendMacro;
}
void message::play::send::updateScore(Player* p, const mcString& name, Byte action, const mcString& objective, varInt value)
{
	varInt id = (int)id::updateScore;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	name.write(data);
	*(data++) = action;
	objective.write(data);

	if (action != 1)
		value.write(data);

	finishSendMacro;
}
void message::play::send::updateSimulationDistance(Player* p, varInt value)
{
	varInt id = (int)id::updateSimulationDistance;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	value.write(data);

	finishSendMacro;
}
void message::play::send::setTitleSubtitle(Player* p, const Chat& subtitle)
{
	varInt id = (int)id::setTitleSubtitle;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	subtitle.write(data);

	finishSendMacro;
}
void message::play::send::entityHeadLook(Player* p, varInt eid, Angle headYaw)
{
	varInt id = (int)id::entityHeadLook;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);
	*(data++) = (char&)headYaw;

	finishSendMacro;
}
void message::play::send::multiBlockChange(Player* p, blong sectionPosition, bool unknown, varInt arraySize, varLong* blocks)
{
	varInt id = (int)id::multiBLockChange;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	sectionPosition.write(data);
	*(data++) = unknown;
	arraySize.write(data);
	for (int i = 0; i < arraySize; i++)
		blocks[i].write(data);

	finishSendMacro;
}
void message::play::send::selectAdvancementTab(Player* p, bool hasId, const mcString& identifier)
{
	varInt id = (int)id::selectAdvancementTab;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	*(data++) = hasId;
	if (hasId)
		identifier.write(data);

	finishSendMacro;
}
void message::play::send::actionBar(Player* p, const Chat& actionBarText)
{
	varInt id = (int)id::actionBar;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	actionBarText.write(data);

	finishSendMacro;
}
void message::play::send::worldBorderCenter(Player* p, bdouble x, bdouble z)
{
	varInt id = (int)id::worldBorderCenter;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	x.write(data);
	z.write(data);

	finishSendMacro;
}
void message::play::send::worldBorderLerpSize(Player* p, bdouble oldDiameter, bdouble newDiameter, varLong speed)
{
	varInt id = (int)id::worldBorderLerpSize;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	oldDiameter.write(data);
	newDiameter.write(data);
	speed.write(data);

	finishSendMacro;
}
void message::play::send::worldBorderSize(Player* p, bdouble diameter)
{
	varInt id = (int)id::worldBorderSize;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	diameter.write(data);

	finishSendMacro;
}
void message::play::send::worldBorderWarningDelay(Player* p, varInt warningTime)
{
	varInt id = (int)id::worldBorderWarningDelay;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	warningTime.write(data);

	finishSendMacro;
}
void message::play::send::worldBorderWarningReach(Player* p, varInt warningBlocks)
{
	varInt id = (int)id::worldBorderWarningReach;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	warningBlocks.write(data);

	finishSendMacro;
}
void message::play::send::camera(Player* p, varInt camId)
{
	varInt id = (int)id::camera;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	camId.write(data);

	finishSendMacro;
}
void message::play::send::destroyEntity(Player* p, varInt eid) {
	varInt id = (int)id::destroyEntities;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	varInt(1).write(data);
	eid.write(data);

	finishSendMacro;
}
void message::play::send::destroyEntities(Player* p, varInt count, varInt* eids)
{
	varInt id = (int)id::destroyEntities;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	count.write(data);
	for (int i = 0; i < count; i++)
		eids[i].write(data);

	finishSendMacro;
}
void message::play::send::removeEntityEffect(Player* p, varInt eid, varInt effectId)
{
	varInt id = (int)id::removeEntityEffect;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);
	effectId.write(data);

	finishSendMacro;
}
void message::play::send::resourcePackSend(Player* p, const mcString& url, const mcString& hash, bool forced, bool hasPromptMessage, const Chat& promptMessage = "")
{
	varInt id = (int)id::resourcePackSend;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	url.write(data);
	hash.write(data);
	*(data++) = forced;
	*(data++) = hasPromptMessage;
	if (hasPromptMessage)
		promptMessage.write(data);

	finishSendMacro;
}
void message::play::send::entityPosition(Player* p, varInt eid, bshort deltaX, bshort deltaY, bshort deltaZ, bool onGround)
{
	varInt id = (int)id::entityPosition;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);
	deltaX.write(data);
	deltaY.write(data);
	deltaZ.write(data);
	*(data++) = onGround;

	finishSendMacro;
}
void message::play::send::entityRotation(Player* p, varInt eid, Angle yaw, Angle pitch, bool onGround)
{
	varInt id = (int)id::entityRotation;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);
	*(data++) = (char&)yaw;
	*(data++) = (char&)pitch;
	*(data++) = onGround;

	finishSendMacro;
}
void message::play::send::vehicleMove(Player* p, bdouble x, bdouble y, bdouble z, bfloat yaw, bfloat pitch)
{
	varInt id = (int)id::vehicleMove;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	x.write(data);
	y.write(data);
	z.write(data);
	yaw.write(data);
	pitch.write(data);

	finishSendMacro;
}
void message::play::send::openBook(Player* p, Hand whichHand)
{
	varInt id = (int)id::openBook;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	varInt((int)whichHand).write(data);

	finishSendMacro;
}
void message::play::send::openWindow(Player* p, varInt winId, window::type winType, const Chat& winTitle)
{
	varInt id = (int)id::openWindow;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	winId.write(data);
	varInt(static_cast<int>(winType)).write(data);
	winTitle.write(data);

	finishSendMacro;
}
void message::play::send::openSignEditor(Player* p, const Position& location)
{
	varInt id = (int)id::openSignEditor;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	location.write(data);

	finishSendMacro;
}
void message::play::send::ping(Player* p, bint pingId)
{
	varInt id = (int)id::ping;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	pingId.write(data);

	finishSendMacro;
}
void message::play::send::craftRecipeResponse(Player* p, Byte winId, const mcString& recipeIdentifier)
{
	varInt id = (int)id::craftRecipeResponse;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	*(data++) = winId;
	recipeIdentifier.write(data);

	finishSendMacro;
}
void message::play::send::entityPositionAndRotation(Player* p, varInt eid, bshort deltaX, bshort deltaY, bshort deltaZ, Angle yaw, Angle pitch, bool onGround)
{
	varInt id = (int)id::entityPositionAndRotation;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);
	deltaX.write(data);
	deltaY.write(data);
	deltaZ.write(data);
	*(data++) = (char&)yaw;
	*(data++) = (char&)pitch;
	*(data++) = onGround;

	finishSendMacro;
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
	p->viewDistance = (p->viewDistance > (int)viewDistance) ? (int)viewDistance : p->viewDistance;
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
void message::play::receive::clickWindow(Player* p, Byte windowID, varInt stateID, bshort clickedSlot, Byte button, varInt mode, varInt length, bshort* slotNumbers, Slot** slots, Slot* clickedItem)
{
	Log::debug(PROTOCOL_WARNINGS) << "Partially handled packet: clickWindow" << Log::endl;

	Log::info() << "WindowID: " << (int)windowID << " StateID: "<<stateID<< Log::endl;
	Log::info() << "Mode: " << mode << "\tButton: " << (int)button << "\tClickedSlot : " << (int)clickedSlot << " Length: " << length << Log::endl;
	for (int i = 0; i < length; i++)
	{
		Log::info() << "\t"<< i << " sn: " << slotNumbers[i] << Log::endl;
	}
	Log::info() << Log::endl;

	int inventoryFirstSlotIndex = (windowID == 0) ? 9 : window::getWindowSlotCount(p->inventory->getLatestWindow(windowID));
	Log::info() << "iFSI: " << inventoryFirstSlotIndex << Log::endl;

	switch (mode)
	{
	case 0:
		if (button == 0)
		{
			if (clickedSlot == -999)
			{ //drop whole slot
				Entity::item* theItem = new Entity::item(Entity::entity(p->world->getEidDispenser(), Entity::type::minecraft_item, p->x, p->y + 1.25, p->z, 0.7, 0.6), *p->inventory->getFloatingSlot());
				p->inventory->setFloatingSlot(Slot());

				p->world->addEntity(theItem);

				for (Player* seener : p->seenBy)
				{
					message::play::send::spawnEntity(seener, theItem, 100, 0, 100);
					message::play::send::entityMetadata(seener, theItem->getEid(), { Entity::Metadata(8, Entity::Metadata::type::_Slot, &theItem->theItem) });
				}
				message::play::send::spawnEntity(p, theItem, 100, 0, 100);
				message::play::send::entityMetadata(p, theItem->getEid(), { Entity::Metadata(8, Entity::Metadata::type::_Slot, &theItem->theItem) });
			}
			else
			{ //select or place slot
				if (clickedSlot == -1)
					return;

				Slot* clicked = p->inventory->getSlotByIndex(clickedSlot);
				Slot* floating = p->inventory->getFloatingSlot();

				if (clicked->getItemId() == floating->getItemId())
				{
					unsigned remained = p->inventory->addToSlot(*floating, clickedSlot);
					
					if (remained != floating->count)
						floating->count = floating->count - remained;
					else
						p->inventory->setFloatingSlot(Slot());
				}
				else
					p->inventory->swapWithFloating(clickedSlot);
			}
		}
		else
		{	//button == 1
			if (clickedSlot == -999)
			{	//drop one item from slot
				Slot* floatingSlot = p->inventory->getFloatingSlot();
				floatingSlot->count--; //decrease the floating item count;

				Slot dropSlot = *floatingSlot;
				dropSlot.count = 1; //copy the floating item, but with 1 count

				Entity::item* theItem = new Entity::item(Entity::entity(p->world->getEidDispenser(), Entity::type::minecraft_item, p->x, p->y + 1.25, p->z, 0.7, 0.6), dropSlot);

				p->world->addEntity(theItem);

				for (Player* seener : p->seenBy)
				{
					message::play::send::spawnEntity(seener, theItem, 100, 0, 100);
					message::play::send::entityMetadata(seener, theItem->getEid(), { Entity::Metadata(8, Entity::Metadata::type::_Slot, &theItem->theItem) });
				}
				message::play::send::spawnEntity(p, theItem, 100, 0, 100);
				message::play::send::entityMetadata(p, theItem->getEid(), { Entity::Metadata(8, Entity::Metadata::type::_Slot, &theItem->theItem) });
			}
			else
			{	//select half from slot
				if (clickedSlot == -1)
					return;

				Slot* selectedSlot = p->inventory->getSlotByIndex(clickedSlot);
				Slot newFloatingSlot = *selectedSlot;
				
				newFloatingSlot.count /= 2;
				if (selectedSlot->count % 2 == 1)
					newFloatingSlot.count++;

				p->inventory->setFloatingSlot(newFloatingSlot);
				selectedSlot->count = selectedSlot->count / 2;
			}
		}
		break;
	case 1:
		//button 0 and 1 are doing identical behaviors
		//move whole slot to appropiate slot

		break;
	case 2:
		if (button == 40)
		{	//offhand swap
			Slot* selectedSlot = p->inventory->getSlotByIndex(clickedSlot);
			Slot* floatingSlot = p->inventory->getFloatingSlot();

			std::swap(selectedSlot, floatingSlot);

			message::play::send::setSlot(p, 0, 0, 45, floatingSlot);
			message::play::send::setSlot(p, 0, 0, clickedSlot, selectedSlot);
		}
		else
		{	//button is hotbar index from 0
			Slot* selectedSlot = p->inventory->getSlotByIndex(clickedSlot);
			Slot* hotbarSlot = p->inventory->getHotbarSlot(button);

			std::swap(selectedSlot, hotbarSlot);

			message::play::send::setSlot(p, 0, 0, 36 + button, hotbarSlot);
			message::play::send::setSlot(p, 0, 0, clickedSlot, selectedSlot);
		}
		break;
	case 3:
		//button is always 2
		//middle click, for creative players in non-player inventories

		break;
	case 4:
		if (clickedSlot == -999 || clickedSlot == -1)
			return;

		if (button == 0)
		{ //q, drop one from whole slot
			Slot* selectedSlot = p->inventory->getSlotByIndex(clickedSlot);
			selectedSlot->count -= 1; //decrease the floating item count;

			Slot dropSlot = *selectedSlot;
			dropSlot.count = 1; //copy the floating item, but with 1 count

			Entity::item* theItem = new Entity::item(Entity::entity(p->world->getEidDispenser(), Entity::type::minecraft_item, p->x, p->y + 1.25, p->z, 0.7, 0.6), dropSlot);

			p->world->addEntity(theItem);

			for (Player* seener : p->seenBy)
			{
				message::play::send::spawnEntity(seener, theItem, 100, 0, 100);
				message::play::send::entityMetadata(seener, theItem->getEid(), { Entity::Metadata(8, Entity::Metadata::type::_Slot, &theItem->theItem) });
			}
			message::play::send::spawnEntity(p, theItem, 100, 0, 100);
			message::play::send::entityMetadata(p, theItem->getEid(), { Entity::Metadata(8, Entity::Metadata::type::_Slot, &theItem->theItem) });
		}
		else
		{ //ctrl + q, button == 1 drop whole slot
			Entity::item* theItem = new Entity::item(Entity::entity(p->world->getEidDispenser(), Entity::type::minecraft_item, p->x, p->y + 1.25, p->z, 0.7, 0.6), *p->inventory->getSlotByIndex(clickedSlot));
			p->inventory->setSlotByIndex(clickedSlot, Slot());

			p->world->addEntity(theItem);

			for (Player* seener : p->seenBy)
			{
				message::play::send::spawnEntity(seener, theItem, 100, 0, 100);
				message::play::send::entityMetadata(seener, theItem->getEid(), { Entity::Metadata(8, Entity::Metadata::type::_Slot, &theItem->theItem) });
			}
			message::play::send::spawnEntity(p, theItem, 100, 0, 100);
			message::play::send::entityMetadata(p, theItem->getEid(), { Entity::Metadata(8, Entity::Metadata::type::_Slot, &theItem->theItem) });
		}
		break;
	case 5:
		switch (button)
		{
		case 0: //start left mouse drag

			break;
		case 4: //start right mouse drag

			break;
		case 8: //start middle mouse drag (creative players in non-creative players inv) (bugged)

			break;
		case 1: //add slot left mouse drag

			break;
		case 5: //add slot right mouse drag

			break;
		case 9: //add slot middle mouse drag (creative players in non-creative players inv) (bugged)

			break;
		case 2: //end left mouse drag

			break;
		case 6: //end right mouse drag

			break;
		case 10: //end slot middle mouse drag (creative players in non-creative players inv) (bugged)

			break;
		}
		break;
	case 6:
		//double click

		break;
	}
}
void message::play::receive::closeWindow(Player* p, Byte winId)
{
	if(winId != 0)
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
	for (Entity::entity* groundItem : p->world->getEntitiesByType(Entity::type::minecraft_item)) {
		if (Position::inRange(groundItem->x, groundItem->y, groundItem->z, p->x, p->y, p->z, 2))
		{
			Entity::item* droppedItem = static_cast<Entity::item*>(groundItem);

			if (droppedItem->spawnedTimeStamp + 1500 > utility::time::timeSinceEpoch())
				continue;

			auto pickedDataArray = p->inventory->addToInventory(droppedItem->theItem);
			Byte totalPickedCount = 0;
			
			for(auto& pickedData : pickedDataArray) {
				Byte pickedCount = pickedData.first;
				Byte pickedIndex = pickedData.second;

				message::play::send::collectItem(p, groundItem->getEid(), p->getEid(), pickedCount);

				Slot* pickedupSlot = p->inventory->getSlotByIndex(pickedIndex);
				message::play::send::setSlot(p, (pickedIndex > 35 ? 0 : -2), 0, pickedIndex, pickedupSlot);

				totalPickedCount += pickedCount;
			}

			if (totalPickedCount == droppedItem->theItem.count) { //all of the entity got picked up, destroy it
				message::play::send::destroyEntity(p, droppedItem->getEid()); //seeners too
				p->world->removeEntity(droppedItem->getEid());
			}
			else if (totalPickedCount < droppedItem->theItem.count) { //not everything got picked up, update the data
				droppedItem->theItem.count = droppedItem->theItem.count - totalPickedCount;
				message::play::send::entityMetadata(p, droppedItem->getEid(), { Entity::Metadata(8, Entity::Metadata::type::_Slot, &droppedItem->theItem) });
			}
		}
	}
}
void message::play::receive::playerPosition(Player* p, bdouble X, bdouble feetY, bdouble Z, bool onGround)
{
	if (p->pendingTpId != -1)
		return;

	for (Player* seener : p->seenBy)
		ignoreExceptions(message::play::send::entityPosition(seener, p->getEid(), short((X - p->x) * 4096), short((feetY - p->y) * 4096), short((Z - p->z) * 4096), onGround));

	p->updatePosition(X, feetY, Z);
	p->onGround = onGround;

	processGroundItem(p);
}
void message::play::receive::playerPositionAndRotation(Player* p, bdouble X, bdouble Y, bdouble Z, bfloat yaw, bfloat pitch, bool onGround)
{
	if (p->pendingTpId != -1)
		return;

	for (Player* seener : p->seenBy)
	{
		ignoreExceptions(message::play::send::entityPositionAndRotation(seener, p->getEid(), short((X - p->x) * 4096), short((Y - p->y) * 4096), short((Z - p->z) * 4096), (float)yaw, (float)pitch, onGround));
		ignoreExceptions(message::play::send::entityHeadLook(seener, p->getEid(), (float)p->yaw));
	}
	p->updatePosition(X, Y, Z);
	p->updateRotation(yaw, pitch);
	p->onGround = onGround;

	processGroundItem(p);
}
void message::play::receive::playerRotation(Player* p, bfloat yaw, bfloat pitch, bool onGround)
{
	for (Player* seener : p->seenBy)
	{
		ignoreExceptions(message::play::send::entityRotation(seener, p->getEid(), (float)yaw, (float)pitch, onGround));
		ignoreExceptions(message::play::send::entityHeadLook(seener, p->getEid(), (float)p->yaw));
	}

	p->updateRotation(yaw, pitch);
	p->onGround = onGround;
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
	p->inventory->setSelectedSlot(slot);

	Equipment* eqp = new Equipment(0, p->inventory->getSelectedSlot());

	for (Player* seener : p->seenBy)
		message::play::send::entityEquipment(seener, p->getEid(), eqp);

	delete eqp;
}
void message::play::receive::creativeInventoryAction(Player* p, bshort slot, Slot* clickedItem)
{
	if (slot == -1)
	{ //throw away from inventory, create entity
		Entity::item* theItem = new Entity::item(Entity::entity(p->world->getEidDispenser(), Entity::type::minecraft_item, p->x, p->y + 1.25, p->z, 0.7, 0.6), *clickedItem);

		p->world->addEntity(theItem);

		for (Player* seener : p->seenBy)
		{
			message::play::send::spawnEntity(seener, theItem, 100, 0, 100);
			message::play::send::entityMetadata(seener, theItem->getEid(), { Entity::Metadata(8, Entity::Metadata::type::_Slot, &theItem->theItem) });
		}
		message::play::send::spawnEntity(p, theItem, 100, 0, 100);
		message::play::send::entityMetadata(p, theItem->getEid(), { Entity::Metadata(8, Entity::Metadata::type::_Slot, &theItem->theItem) });
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

		Entity::item* theItem = new Entity::item(Entity::entity(p->world->getEidDispenser(), Entity::type::minecraft_item, p->x, p->y + 1.25, p->z, 0.7, 0.6), *playerItem);
		p->world->addEntity(theItem);

		for (Player* seener : p->seenBy)
		{
			message::play::send::spawnEntity(seener, theItem, 100, 0, 100);
			message::play::send::entityMetadata(seener, theItem->getEid(), { Entity::Metadata(8, Entity::Metadata::type::_Slot, &theItem->theItem) });
		}
		message::play::send::spawnEntity(p, theItem, 100, 0, 100);
		message::play::send::entityMetadata(p, theItem->getEid(), { Entity::Metadata(8, Entity::Metadata::type::_Slot, &theItem->theItem) });

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

		Entity::item* theItem = new Entity::item(Entity::entity(p->world->getEidDispenser(), Entity::type::minecraft_item, p->x, p->y + 1.25, p->z, 0.7, 0.6), *playerItem);
		theItem->theItem.count = 1;
		
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
			message::play::send::entityMetadata(seener, theItem->getEid(), { Entity::Metadata(8, Entity::Metadata::type::_Slot, &theItem->theItem) });
		}
		message::play::send::spawnEntity(p, theItem, 100, 0, 100);
		message::play::send::entityMetadata(p, theItem->getEid(), { Entity::Metadata(8, Entity::Metadata::type::_Slot, &theItem->theItem) });
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
		message::play::send::entityMetadata(seener, eid, { Entity::Metadata(0, Entity::Metadata::type::_Byte, &p->attributes) });

		varInt entityPose = p->attributes == 0x02 ? Entity::pose::sneaking : Entity::pose::standing;
		message::play::send::entityMetadata(seener, eid, { Entity::Metadata(6, Entity::Metadata::type::_Pose, &entityPose) });
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
void message::play::receive::playerBlockPlacement(Player* p, Hand hand, const Position& location, playerDigging::face face, bfloat curX, bfloat curY, bfloat curZ, bool insideBlock)
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