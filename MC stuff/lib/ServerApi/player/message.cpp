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
	p->schedulePacket(start, size, data,true);

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

/*
{
	"version": {
		"name": "1.17.1",
		"protocol": 756
	},
	"players": {
		"max": 100,
		"online": 5
	},
	"description": {
		"text": "Hello world"
	}
}
*/
void message::status::receive::request(Player* p)
{
	message::status::send::respose(p, "{" + Options::version + ",\"players\":{\"max\":" + std::to_string(rand() % 20 + 20) + ",\"online\":" + std::to_string(rand() % 20) + ",\"sample\":[{\"name\":\"TheGoldenSnowman\",\"id\":\"4566e69f-c907-48ee-8d71-d7ba5aa00d20\"},{\"name\":\"TimmyBrott\",\"id\":\"4566e69f-c907-48ee-8d71-d7ba5aa00d21\"},{\"name\":\"NativeLog05\",\"id\":\"4566e69f-c907-48ee-8d71-d7ba5aa00d22\"},{\"name\":\"Tim\",\"id\":\"4566e69f-c907-48ee-8d71-d7ba5aa00d23\"}]},\"description\":" + Options::motd() + "}");
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
		login::send::disconnect(p, "{\"text\":\"Use 1.17.1, " + (std::string)username + ", you nitwit!\",\"color\":\"red\",\"bold\":\"true\"}");
		return;
	}

	//player initialization
	//general fields
	p->uuid = new mcUUID(mcUUID::player);
	p->username = username;
	p->viewDistance = Options::viewDistance();
	p->gm = gamemode::creative;
	p->eid = Player::eidDispenser.Alloc();
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

	login::send::setCompression(p, 128);

	login::send::success(p, *p->uuid, username);

	play::send::joinGame(p, (int)p->eid, false, gamemode::creative, gamemode::none, 0, nullptr, World::dimension_codec, World::worlds[World::spawnWorld]->characteristics, World::worlds[World::spawnWorld]->name, 0x5f19a34be6c9129a, 0, p->viewDistance, false, false, false, World::worlds[World::spawnWorld]->isFlat);

	play::send::pluginMessage(p, "minecraft:brand", 10, "\x9lazorenii");

	play::send::serverDifficulty(p, 2, false);

	play::send::playerAbilities(p, true, true, true, p->gm == gamemode::creative, 0.05f, 0.1f);

	play::send::declareRecipes(p, 0);

	std::vector<Player*> inGamePlayers;
	for (Player* player : Player::players) if (player->state == ConnectionState::play && player->Connected()) inGamePlayers.push_back(player);
	Player** playerInfoList = inGamePlayers.data();
	play::send::playerInfo(p, playerInfo::addPlayer, (int)inGamePlayers.size(), playerInfoList);

	play::send::tags(p);

	play::send::declareCommands(p);

	Player::broadcastChat(Chat((p->username + " joined the game").c_str(), Chat::color::yellow), p);
	broadcastMessageOmit(play::send::playerInfo(player_macro, playerInfo::addPlayer, 1, &p), p);

	p->setWorld(World::worlds[World::spawnWorld]);

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

void message::play::send::spawnEntity(Player* p, varInt eid, const mcUUID& uuid, EntityType type, bigEndian<double> x, bigEndian<double> y, bigEndian<double> z, Angle pitch, Angle yaw, bint Data, bshort velocityX, bshort velocityY, bshort velocityZ)
{
	varInt id = (int)id::spawnEntity;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);
	uuid.write(data);
	varInt(type).write(data);
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
void message::play::send::spawnXPorb(Player* p, varInt eid, bdouble x, bdouble y, bdouble z, bigEndian<short> xpCount)
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
void message::play::send::spawnLivingEntity(Player* p, varInt eid, const mcUUID& uuid, EntityType type, bdouble x, bdouble y, bdouble z, Angle yaw, Angle pitch, Angle headPitch, bshort velocityX, bshort velocityY, bshort velocityZ)
{
	varInt id = (int)id::spawnLivingEntity;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);
	uuid.write(data);
	varInt(type).write(data);
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
void message::play::send::spawnPainting(Player* p, varInt eid, const mcUUID& uuid, Painting::motive motive, Position location, Painting::direction direction)
{
	varInt id = (int)id::spawnPainting;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);
	uuid.write(data);
	varInt(motive).write(data);
	location.write(data);
	*(data++) = direction;

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
void message::play::send::sculkVibrationSignal(Player* p, Position source, Sculk::destinationType destinationType, Sculk::destination destination, varInt arrivalTime)
{
	varInt id = (int)id::sculkVibrationSignal;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	source.write(data);
	if (destinationType == Sculk::block)
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
void message::play::send::entityAnimation(Player* p, varInt eid, Animation animation)
{
	varInt id = (int)id::entityAnimation;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);
	*(data++) = (Byte&)animation;

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
	const nbt_compound& dimension, const mcString& worldName, int64 hashedSeedHigh, varInt maxPlayers, varInt viewDistance, bool reducedDebugInfo, bool respawnScreen, bool isDebug, bool isFlat)
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
	*(data++) = reducedDebugInfo;
	*(data++) = respawnScreen;
	*(data++) = isDebug;
	*(data++) = isFlat;

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
			player->uuid->write(data);
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
			player->uuid->write(data);
			varInt((Byte)player->gm).write(data);
		}
		break;
	case playerInfo::updateLatency:
		for (int i = 0; i < playerCount; i++)
		{
			Player*& player = players[i];
			player->uuid->write(data);
			player->ping.write(data);
		}
		break;
	case playerInfo::updateDisplayName:
		for (int i = 0; i < playerCount; i++)
		{
			Player*& player = players[i];
			player->uuid->write(data);
			*(data++) = player->hasDisplayName;
			if (p->hasDisplayName) p->displayName->write(data);
		}
		break;
	case playerInfo::removePlayer:
		for (int i = 0; i < playerCount; i++)
		{
			Player*& player = players[i];
			player->uuid->write(data);
		}
		break;
	}

	finishSendMacro;
}
/*void message::play::send::chunkData(Player* p, bint cX, bint cZ)
{
	varInt id = (int)id::chunkData;
	prepareSendMacro(1024*1024);

	id.write(data);
	cX.write(data);
	cZ.write(data);

	Chunk* chunk = p->world->get(cX, cZ);
	int worldHeight = World::worlds[0]->characteristics["height"].vInt();
	int sectionCount = worldHeight >> 4;

	//primary bitmask length
	varInt((int)chunk->sectionMask->getCompactedSize()).write(data);
	//primary bitmask
	chunk->sectionMask->write(data);
	//heightMaps
	nbt_compound nbt_heightmap("", new nbt * [1]{
		new nbt_long_array("MOTION_BLOCKING",*chunk->heightmaps)
		}, 1);
	nbt_heightmap.write(data);
	//biomes length
	varInt(uint(64 * chunk->sections.size())).write(data);
	//biomes
	for (int s = 0; s < chunk->sections.size(); s++)
		for (int y = 0; y < 4; y++)
			for (int z = 0; z < 4; z++)
				for (int x = 0; x < 4; x++)
					chunk->sections[s].biomes[x][y][z].write(data);

	//data preparation
	char* dataBuffer = new char[1024024], * dataStart = dataBuffer;
	for (int i = 0; i < sectionCount; i++)
	{
		Section& section = chunk->sections[i];
		if (section.blockCount)
		{
			section.blockCount.write(dataBuffer);
			*(dataBuffer++) = section.bitsPerBlock;
			if (!section.useGlobalPallete)
			{
				varInt((uint)section.pallete.size()).write(dataBuffer);
				for (varInt val : section.pallete) val.write(dataBuffer);
			}
			varInt((uint)section.blockStates->getCompactedSize()).write(dataBuffer);
			section.blockStates->write(dataBuffer);
		}
	}
	uint size = (uint)(dataBuffer - dataStart);
	//size
	varInt(size).write(data);
	//data
	for (uint i = 0; i < size; i++) *(data++) = dataStart[i];
	delete[] dataStart;

	//nOfBlockEntities
	chunk->nOfBlockEntities.write(data);
	//blockEntities

	finishSendMacro;
}*/
void message::play::send::chunkData(Player* p, bint cX, bint cZ, varInt bitMaskLength, blong* bitMask, const nbt_compound& heightMaps, varInt biomesLength, varInt* biomes,
	varInt dataSize, char* chunkData, varInt nOfBlockEntities, nbt_compound* blockEntities)
{
	varInt id = (int)id::chunkData;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	cX.write(data);
	cZ.write(data);
	bitMaskLength.write(data);
	for (int i = 0; i < bitMaskLength; i++) bitMask[i].write(data);
	heightMaps.write(data);
	biomesLength.write(data);
	for (int i = 0; i < biomesLength; i++) biomes[i].write(data);
	dataSize.write(data);
	for (int i = 0; i < dataSize; i++) *(data++) = chunkData[i];
	nOfBlockEntities.write(data);
	for (int i = 0; i < nOfBlockEntities; i++) blockEntities[i].write(data);

	finishSendMacro;
}
void message::play::send::particle(Player* p, bint particleId, bool longDistance, bdouble x, bdouble y, bdouble z, bfloat offsetX, bfloat offsetY, bfloat offsetZ, bfloat particleData, bint count, Particle* particle = nullptr) {
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
		particle->write(data);

	finishSendMacro;
}
void message::play::send::playerPosAndLook(Player* p, bigEndian<double> x, bigEndian<double> y, bigEndian<double> z, bigEndian<float> yaw, bigEndian<float> pitch, Byte flags, bool dismountVehicle)
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
void message::play::send::playerAbilities(Player* p, bool invulnerable, bool flying, bool allowFlying, bool creative, bigEndian<float> flyingSpeed, bigEndian<float> fovModifier)
{
	varInt id = (int)id::playerAbilities_clientbound;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	*(data++) = char(Byte(invulnerable) | (Byte(flying) << 1) | (Byte(allowFlying) << 2) | (Byte(creative) << 3));
	flyingSpeed.write(data);
	fovModifier.write(data);

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
void message::play::send::entityTeleport(Player* p, varInt eid, bdouble x, bdouble y, bdouble z, Angle yaw, Angle pitch, bool onGround) {
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
void message::play::send::pluginMessage(Player* p, const mcString& channel, ull ByteCount, const char* Bytes)
{
	varInt id = (int)id::pluginMessage_clientbound;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	channel.write(data);
	for (ull i = 0; i < ByteCount; i++) *(data++) = Bytes[i];

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
void message::play::send::spawnPosition(Player* p, Position location, bfloat angle)
{
	varInt id = (int)id::spawnPosition;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	location.write(data);
	angle.write(data);

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
void message::play::send::declareRecipes(Player* p, varInt nOfRecipes)
{
	varInt id = (int)id::declareRecipes;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	nOfRecipes.write(data);

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
/*void message::play::send::updateLight(Player* p, varInt cX, varInt cZ)
{
	Chunk* chunk = p->world->get(cX, cZ);
	int sectionCount = (int)chunk->lightData.size();

	varInt id = (int)id::updateLight;
	prepareSendMacro(1024*1024);

	id.write(data);
	cX.write(data);
	cZ.write(data);
	*(data++) = true;
	//sky light mask length
	varInt((int)chunk->skyLightMask->getCompactedSize()).write(data);
	//sky light mask
	chunk->skyLightMask->write(data);
	//block light mask length
	varInt((int)chunk->blockLightMask->getCompactedSize()).write(data);
	//block light mask
	chunk->blockLightMask->write(data);
	//empty sky light mask length
	varInt((int)chunk->emptySkyLightMask->getCompactedSize()).write(data);
	//empty sky light mask
	chunk->emptySkyLightMask->write(data);
	//empty block light mask length
	varInt((int)chunk->emptyBlockLightMask->getCompactedSize()).write(data);
	//empty block light mask
	chunk->emptyBlockLightMask->write(data);
	//sky light array count
	varInt c;
	for (int i = 0; i < sectionCount; i++) if (chunk->skyLightMask->getElement(i)) c++;
	c.write(data);
	//sky light arrays
	for (int i = 0; i < sectionCount; i++) if (chunk->skyLightMask->getElement(i))
	{
		LightSection::lightArrayLength.write(data);
		chunk->lightData[i].skyLight->write(data);
	}
	//block light array count
	c = 0;
	for (int i = 0; i < sectionCount; i++) if (chunk->blockLightMask->getElement(i)) c++;
	c.write(data);
	//block light arrays
	for (int i = 0; i < sectionCount; i++) if (chunk->blockLightMask->getElement(i))
	{
		LightSection::lightArrayLength.write(data);
		chunk->lightData[i].blockLight->write(data);
	}

	finishSendMacro;
}*/
void message::play::send::updateLight(Player* p, varInt cX, varInt cZ, bool trustEdges,
	varInt length1, blong* skyLightMask, varInt length2, blong* blockLightMask,
	varInt length3, blong* emptySkyLightMask, varInt length4, blong* emptyBlockLightMask,
	varInt skyLightArrayCount, char** skyLightArrays, varInt blockLightArrayCount, char** blockLightArrays)
{
	varInt id = (int)id::updateLight;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	cX.write(data);
	cZ.write(data);
	*(data++) = trustEdges;
	length1.write(data);
	for (int i = 0; i < length1; i++) skyLightMask[i].write(data);
	length2.write(data);
	for (int i = 0; i < length2; i++) blockLightMask[i].write(data);
	length3.write(data);
	for (int i = 0; i < length3; i++) emptySkyLightMask[i].write(data);
	length4.write(data);
	for (int i = 0; i < length4; i++) emptyBlockLightMask[i].write(data);

	skyLightArrayCount.write(data);
	for (int i = 0; i < skyLightArrayCount; i++)
	{
		varInt(2048).write(data);
		for (int j = 0; j < 2048; j++)
		{
			*(data++) = skyLightArrays[i][j];
		}
	}
	blockLightArrayCount.write(data);
	for (int i = 0; i < blockLightArrayCount; i++)
	{
		varInt(2048).write(data);
		for (int j = 0; j < 2048; j++)
		{
			*(data++) = blockLightArrays[i][j];
		}
	}

	finishSendMacro;
}
void message::play::send::disconnect(Player* p, const Chat& reason)
{
	varInt id = (int)id::disconnect;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	reason.write(data);

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
void message::play::send::clearTitles(Player* p, bool reset) {
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
void message::play::send::closeWindow(Player* p, Byte winId) {
	varInt id = (int)id::closeWindow; //to do: send this packet when "a window is forcibly closed, such as when a chest is destroyed while it's open."
	prepareSendMacro(1024 * 1024);

	id.write(data);
	*(data++) = winId;

	finishSendMacro;
}
void message::play::send::windowItems(Player* p, Byte winId, varInt stateId, varInt count, Slot** slots, const Slot& carried) {
	varInt id = (int)id::windowItems;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	*(data++) = winId;
	stateId.write(data);
	count.write(data);
	for (int i = 0; i < count; i++)
		slots[i]->write(data);
	carried.write(data);

	finishSendMacro;
}
void message::play::send::windowProperty(Player* p, Byte winId, bshort property, bshort value) {
	varInt id = (int)id::windowProperty;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	*(data++) = winId;
	property.write(data);
	value.write(data);

	finishSendMacro;
}
void message::play::send::setSlot(Player* p, Byte winId, varInt stateId, bshort slot, const Slot& slotData)
{
	varInt id = (int)id::setSlot;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	*(data++) = winId;
	stateId.write(data);
	slot.write(data);
	slotData.write(data);

	finishSendMacro;
}
void message::play::send::setCooldown(Player* p, varInt itemId, varInt cooldown) {
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
void message::play::send::entityProperties(Player* p, varInt eid, varInt nOfProperties, EntityProperty* properties)
{
	varInt id = (int)id::entityProperties;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);
	nOfProperties.write(data);
	for (int i = 0; i < nOfProperties; i++)
	{
		EntityProperty& prop = properties[i];
		prop.key.write(data);
		prop.value.write(data);
		varInt(0).write(data);
	}

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
void message::play::send::entityHeadLook(Player* p, varInt eid, Angle headYaw)
{
	varInt id = (int)id::entityHeadLook;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);
	*(data++) = (char&)headYaw;

	finishSendMacro;
}
void message::play::send::destroyEntities(Player* p, varInt count, varInt* eids)
{
	varInt id = (int)id::destroyEntities;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	count.write(data);
	for (int i = 0; i < count; i++) eids[i].write(data);

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

/*void message::translateChunk()
{

}
void message::translateLight()
{

}*/
void message::play::send::sendFullChunk(Player* p, int cX, int cZ)
{
	Chunk* chunk = p->world->get(cX, cZ, true);

	//update light
	int sectionCount = (int)chunk->lightData.size();

	varInt id = (int)id::updateLight;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	varInt(cX).write(data);
	varInt(cZ).write(data);
	*(data++) = true;
	//sky light mask length
	varInt((int)chunk->skyLightMask->getCompactedSize()).write(data);
	//sky light mask
	chunk->skyLightMask->write(data);
	//block light mask length
	varInt((int)chunk->blockLightMask->getCompactedSize()).write(data);
	//block light mask
	chunk->blockLightMask->write(data);
	//empty sky light mask length
	varInt((int)chunk->emptySkyLightMask->getCompactedSize()).write(data);
	//empty sky light mask
	chunk->emptySkyLightMask->write(data);
	//empty block light mask length
	varInt((int)chunk->emptyBlockLightMask->getCompactedSize()).write(data);
	//empty block light mask
	chunk->emptyBlockLightMask->write(data);
	//sky light array count
	varInt c;
	for (int i = 0; i < sectionCount; i++) if (chunk->skyLightMask->getElement(i)) c++;
	c.write(data);
	//sky light arrays
	for (int i = 0; i < sectionCount; i++) if (chunk->skyLightMask->getElement(i))
	{
		LightSection::lightArrayLength.write(data);
		ull size = chunk->lightData[i].skyLight->getCompactedSize();
		blong* lightArray = chunk->lightData[i].skyLight->getCompactedValues();
		for (ull i = 0; i < size; i++)
		{
			*(((ull*&)(data))++) = lightArray[i];
		}
	}
	//block light array count
	c = 0;
	for (int i = 0; i < sectionCount; i++) if (chunk->blockLightMask->getElement(i)) c++;
	c.write(data);
	//block light arrays
	for (int i = 0; i < sectionCount; i++) if (chunk->blockLightMask->getElement(i))
	{
		LightSection::lightArrayLength.write(data);
		ull size = chunk->lightData[i].blockLight->getCompactedSize();
		blong* lightArray = chunk->lightData[i].blockLight->getCompactedValues();
		for (ull i = 0; i < size; i++)
		{
			*(((ull*&)(data))++) = lightArray[i];
		}
	}

	finishSendMacro;

	//chunk data
	id = (int)id::chunkData;
	prepareSendMacroNoDecl(1024 * 1024);

	id.write(data);
	bint(cX).write(data);
	bint(cZ).write(data);

	//Chunk* chunk = p->world->get(cX, cZ);
	int worldHeight = p->world->height;
	sectionCount = worldHeight >> 4;

	//primary bitmask length
	varInt((int)chunk->sectionMask->getCompactedSize()).write(data);
	//primary bitmask
	chunk->sectionMask->write(data);
	//heightMaps
	nbt_compound nbt_heightmap("", new nbt * [1]{
		new nbt_long_array("MOTION_BLOCKING",*chunk->heightmaps)
		}, 1);
	nbt_heightmap.write(data);
	//biomes length
	varInt(uint(64 * chunk->sections.size())).write(data);
	//biomes
	for (int s = 0; s < chunk->sections.size(); s++)
		for (int y = 0; y < 4; y++)
			for (int z = 0; z < 4; z++)
				for (int x = 0; x < 4; x++)
					chunk->sections[s].biomes[x][y][z].write(data);

	//data preparation
	char* dataBuffer = new char[1024024], * dataStart = dataBuffer;
	for (int i = 0; i < sectionCount; i++)
	{
		Section& section = chunk->sections[i];
		if (section.blockCount)
		{
			section.blockCount.write(dataBuffer);
			*(dataBuffer++) = section.bitsPerBlock;
			if (!section.useGlobalPallete)
			{
				varInt((uint)section.palette.size()).write(dataBuffer);
				for (const PaletteEntry val : section.palette) val.block.id.write(dataBuffer);
			}
			varInt((uint)section.blockStates->getCompactedSize()).write(dataBuffer);
			section.blockStates->write(dataBuffer);
		}
	}
	uint dataSize = (uint)(dataBuffer - dataStart);
	//size
	varInt(dataSize).write(data);
	//data
	for (uint i = 0; i < dataSize; i++) *(data++) = dataStart[i];
	delete[] dataStart;

	//nOfBlockEntities
	chunk->nOfBlockEntities.write(data);
	//blockEntities

	finishSendMacroNoDecl;
}

void message::play::receive::keepAlive(Player* p, blong keepAlive_id)
{
	if (keepAlive_id != p->lastKeepAliveId) return;
	p->ping = (int)((int64)cycleTime - (int64)keepAlive_id);
	broadcastMessage(send::playerInfo(player_macro, playerInfo::updateLatency, 1, &p));
	p->lastKeepAliveId = -1;
}
void message::play::receive::teleportConfirm(Player* p, varInt teleportId)
{
	if (teleportId == p->pendingTpId) p->pendingTpId = -1;
}
void message::play::receive::clientStatus(Player*, varInt actionId)
{

}
void message::play::receive::clientSettings(Player* p, const mcString& locale, Byte viewDistance, ChatMode chatMode, bool chatColors, Byte displayedSkinParts, Hand mainHand, bool disableTextFiltering)
{
	p->locale = locale;
	p->viewDistance = (p->viewDistance > (int)viewDistance) ? (int)viewDistance : p->viewDistance;
	p->chatMode = chatMode;
	p->chatColors = chatColors;
	p->displayedSkinParts = displayedSkinParts;
	p->mainHand = mainHand;
	p->disableTextFiltering = disableTextFiltering;
}
void message::play::receive::closeWindow(Player* p, Byte winId) {
	message::play::send::closeWindow(p, winId);
}
void message::play::receive::interactEntity(Player* p, varInt eid, varInt type, bfloat targetX, bfloat targetY, bfloat targetZ, Hand mainHand, bool sneaking)
{

	if (type == 2) {

	}

}
void message::play::receive::chatMessage(Player* p, mcString& content)
{
	if (content[0] == '/')
	{
		try
		{
			content.erase(0, 1);
			try
			{
				if (!Commands::dispatch(p, content)) throw Chat("Unknown command", Chat::color::red);
			}
			catch (const Chat& msg)
			{
				send::chatMessage(p, msg, ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
			}
			catch (...)
			{
				send::chatMessage(p, Chat("Internal error occured", Chat::color::red), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
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
	else {
		Chat msg(('<' + p->username + "> " + content).c_str());

		for (Player* pl : Player::players)
			message::play::send::chatMessage(pl, msg, 0, *p->uuid);
	}
}
void message::play::receive::playerPosition(Player* p, bdouble X, bdouble feetY, bdouble Z, bool onGround)
{
	if (p->pendingTpId != -1)
		return;

	for (Player* seener : p->seenBy) ignoreExceptions(message::play::send::entityPosition(seener, p->eid, short((X - p->X) * 4096), short((feetY - p->Y) * 4096), short((Z - p->Z) * 4096), onGround));
	p->updatePosition(X, feetY, Z);
	p->onGround = onGround;
}
void message::play::receive::playerPositionAndRotation(Player* p, bdouble X, bdouble Y, bdouble Z, bfloat yaw, bfloat pitch, bool onGround)
{
	if (p->pendingTpId != -1)
		return;

	for (Player* seener : p->seenBy)
	{
		ignoreExceptions(message::play::send::entityPositionAndRotation(seener, p->eid, short((X - p->X) * 4096), short((Y - p->Y) * 4096), short((Z - p->Z) * 4096), (float)yaw, (float)pitch, onGround));
		ignoreExceptions(message::play::send::entityHeadLook(seener, p->eid, (float)p->yaw));
	}
	p->updatePosition(X, Y, Z);
	p->updateRotation(yaw, pitch);
	p->onGround = onGround;
}
void message::play::receive::playerRotation(Player* p, bfloat yaw, bfloat pitch, bool onGround)
{
	for (Player* seener : p->seenBy)
	{
		ignoreExceptions(message::play::send::entityRotation(seener, p->eid, (float)yaw, (float)pitch, onGround));
		ignoreExceptions(message::play::send::entityHeadLook(seener, p->eid, (float)p->yaw));
	}

	p->updateRotation(yaw, pitch);
	p->onGround = onGround;
}
void message::play::receive::heldItemChange(Player* p, bshort slot)
{
	p->selectedSlot = slot;

	Equipment** eqp = new Equipment * [1];
	eqp[0] = new Equipment(0, p->slots[36 + slot]);

	for (Player* seener : p->seenBy)
		message::play::send::entityEquipment(seener, p->eid, eqp);
	
	delete[] eqp;
}
void message::play::receive::creativeInventoryAction(Player* p, bshort slot, Slot* clickedItem)
{
	if (slot == -1) { //throw away from inventory, create entity
		Log::txt() << "create!" << "\n";
	}
	else { //put in inventory
		p->slots[slot] = clickedItem;

		if (p->selectedSlot == slot - 36) {
			Equipment** eqp = new Equipment * [1];
			eqp[0] = new Equipment(0, clickedItem);

			for (Player* seener : p->seenBy)
				message::play::send::entityEquipment(seener, p->eid, eqp);

			delete[] eqp;
		}
	}
}
void message::play::receive::animation(Player* p, Hand hand)
{
	Animation animation = (hand == p->mainHand) ? Animation::swingMainArm : Animation::swingOffhand;

	for (Player* seener : p->seenBy)
		ignoreExceptions(message::play::send::entityAnimation(seener, p->eid, animation));
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
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: query block nbt");
		}
		break;
		case play::id::setDifficulty:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: set difficulty");
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

			IF_PROTOCOL_WARNINGS(Log::txt() << "\nPartially handled packet: client status");
		}
		break;
		case play::id::clientSettings:
		{
			mcString locale;
			Byte viewDistance, displayedSkinParts;
			varInt chatMode, mainHand;
			bool chatColors, disableTextFiltering;

			locale.read(data);
			viewDistance = *(data++);
			chatMode.read(data);
			chatColors = *(data++);
			displayedSkinParts = *(data++);
			mainHand.read(data);
			disableTextFiltering = *(data++);

			play::receive::clientSettings(p, locale, viewDistance, (ChatMode)(int)chatMode, chatColors, displayedSkinParts, (Hand)(int)mainHand, disableTextFiltering);
		}
		break;
		case play::id::tabComplete_serverbound:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: tab complete");
		}
		break;
		case play::id::clickWindowButton:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: click window button");
		}
		break;
		case play::id::clickWindow:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: click window");
		}
		break;
		case play::id::closeWindow_serverbound:
		{
			Byte windowID;
			windowID = *(data++);

			play::receive::closeWindow(p, windowID);

			IF_PROTOCOL_WARNINGS(Log::txt() << "\nPartially handled packet: close window");
		}
		break;
		case play::id::pluginMessage_serverbound:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: plugin message");
		}
		break;
		case play::id::editBook:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: edit book");
		}
		break;
		case play::id::queryEntityNbt:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: query entity nbt");
		}
		break;
		case play::id::interactEntity:
		{
			varInt eid, type, mainHand;
			bfloat targetX, targetY, targetZ;
			bool sneaking;

			eid.read(data);
			type.read(data);
			if (type == 2) {
				targetX.read(data);
				targetY.read(data);
				targetZ.read(data);
				mainHand.read(data);
			}
			sneaking = *(data++);

			message::play::receive::interactEntity(p, eid, type, targetX, targetY, targetZ, (Hand)(int)mainHand, sneaking);
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nPartially handled packet: interact entity");
		}
		break;
		case play::id::generateStructure:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: generate structure");
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
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: lock difficulty");
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
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: player movement");
		}
		break;
		case play::id::vehicleMove:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: vehicle move");
		}
		break;
		case play::id::steerBoat:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: steer boat");
		}
		break;
		case play::id::pickItem:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: pick item");
		}
		break;
		case play::id::craftRecipeRequest:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: craft recipe request");
		}
		break;
		case play::id::playerAbilities_serverbound:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: player abilities");
		}
		break;
		case play::id::playerDigging:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: player digging");
		}
		break;
		case play::id::entityAction:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: entity action");
		}
		break;
		case play::id::steerVehicle:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: steer vehicle");
		}
		break;
		case play::id::pong:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: pong");
		}
		break;
		case play::id::setRecipeBookState:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: set recipe book state");
		}
		break;
		case play::id::setDisplayedRecipe:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: set displayed recipe");
		}
		break;
		case play::id::nameItem:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: name item");
		}
		break;
		case play::id::resourcePackStatus:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: resource pack status");
		}
		break;
		case play::id::advancementTab:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: advancement tab");
		}
		break;
		case play::id::selectTrade:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: select trade");
		}
		break;
		case play::id::setBeaconEffect:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: set beacon effect");
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
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: update command block");
		}
		break;
		case play::id::updateCommandBlockMinecart:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: update command block minecart");
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
			if (present) {
				itemId.read(data);
				count = *(data++);

				if (nbt::checkTag(data, nbt::tag::Compound))
					item_data->read(data);
			}

			Slot* item = new Slot(present, itemId, count, item_data);
			play::receive::creativeInventoryAction(p, slot, item);
		}
		break;
		case play::id::updateJigsawBlock:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: update jigsaw block");
		}
		break;
		case play::id::updateStructureBlock:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: update structure block");
		}
		break;
		case play::id::updateSign:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: update sign");
		}
		break;
		case play::id::animation_serverbound:
		{
			varInt hand;
			hand.read(data);
			play::receive::animation(p, (Hand)(int)hand);
		}
		break;
		case play::id::spectate:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: spectate");
		}
		break;
		case play::id::playerBlockPlacement:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: player block placement");
		}
		break;
		case play::id::useItem:
		{
			IF_PROTOCOL_WARNINGS(Log::txt() << "\nUnhandled packet: use item");
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