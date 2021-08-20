#include "message.h"
#include "options.h"
#include "world.h"

playerInfo::Player::Player(const mcUUID& uuid, const mcString& name, gamemode gm, int ping) :uuid(uuid), name(name), gm((byte)gm), ping(ping) { }

#define disconnectAfter(p,f) try { f; } catch (...) { p->disconnect(); throw; } p->disconnect()

void message::handshake::receive::standard(Player* p, varInt protocolVersion, const mcString& serverAdress, Port port, varInt nextState)
{
	p->protocolVersion = protocolVersion;
	p->state = (ConnectionState)(int)nextState;
}
void message::handshake::receive::legacy(Player* p, byte payload)
{
	throw "Legacy handshake not yet implemented";
}

void message::status::send::respose(Player* p, const mcString& jsonResponse)
{
	varInt id_ = (int)id::response;
	char* data = new char[1024 * 1024], * start = data;

	id_.write(data);
	jsonResponse.write(data);

	sendPacketData(p, start, data - start);
}
void message::status::send::pong(Player* p, blong payload)
{
	varInt id = (int)id::pong;
	char* data = new char[9], * start = data;

	id.write(data);
	payload.write(data);

	disconnectAfter(p, sendPacketData(p, start, data - start));
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
	char* data = new char[1024 * 1024], * start = data;

	id.write(data);
	reason.write(data);

	disconnectAfter(p, sendPacketData(p, start, data - start));
}
void message::login::send::encryptionRequest(Player*, varInt publicKeyLength, byte* publicKey, varInt verifyTokenLength, byte* verifyToken)
{
	throw "Encryption not supported";
}
void message::login::send::setCompression(Player*, varInt threshold)
{
	//support for compression
	throw protocolError("compression not supported");
}
void message::login::send::success(Player* p, const mcUUID& uuid, const mcString& username)
{
	p->state = ConnectionState::play;

	varInt id = (int)id::success;
	char* data = new char[1024 * 1024], * start = data;

	id.write(data);
	uuid.write(data);
	username.write(data);

	sendPacketData(p, start, data - start);
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
	p->uuid = new mcUUID(mcUUID::player);
	p->username = username;
	p->nextKeepAlive = clock() + p->keepAliveInterval;
	p->world = World::worlds[1];
	p->X = p->world->spawn.X;
	p->Y = p->world->spawn.Y;
	p->Z = p->world->spawn.Z;
	p->yaw = p->world->spawn.Yaw;
	p->pitch = p->world->spawn.Pitch;
	p->viewDistance = Options::viewDistance();
	login::send::success(p, *p->uuid, username);

	//mcString* wlds = new mcString("world");
	play::send::joinGame(p, 0x17, false, gamemode::creative, gamemode::none, 0, nullptr, World::dimension_codec, World::worlds[0]->characteristics, World::worlds[0]->name, 0x5f19a34be6c9129a, 0, p->viewDistance, false, true, false, p->world->isFlat);
	//delete wlds;

	play::send::pluginMessage(p, "minecraft:brand", 10, "\x9lazorenii");

	play::send::serverDifficulty(p, 2, false);

	play::send::playerAbilities(p, false, true, true, false, 0.05f, 0.1f);

	play::send::declareRecipes(p, 0);

	playerInfo::Player* pl = new playerInfo::Player(*p->uuid, p->username, gamemode::survival, 100);
	play::send::playerInfo(p, playerInfo::addPlayer, 1, pl);
	delete pl;

	play::send::tags(p);

	play::send::updateViewPosition(p, p->chunkX, p->chunkZ);

	play::send::timeUpdate(p, 6000i64, 6000i64);

	play::send::spawnPosition(p, p->world->spawn.Absolute, 0.f);

	for (int x = p->chunkX - p->viewDistance; x <= p->chunkX + p->viewDistance; x++) for (int z = p->chunkZ - p->viewDistance; z <= p->chunkZ + p->viewDistance; z++)
	{
		play::send::updateLight(p, x, z);
		play::send::chunkData(p, x, z);
	}

	play::send::playerPosAndLook(p, p->X, p->Y, p->Z, p->yaw, p->pitch, 0, 0x0, false);
}
void message::login::receive::encryptionResponse(Player*, varInt sharedSecretLength, byte* sharedSecret, varInt verifyTokenLength, byte* verifyToken)
{
	throw protocolError("Encryption not supported");
}

void message::play::send::keepAlive(Player* p, blong keepAlive_id)
{
	varInt id = (int)id::keepAlive_clientbound;
	char* data = new char[1024 * 1024], * start = data;

	id.write(data);
	keepAlive_id.write(data);

	sendPacketData(p, start, data - start);
}
void message::play::send::joinGame(Player* p, bint eid, bool isHardcore, gamemode gm, gamemode prev_gm, varInt worldCount, mcString* worldNames, const nbt_compound& dimensionCodec,
	const nbt_compound& dimension, const mcString& worldName, int64 hashedSeedHigh, varInt maxPlayers, varInt viewDistance, bool reducedDebugInfo, bool respawnScreen, bool isDebug, bool isFlat)
{
	varInt id = (int)id::joinGame;
	char* data = new char[1024 * 1024], * start = data;

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

	sendPacketData(p, start, data - start);
}
void message::play::send::playerInfo(Player* p, varInt action, varInt playerCount, playerInfo::Player* players)
{
	varInt id = (int)id::playerInfo;
	char* data = new char[1024 * 1024], * start = data;

	id.write(data);
	action.write(data);
	playerCount.write(data);
	switch (action)
	{
	case playerInfo::addPlayer:
		for (int i = 0; i < playerCount; i++)
		{
			playerInfo::Player& player = players[i];
			player.uuid.write(data);
			player.name.write(data);
			player.nOfProperties.write(data);
			player.gm.write(data);
			player.ping.write(data);
			*(data++) = player.hasDisplayName;
		}
		break;
	case playerInfo::updateGm:
		throw "WIP";
		for (int i = 0; i < playerCount; i++)
		{

		}
		break;
	case playerInfo::updateLatency:
		throw "WIP";
		for (int i = 0; i < playerCount; i++)
		{

		}
		break;
	case playerInfo::updateDisplayName:
		throw "WIP";
		for (int i = 0; i < playerCount; i++)
		{

		}
		break;
	case playerInfo::removePlayer:
		throw "WIP";
		for (int i = 0; i < playerCount; i++)
		{

		}
		break;
	}

	sendPacketData(p, start, data - start);
}
void message::play::send::chunkData(Player* p, bint cX, bint cZ)
{
	varInt id = (int)id::chunkData;
	char* data = new char[1024 * 1024], * start = data;

	id.write(data);
	cX.write(data);
	cZ.write(data);

	Chunk* chunk = p->world->get(cX, cZ);
	int worldHeight = World::worlds[0]->characteristics["height"].vInt();
	int sectionCount = worldHeight >> 4;

	//primary bitmask length
	varInt(((sectionCount - 1) >> 6) + 1).write(data);
	//primary bitmask
	BitStream bitmask(1);
	for (int i = 0; i < sectionCount; i++)
		bitmask << (bool)(chunk->sections[i].blockCount);
	bitmask.write(data);
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
	//size

	//data
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
	varInt(size).write(data);
	for (uint i = 0; i < size; i++) *(data++) = dataStart[i];
	delete[] dataStart;

	//nOfBlockEntities
	varInt(0).write(data);
	//blockEntities

	sendPacketData(p, start, data - start);
}
void message::play::send::chunkData(Player* p, bint cX, bint cZ, varInt bitMaskLength, blong* bitMask, const nbt_compound& heightMaps, varInt biomesLength, varInt* biomes,
	varInt dataSize, char* chunkData, varInt nOfBlockEntities, nbt_compound* blockEntities)
{
	varInt id = (int)id::chunkData;
	char* data = new char[1024 * 1024], * start = data;

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

	sendPacketData(p, start, data - start);
}
void message::play::send::playerPosAndLook(Player* p, bigEndian<double> x, bigEndian<double> y, bigEndian<double> z, bigEndian<float> yaw, bigEndian<float> pitch, byte flags, varInt teleportId, bool dismountVehicle)
{
	varInt id = (int)id::playerPosAndLook_clientbound;
	char* data = new char[1024 * 1024], * start = data;

	id.write(data);
	x.write(data);
	y.write(data);
	z.write(data);
	yaw.write(data);
	pitch.write(data);
	*(data++) = flags;
	teleportId.write(data);
	*(data++) = dismountVehicle;

	sendPacketData(p, start, data - start);
}
void message::play::send::playerAbilities(Player* p, bool invulnerable, bool flying, bool allowFlying, bool creative, bigEndian<float> flyingSpeed, bigEndian<float> fovModifier)
{
	varInt id = (int)id::playerAbilities_clientbound;
	char* data = new char[1024 * 1024], * start = data;

	id.write(data);
	*(data++) = char(byte(invulnerable) | (byte(flying) << 1) | (byte(allowFlying) << 2) | (byte(creative) << 3));
	flyingSpeed.write(data);
	fovModifier.write(data);

	sendPacketData(p, start, data - start);
}
void message::play::send::timeUpdate(Player* p, blong worldAge, blong timeOfDay)
{
	varInt id = (int)id::timeUpdate;
	char* data = new char[1024 * 1024], * start = data;

	id.write(data);
	worldAge.write(data);
	timeOfDay.write(data);

	sendPacketData(p, start, data - start);
}
void message::play::send::pluginMessage(Player* p, const mcString& channel, ull byteCount, const char* bytes)
{
	varInt id = (int)id::pluginMessage_clientbound;
	char* data = new char[1024 * 1024], * start = data;

	id.write(data);
	channel.write(data);
	for (ull i = 0; i < byteCount; i++) *(data++) = bytes[i];

	sendPacketData(p, start, data - start);
}
void message::play::send::heldItemChange(Player* p, byte slot)
{
	varInt id = (int)id::heldItemChange_clientbound;
	char* data = new char[1024 * 1024], * start = data;

	id.write(data);
	*(data++) = slot;

	sendPacketData(p, start, data - start);
}
void message::play::send::serverDifficulty(Player* p, byte difficulty, bool isLocked)
{
	varInt id = (int)id::serverDifficulty;
	char* data = new char[1024 * 1024], * start = data;

	id.write(data);
	*(data++) = difficulty;
	*(data++) = isLocked;

	sendPacketData(p, start, data - start);
}
void message::play::send::spawnPosition(Player* p, Position location, bfloat angle)
{
	varInt id = (int)id::spawnPosition;
	char* data = new char[1024 * 1024], * start = data;

	id.write(data);
	location.write(data);
	angle.write(data);

	sendPacketData(p, start, data - start);
}
void message::play::send::declareRecipes(Player* p, varInt nOfRecipes)
{
	varInt id = (int)id::declareRecipes;
	char* data = new char[1024 * 1024], * start = data;

	id.write(data);
	nOfRecipes.write(data);

	sendPacketData(p, start, data - start);
}
void message::play::send::updateViewPosition(Player* p, varInt chunkX, varInt chunkZ)
{
	varInt id = (int)id::updateViewPosition;
	char* data = new char[1024 * 1024], * start = data;

	id.write(data);
	chunkX.write(data);
	chunkZ.write(data);

	sendPacketData(p, start, data - start);
}
void message::play::send::updateLight(Player* p, varInt cX, varInt cZ)
{
	blong* lightMask = new blong(0b11111111111111111111111111i64);
	char* sectionLight = new char[2048]{  };
	for (int i = 0; i < 2048; i++) sectionLight[i] = 0xffi8;
	char** arrays = new char* [26]{ sectionLight,sectionLight,sectionLight,sectionLight,sectionLight,sectionLight,sectionLight,sectionLight,sectionLight,sectionLight,sectionLight,sectionLight,sectionLight,sectionLight,sectionLight,sectionLight,sectionLight,sectionLight,sectionLight,sectionLight,sectionLight,sectionLight,sectionLight,sectionLight,sectionLight,sectionLight };
	for (int i = 0; i < 26; i++) sectionLight[i] = 0xffi8;

	try
	{
		play::send::updateLight(p, cX, cZ, true, 1, lightMask, 1, lightMask, 1, lightMask, 1, lightMask, 26, arrays, 26, arrays);
	}
	catch (...)
	{
		delete[] lightMask;
		delete[] sectionLight;
		delete[] arrays;
		throw;
	}

	delete[] lightMask;
	delete[] sectionLight;
	delete[] arrays;
}
void message::play::send::updateLight(Player* p, varInt cX, varInt cZ, bool trustEdges,
	varInt length1, blong* skyLightMask, varInt length2, blong* blockLightMask,
	varInt length3, blong* emptySkyLightMask, varInt length4, blong* emptyBlockLightMask,
	varInt skyLightArrayCount, char** skyLightArrays, varInt blockLightArrayCount, char** blockLightArrays)
{
	varInt id = (int)id::updateLight;
	char* data = new char[1024 * 1024], * start = data;

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

	sendPacketData(p, start, data - start);
}
void message::play::send::disconnect(Player* p, const Chat& reason)
{
	varInt id = (int)id::disconnect;
	char* data = new char[1024 * 1024], * start = data;

	id.write(data);
	reason.write(data);

	disconnectAfter(p, sendPacketData(p, start, data - start));
}
void message::play::send::chatMessage(Player* p, const Chat& msg, byte position, const mcUUID& sender)
{
	varInt id = (int)id::chatMessage_clientbound;
	char* data = new char[1024 * 1024], * start = data;

	id.write(data);
	msg.write(data);
	*(data++) = position;
	sender.write(data);

	sendPacketData(p, start, data - start);
}
void message::play::send::changeGameState(Player* p, byte reason, bfloat value)
{
	varInt id = (int)id::changeGameState;
	char* data = new char[1024 * 1024], * start = data;

	id.write(data);
	*(data++) = reason;
	value.write(data);

	sendPacketData(p, start, data - start);
}
void message::play::send::unloadChunk(Player* p, bint x, bint z)
{
	varInt id = (int)id::unloadChunk;
	char* data = new char[1024], * start = data;

	id.write(data);
	x.write(data);
	z.write(data);

	p->world->unload(x, z);

	sendPacketData(p, start, data - start);
}
void message::play::send::tags(Player* p)
{
	tags(p, Tags::defaltTagsLengthCount, Tags::defaultTags);
}
void message::play::send::tags(Player* p, varInt tagCategoryCount, Tags* tags)
{
	varInt id = (int)id::tags;
	char* data = new char[1024*1024], * start = data;

	id.write(data);
	tagCategoryCount.write(data);
	for (int i = 0; i < tagCategoryCount; i++)
	{
		tags[i].tagType.write(data);
		tags[i].tagCount.write(data);
		int& count = tags[i].tagCount;
		for (int j = 0; j < count; j++)
		{
			Tags::Tag& tag = tags[i].tags[j];
			tag.name.write(data);
			tag.entryCount.write(data);
			for (int k = 0; k < tag.entryCount; k++) tag.entries[k].write(data);
		}
	}

	sendPacketData(p, start, data - start);
}

void message::play::receive::keepAlive(Player* p, blong keepAlive_id)
{
	if (keepAlive_id == p->lastKeepAliveId) p->lastKeepAliveId = -1;
}
void message::play::receive::teleportConfirm(Player* p, varInt teleportId)
{

}
void message::play::receive::clientSettings(Player* p, const mcString& locale, byte viewDistance, varInt chatMode, bool chatColors, byte displayedSkinParts, varInt mainHand, bool disableTextFiltering)
{

}
void message::play::receive::chatMessage(Player* p, const mcString& content)
{
	if (content[0] == '/')
	{
		if (content == "/fast")
		{
			return;
		}
		if (content == "/fly")
		{
			return;
		}
		if (content == "/gamemode")
		{
			play::send::changeGameState(p, 3, 0.f);
			return;
		}
		if (content == "/test")
		{
			return;
		}
		//return;
	}
	Chat msg(('<' + p->username + "> " + content).c_str());

	for (Player* pl : Player::players)
		message::play::send::chatMessage(pl, msg, 0, *p->uuid);
}
void message::play::receive::playerPosition(Player* p, bdouble X, bdouble feetY, bdouble Z, bool onGround)
{
	int newChunkX = fastfloor(X) >> 4,
		newChunkZ = fastfloor(Z) >> 4;

	if (newChunkX != p->chunkX && newChunkZ != p->chunkZ)
	{
		send::updateViewPosition(p, newChunkX, newChunkZ);
		if (newChunkX < p->chunkX && newChunkZ < p->chunkZ)
		{
			//towards negative X
			//towards negative Z
			bint unloadX = p->chunkX + p->viewDistance;
			for (int z = p->chunkZ - p->viewDistance; z <= p->chunkZ + p->viewDistance; z++) send::unloadChunk(p, unloadX, z);
			bint unloadZ = p->chunkZ + p->viewDistance;
			for (int x = p->chunkX - p->viewDistance; x < p->chunkX + p->viewDistance; x++) send::unloadChunk(p, x, unloadZ);

			bint loadX = newChunkX - p->viewDistance;
			for (int z = newChunkZ - p->viewDistance; z <= newChunkZ + p->viewDistance; z++)
			{
				send::updateLight(p, (int)loadX, (int)z);
				send::chunkData(p, loadX, z);
			}
			bint loadZ = newChunkZ - p->viewDistance;
			for (int x = newChunkX - p->viewDistance + 1; x <= newChunkX + p->viewDistance; x++)
			{
				send::updateLight(p, (int)x, (int)loadZ);
				send::chunkData(p, x, loadZ);
			}
		}
		else if (newChunkX < p->chunkX)
		{
			//towards negative X
			//towards positive Z
			bint unloadX = p->chunkX + p->viewDistance;
			for (int z = p->chunkZ - p->viewDistance; z <= p->chunkZ + p->viewDistance; z++) send::unloadChunk(p, unloadX, z);
			bint unloadZ = p->chunkZ - p->viewDistance;
			for (int x = p->chunkX - p->viewDistance; x < p->chunkX + p->viewDistance; x++) send::unloadChunk(p, x, unloadZ);

			bint loadX = newChunkX - p->viewDistance;
			for (int z = newChunkZ - p->viewDistance; z <= newChunkZ + p->viewDistance; z++)
			{
				send::updateLight(p, (int)loadX, (int)z);
				send::chunkData(p, loadX, z);
			}
			bint loadZ = newChunkZ + p->viewDistance;
			for (int x = newChunkX - p->viewDistance + 1; x <= newChunkX + p->viewDistance; x++)
			{
				send::updateLight(p, (int)x, (int)loadZ);
				send::chunkData(p, x, loadZ);
			}
		}
		else if (newChunkZ < p->chunkZ)
		{
			//???
			//towards positive X
			//towards negative Z
			bint unloadX = p->chunkX - p->viewDistance;
			for (int z = p->chunkZ - p->viewDistance; z <= p->chunkZ + p->viewDistance; z++) send::unloadChunk(p, unloadX, z);
			bint unloadZ = p->chunkZ + p->viewDistance;
			for (int x = p->chunkX - p->viewDistance + 1; x <= p->chunkX + p->viewDistance; x++) send::unloadChunk(p, x, unloadZ);

			bint loadX = newChunkX + p->viewDistance;
			for (int z = newChunkZ - p->viewDistance; z <= newChunkZ + p->viewDistance; z++)
			{
				send::updateLight(p, (int)loadX, (int)z);
				send::chunkData(p, loadX, z);
			}
			bint loadZ = newChunkZ - p->viewDistance;
			for (int x = newChunkX - p->viewDistance; x < newChunkX + p->viewDistance; x++)
			{
				send::updateLight(p, (int)x, (int)loadZ);
				send::chunkData(p, x, loadZ);
			}
		}
		else
		{
			//towards positive X
			//towards positive Z
			bint unloadX = p->chunkX - p->viewDistance;
			for (int z = p->chunkZ - p->viewDistance; z <= p->chunkZ + p->viewDistance; z++) send::unloadChunk(p, unloadX, z);
			bint unloadZ = p->chunkZ - p->viewDistance;
			for (int x = p->chunkX - p->viewDistance + 1; x <= p->chunkX + p->viewDistance; x++) send::unloadChunk(p, x, unloadZ);

			bint loadX = newChunkX + p->viewDistance;
			for (int z = newChunkZ - p->viewDistance; z <= newChunkZ + p->viewDistance; z++)
			{
				send::updateLight(p, (int)loadX, (int)z);
				send::chunkData(p, loadX, z);
			}
			bint loadZ = newChunkZ + p->viewDistance;
			for (int x = newChunkX - p->viewDistance; x < newChunkX + p->viewDistance; x++)
			{
				send::updateLight(p, (int)x, (int)loadZ);
				send::chunkData(p, x, loadZ);
			}
		}
	}
	else if (newChunkX != p->chunkX)
	{
		send::updateViewPosition(p, newChunkX, newChunkZ);
		if (newChunkX < p->chunkX)
		{
			//towards negative X
			bint unloadX = p->chunkX + p->viewDistance;
			for (int z = p->chunkZ - p->viewDistance; z <= p->chunkZ + p->viewDistance; z++) send::unloadChunk(p, unloadX, z);
			bint loadX = newChunkX - p->viewDistance;
			for (int z = newChunkZ - p->viewDistance; z <= newChunkZ + p->viewDistance; z++)
			{
				send::updateLight(p, (int)loadX, (int)z);
				send::chunkData(p, loadX, z);
			}
		}
		else
		{
			//towards positive X
			bint unloadX = p->chunkX - p->viewDistance;
			for (int z = p->chunkZ - p->viewDistance; z <= p->chunkZ + p->viewDistance; z++) send::unloadChunk(p, unloadX, z);
			bint loadX = newChunkX + p->viewDistance;
			for (int z = newChunkZ - p->viewDistance; z <= newChunkZ + p->viewDistance; z++)
			{
				send::updateLight(p, (int)loadX, (int)z);
				send::chunkData(p, loadX, z);
			}
		}
	}
	else if (newChunkZ != p->chunkZ)
	{
		send::updateViewPosition(p, newChunkX, newChunkZ);
		if (newChunkZ < p->chunkZ)
		{
			//towards negative Z
			bint unloadZ = p->chunkZ + p->viewDistance;
			for (int x = p->chunkX - p->viewDistance; x <= p->chunkX + p->viewDistance; x++) send::unloadChunk(p, x, unloadZ);
			bint loadZ = newChunkZ - p->viewDistance;
			for (int x = newChunkX - p->viewDistance; x <= newChunkX + p->viewDistance; x++)
			{
				send::updateLight(p, (int)x, (int)loadZ);
				send::chunkData(p, x, loadZ);
			}
		}
		else
		{
			//towards positive Z
			bint unloadZ = p->chunkZ - p->viewDistance;
			for (int x = p->chunkX - p->viewDistance; x <= p->chunkX + p->viewDistance; x++)
			{
				send::unloadChunk(p, x, unloadZ);
				bint loadZ = newChunkZ + p->viewDistance;
				for (int x = newChunkX - p->viewDistance; x <= newChunkX + p->viewDistance; x++)
				{
					send::updateLight(p, (int)x, (int)loadZ);
					send::chunkData(p, x, loadZ);
				}
			}
		}
	}

	p->X = X;
	p->Y = feetY;
	p->Z = Z;
	p->onGround = onGround;
	p->chunkX = newChunkX;
	p->chunkZ = newChunkZ;
}
void message::play::receive::playerPositionAndRotation(Player* p, bdouble X, bdouble Y, bdouble Z, bfloat yaw, bfloat pitch, bool onGround)
{
	playerRotation(p, yaw, pitch, onGround);
	playerPosition(p, X, Y, Z, onGround);
}
void message::play::receive::playerRotation(Player* p, bfloat yaw, bfloat pitch, bool onGround)
{
	p->yaw = yaw;
	p->pitch = pitch;
	p->onGround = onGround;
}

void message::sendPacketData(Player* p, char* data, ull size)
{
	varInt len = (int)(uint)size;
	char* lendata = new char[3], * buffer = lendata;
	len.write(buffer);

	//suport for compression

	try
	{
		p->send(lendata, buffer - lendata);
		p->send(data, size);
	}
	catch (...)
	{
		delete[] lendata;
		delete[] data;
		throw;
	}

	delete[] lendata;
	delete[] data;
}
void message::dispatch(Player* p, char* data, size_t size)
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
			throw protocolWarning("Partially handled packet: teleport confirm");
		}
		break;
		case play::id::queryBlockNbt:
		{
			throw protocolWarning("Unhandled packet: query block nbt");
		}
		break;
		case play::id::setDifficulty:
		{
			throw protocolWarning("Unhandled packet: set difficulty");
		}
		break;
		case play::id::chatMessage_serverbound:
		{
			{
				mcString content;
				content.read(data);
				message::play::receive::chatMessage(p, content);
			}
			throw protocolWarning("Partially handled packet: chat message");
		}
		break;
		case play::id::clientStatus:
		{
			throw protocolWarning("Unhandled packet: client status");
		}
		break;
		case play::id::clientSettings:
		{
			{
				mcString locale;
				byte viewDistance, displayedSkinParts;
				varInt chatMode, mainHand;
				bool chatColors, disableTextFiltering;

				locale.read(data);
				viewDistance = *(data++);
				chatMode.read(data);
				chatColors = *(data++);
				displayedSkinParts = *(data++);
				mainHand.read(data);
				disableTextFiltering = *(data++);

				play::receive::clientSettings(p, locale, viewDistance, chatMode, chatColors, displayedSkinParts, mainHand, disableTextFiltering);
			}
			throw protocolWarning("Partially handled packet: client settings");
		}
		break;
		case play::id::tabComplete_serverbound:
		{
			throw protocolWarning("Unhandled packet: tab complete");
		}
		break;
		case play::id::clickWindowButton:
		{
			throw protocolWarning("Unhandled packet: click window button");
		}
		break;
		case play::id::clickWindow:
		{
			throw protocolWarning("Unhandled packet: click window");
		}
		break;
		case play::id::closeWindow_serverbound:
		{
			throw protocolWarning("Unhandled packet: close window");
		}
		break;
		case play::id::pluginMessage_serverbound:
		{
			throw protocolWarning("Unhandled packet: plugin message");
		}
		break;
		case play::id::editBook:
		{
			throw protocolWarning("Unhandled packet: edit book");
		}
		break;
		case play::id::queryEntityNbt:
		{
			throw protocolWarning("Unhandled packet: query entity nbt");
		}
		break;
		case play::id::interactEntity:
		{
			throw protocolWarning("Unhandled packet: interact entity");
		}
		break;
		case play::id::generateStructure:
		{
			throw protocolWarning("Unhandled packet: generate structure");
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
			throw protocolWarning("Unhandled packet: lock difficulty");
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
			throw protocolWarning("Partially handled packet: player position");
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
			throw protocolWarning("Partially handled packet: player position and rotation");
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
			throw protocolWarning("Partially handled packet: player rotation");
		}
		break;
		case play::id::playerMovement:
		{
			throw protocolWarning("Unhandled packet: player movement");
		}
		break;
		case play::id::vehicleMove:
		{
			throw protocolWarning("Unhandled packet: vehicle move");
		}
		break;
		case play::id::steerBoat:
		{
			throw protocolWarning("Unhandled packet: steer boat");
		}
		break;
		case play::id::pickItem:
		{
			throw protocolWarning("Unhandled packet: pick item");
		}
		break;
		case play::id::craftRecipeRequest:
		{
			throw protocolWarning("Unhandled packet: craft recipe request");
		}
		break;
		case play::id::playerAbilities_serverbound:
		{
			throw protocolWarning("Unhandled packet: player abilities");
		}
		break;
		case play::id::playerDigging:
		{
			throw protocolWarning("Unhandled packet: player digging");
		}
		break;
		case play::id::entityAction:
		{
			throw protocolWarning("Unhandled packet: entity action");
		}
		break;
		case play::id::steerVehicle:
		{
			throw protocolWarning("Unhandled packet: steer vehicle");
		}
		break;
		case play::id::pong:
		{
			throw protocolWarning("Unhandled packet: pong");
		}
		break;
		case play::id::setRecipeBookState:
		{
			throw protocolWarning("Unhandled packet: set recipe book state");
		}
		break;
		case play::id::setDisplayedRecipe:
		{
			throw protocolWarning("Unhandled packet: set displayed recipe");
		}
		break;
		case play::id::nameItem:
		{
			throw protocolWarning("Unhandled packet: name item");
		}
		break;
		case play::id::resourcePackStatus:
		{
			throw protocolWarning("Unhandled packet: resource pack status");
		}
		break;
		case play::id::advancementTab:
		{
			throw protocolWarning("Unhandled packet: advancement tab");
		}
		break;
		case play::id::selectTrade:
		{
			throw protocolWarning("Unhandled packet: select trade");
		}
		break;
		case play::id::setBeaconEffect:
		{
			throw protocolWarning("Unhandled packet: set beacon effect");
		}
		break;
		case play::id::heldItemChange_serverbound:
		{
			throw protocolWarning("Unhandled packet: held item change");
		}
		break;
		case play::id::updateCommandBlock:
		{
			throw protocolWarning("Unhandled packet: update command block");
		}
		break;
		case play::id::updateCommandBlockMinecart:
		{
			throw protocolWarning("Unhandled packet: update command block minecart");
		}
		break;
		case play::id::creativeInventoryAction:
		{
			throw protocolWarning("Unhandled packet: creative inventoty action");
		}
		break;
		case play::id::updateJigsawBlock:
		{
			throw protocolWarning("Unhandled packet: update jigsaw block");
		}
		break;
		case play::id::updateStructureBlock:
		{
			throw protocolWarning("Unhandled packet: update structure block");
		}
		break;
		case play::id::updateSign:
		{
			throw protocolWarning("Unhandled packet: update sign");
		}
		break;
		case play::id::animation_serverbound:
		{
			throw protocolWarning("Unhandled packet: animation");
		}
		break;
		case play::id::spectate:
		{
			throw protocolWarning("Unhandled packet: spectate");
		}
		break;
		case play::id::playerBlockPlacement:
		{
			throw protocolWarning("Unhandled packet: player block placement");
		}
		break;
		case play::id::useItem:
		{
			throw protocolWarning("Unhandled packet: use item");
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