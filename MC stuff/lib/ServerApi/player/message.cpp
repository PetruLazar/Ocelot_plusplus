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

#define disconnectAfter(p,f) try { f; } catch (...) { p->disconnect(); throw; } p->disconnect()
#define prepareSendMacro(x) char* data = new char[x] + 6, *start = data
#define prepareSendMacroNoDecl(x) data = new char[x] + 6; start = data
#define finishSendMacro sendPacketData(p, start, data - start)

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

	disconnectAfter(p, finishSendMacro);
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

	disconnectAfter(p, finishSendMacro);
}
void message::login::send::encryptionRequest(Player* p, varInt publicKeyLength, byte* publicKey, varInt verifyTokenLength, byte* verifyToken)
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
	p->eid = 0x17;
	//position, rotation ad world
	p->world = World::worlds[World::spawnWorld];
	p->X = p->world->spawn.X;
	p->Y = p->world->spawn.Y;
	p->Z = p->world->spawn.Z;
	p->chunkX = p->world->spawn.ChunkX;
	p->chunkZ = p->world->spawn.ChunkZ;
	p->yaw = p->world->spawn.Yaw;
	p->pitch = p->world->spawn.Pitch;
	//playerInfo data
	p->ping = -1;
	p->hasDisplayName = false;
	//p->displayName = new Chat(("[Tester] " + username).c_str());

	login::send::setCompression(p, 128);

	login::send::success(p, *p->uuid, username);

	play::send::joinGame(p, (int)p->eid, false, gamemode::creative, gamemode::none, 0, nullptr, World::dimension_codec, p->world->characteristics, p->world->name, 0x5f19a34be6c9129a, 0, p->viewDistance, false, false, false, p->world->isFlat);

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

	play::send::updateViewPosition(p, p->chunkX, p->chunkZ);

	play::send::timeUpdate(p, 6000i64, 6000i64);

	play::send::spawnPosition(p, p->world->spawn.Absolute, 0.f);

	for (int x = p->chunkX - p->viewDistance; x <= p->chunkX + p->viewDistance; x++) for (int z = p->chunkZ - p->viewDistance; z <= p->chunkZ + p->viewDistance; z++)
	{
		play::send::sendFullChunk(p, x, z);
		//play::send::updateLight(p, x, z);
		//play::send::chunkData(p, x, z);
	}

	play::send::playerPosAndLook(p, p->X, p->Y, p->Z, p->yaw, p->pitch, 0, 0x0, false);

	Player::broadcastChat(Chat((p->username + " joined the game").c_str(), Chat::yellow), p);
	broadcastMessageOmit(play::send::playerInfo(player_macro, playerInfo::addPlayer, 1, &p), p)
}
void message::login::receive::encryptionResponse(Player*, varInt sharedSecretLength, byte* sharedSecret, varInt verifyTokenLength, byte* verifyToken)
{
	throw protocolError("Encryption not supported");
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
			varInt((byte)player->gm).write(data);
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
			varInt((byte)player->gm).write(data);
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
void message::play::send::playerPosAndLook(Player* p, bigEndian<double> x, bigEndian<double> y, bigEndian<double> z, bigEndian<float> yaw, bigEndian<float> pitch, byte flags, varInt teleportId, bool dismountVehicle)
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
	teleportId.write(data);
	*(data++) = dismountVehicle;

	finishSendMacro;
}
void message::play::send::playerAbilities(Player* p, bool invulnerable, bool flying, bool allowFlying, bool creative, bigEndian<float> flyingSpeed, bigEndian<float> fovModifier)
{
	varInt id = (int)id::playerAbilities_clientbound;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	*(data++) = char(byte(invulnerable) | (byte(flying) << 1) | (byte(allowFlying) << 2) | (byte(creative) << 3));
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
void message::play::send::pluginMessage(Player* p, const mcString& channel, ull byteCount, const char* bytes)
{
	varInt id = (int)id::pluginMessage_clientbound;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	channel.write(data);
	for (ull i = 0; i < byteCount; i++) *(data++) = bytes[i];

	finishSendMacro;
}
void message::play::send::heldItemChange(Player* p, byte slot)
{
	varInt id = (int)id::heldItemChange_clientbound;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	*(data++) = slot;

	finishSendMacro;
}
void message::play::send::serverDifficulty(Player* p, byte difficulty, bool isLocked)
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

	disconnectAfter(p, finishSendMacro);
}
void message::play::send::chatMessage(Player* p, const Chat& msg, byte position, const mcUUID& sender)
{
	varInt id = (int)id::chatMessage_clientbound;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	msg.write(data);
	*(data++) = position;
	sender.write(data);

	finishSendMacro;
}
void message::play::send::changeGameState(Player* p, byte reason, bfloat value)
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
	tags(p, Tags::defaltTagsLengthCount, Tags::defaultTags);
}
void message::play::send::tags(Player* p, varInt tagCategoryCount, Tags* tags)
{
	varInt id = (int)id::tags;
	prepareSendMacro(1024 * 1024);

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

	finishSendMacro;
}
void message::play::send::declareCommands(Player* p)
{
	declareCommands(p, Node::defaultCommandsCount, Node::defaultCommands, Node::defaultCommandsRootIndex);
}
void message::play::send::declareCommands(Player* p, varInt count, Node* nodes, varInt root)
{
	varInt id = (int)id::declareCommands;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	count.write(data);
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
	root.write(data);

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
	*(data++) = (byte)gm;
	*(data++) = (byte)prev_gm;
	*(data++) = isDebug;
	*(data++) = isFlat;
	*(data++) = copyMetadata;

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
	*(data++) = (byte&)yaw;
	*(data++) = (byte&)pitch;

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
}

void message::play::receive::keepAlive(Player* p, blong keepAlive_id)
{
	p->ping = (int)((int64)cycleTime - (int64)keepAlive_id);
	broadcastMessage(send::playerInfo(player_macro, playerInfo::updateLatency, 1, &p));
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
		try
		{
			char* command = (char*)content.c_str();
			Command::parse(p, command);
		}
		catch (const Chat& errormessage)
		{
			send::chatMessage(p, errormessage, 1, mcUUID(0, 0, 0, 0));
		}
		catch (...)
		{
			throw;
		}
		return;
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
				send::sendFullChunk(p, (int)loadX, z);
				//send::updateLight(p, (int)loadX, (int)z);
				//send::chunkData(p, loadX, z);
			}
			bint loadZ = newChunkZ - p->viewDistance;
			for (int x = newChunkX - p->viewDistance + 1; x <= newChunkX + p->viewDistance; x++)
			{
				send::sendFullChunk(p, x, (int)loadZ);
				//send::updateLight(p, (int)x, (int)loadZ);
				//send::chunkData(p, x, loadZ);
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
				send::sendFullChunk(p, (int)loadX, z);
				//send::updateLight(p, (int)loadX, (int)z);
				//send::chunkData(p, loadX, z);
			}
			bint loadZ = newChunkZ + p->viewDistance;
			for (int x = newChunkX - p->viewDistance + 1; x <= newChunkX + p->viewDistance; x++)
			{
				send::sendFullChunk(p, x, (int)loadZ);
				//send::updateLight(p, (int)x, (int)loadZ);
				//send::chunkData(p, x, loadZ);
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
				send::sendFullChunk(p, (int)loadX, z);
				//send::updateLight(p, (int)loadX, (int)z);
				//send::chunkData(p, loadX, z);
			}
			bint loadZ = newChunkZ - p->viewDistance;
			for (int x = newChunkX - p->viewDistance; x < newChunkX + p->viewDistance; x++)
			{
				send::sendFullChunk(p, x, (int)loadZ);
				//send::updateLight(p, (int)x, (int)loadZ);
				//send::chunkData(p, x, loadZ);
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
				send::sendFullChunk(p, (int)loadX, z);
				//send::updateLight(p, (int)loadX, (int)z);
				//send::chunkData(p, loadX, z);
			}
			bint loadZ = newChunkZ + p->viewDistance;
			for (int x = newChunkX - p->viewDistance; x < newChunkX + p->viewDistance; x++)
			{
				send::sendFullChunk(p, x, (int)loadZ);
				//send::updateLight(p, (int)x, (int)loadZ);
				//send::chunkData(p, x, loadZ);
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
				send::sendFullChunk(p, (int)loadX, z);
				//send::updateLight(p, (int)loadX, (int)z);
				//send::chunkData(p, loadX, z);
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
				send::sendFullChunk(p, (int)loadX, z);
				//send::updateLight(p, (int)loadX, (int)z);
				//send::chunkData(p, loadX, z);
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
				send::sendFullChunk(p, x, (int)loadZ);
				//send::updateLight(p, (int)x, (int)loadZ);
				//send::chunkData(p, x, loadZ);
			}
		}
		else
		{
			//towards positive Z
			bint unloadZ = p->chunkZ - p->viewDistance;
			for (int x = p->chunkX - p->viewDistance; x <= p->chunkX + p->viewDistance; x++) send::unloadChunk(p, x, unloadZ);
			bint loadZ = newChunkZ + p->viewDistance;
			for (int x = newChunkX - p->viewDistance; x <= newChunkX + p->viewDistance; x++)
			{
				send::sendFullChunk(p, x, (int)loadZ);
				//send::updateLight(p, (int)x, (int)loadZ);
				//send::chunkData(p, x, loadZ);
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

	try
	{
		p->send(data, size, toDelete);
	}
	catch (...)
	{
		//delete[] toDelete;
		throw;
	}

	//delete[] toDelete;
}
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