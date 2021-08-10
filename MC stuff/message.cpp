#include "message.h"
#include "options.h"
#include "world.h"

playerInfo::Player::Player(const mcUUID& uuid, const mcString& name, gamemode gm, int ping) :uuid(uuid), name(name), gm((byte)gm), ping(ping) { }

#define disconnectAfter(p,f) try { f; } catch (const char* c) { p->disconnect(); throw c; } p->disconnect()

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
	throw "compression not supported";
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
	login::send::success(p, mcUUID(), username);

	p->username = username;
	p->nextKeepAlive = clock() + p->keepAliveInterval;

	//mcString* wlds = new mcString("world");
	play::send::joinGame(p, 0x17, false, gamemode::creative, gamemode::none, 0, nullptr, World::dimension_codec, World::dimension, "", 0x5f19a34be6c9129a, 0, 5, false, true, false, true);
	//delete wlds;

	play::send::pluginMessage(p, "minecraft:brand", 10, "\x9lazorenii");

	play::send::serverDifficulty(p, 2, false);

	play::send::playerAbilities(p, false, true, true, false, 0.05f, 0.1f);

	//play::send::heldItemChange(p, 0);

	play::send::declareRecipes(p, 0);

	//tags

	play::send::playerPosAndLook(p, 96.5, 1., 96.5, 0.f, 0.f, 0, 0x6, false);
	play::send::updateViewPosition(p, 5, 5);

	playerInfo::Player* pl = new playerInfo::Player(mcUUID(), username, gamemode::survival, 100);
	play::send::playerInfo(p, playerInfo::addPlayer, 1, pl);
	delete pl;

	play::send::spawnPosition(p, Position(96, 1, 96), 0.f);

	play::send::timeUpdate(p, 6000i64, -6000i64);

	//play::send::chatMessage();
}
void message::login::receive::encryptionResponse(Player*, varInt sharedSecretLength, byte* sharedSecret, varInt verifyTokenLength, byte* verifyToken)
{
	throw "Encryption not supported";
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
//void message::play::send::tags

void message::play::receive::keepAlive(Player* p, blong keepAlive_id)
{
	if (keepAlive_id == p->lastKeepAliveId) p->lastKeepAliveId = -1;
}
void message::play::receive::teleportConfirm(Player* p, varInt teleportId)
{
	blong* bitMask = new blong(0x1i64);

	char* chunkData = new char[20024];
	chunkData[0] = 0x01;
	chunkData[1] = 0x00;
	chunkData[2] = 4;
	chunkData[3] = 2;
	chunkData[4] = 0;
	chunkData[5] = 1; // 34 for water
	char* buffer = chunkData + 6;
	varInt(256).write(buffer);
	//blong(1).write(buffer);
	for (int i = 0; i < 16; i++) blong(0x1111111111111111).write(buffer);
	for (int i = 16; i < 256; i++) blong(0).write(buffer);

	for (int i = 0; i < 12; i++) for (int j = 0; j < 12; j++)
	{
		varInt* biomes = new varInt[1024];
		for (ull k = 0; k < 1024; k++) biomes[k] = (i + j) % 10;
		play::send::chunkData(p, i, j, 1, bitMask, World::heightMap, 1024, biomes, int(buffer - chunkData), chunkData, 0, nullptr);
		//play::send::updateLight();
		delete[] biomes;
	}

	delete bitMask;
	delete[] chunkData;
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
	catch (const char* c)
	{
		delete[] lendata;
		delete[] data;
		throw c;
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
			throw "Invalid packet id";
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
			throw "Invalid packet id";
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
			throw "Login Plugin not supported.";
		}
		break;
		default:
			p->disconnect();
			throw "Invalid packet id";
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
			throw "Partially handled packet: teleport confirm";
		}
		break;
		case play::id::queryBlockNbt:
		{
			throw "Unhandled packet: query block nbt";
		}
		break;
		case play::id::setDifficulty:
		{
			throw "Unhandled packet: set difficulty";
		}
		break;
		case play::id::chatMessage_serverbound:
		{
			throw "Unhandled packet: chat message";
		}
		break;
		case play::id::clientStatus:
		{
			throw "Unhandled packet: client status";
		}
		break;
		case play::id::clientSettings:
		{
			throw "Unhandled packet: client settings";
		}
		break;
		case play::id::tabComplete_serverbound:
		{
			throw "Unhandled packet: tab complete";
		}
		break;
		case play::id::clickWindowButton:
		{
			throw "Unhandled packet: click window button";
		}
		break;
		case play::id::clickWindow:
		{
			throw "Unhandled packet: click window";
		}
		break;
		case play::id::closeWindow_serverbound:
		{
			throw "Unhandled packet: close window";
		}
		break;
		case play::id::pluginMessage_serverbound:
		{
			throw "Unhandled packet: plugin message";
		}
		break;
		case play::id::editBook:
		{
			throw "Unhandled packet: edit book";
		}
		break;
		case play::id::queryEntityNbt:
		{
			throw "Unhandled packet: query entity nbt";
		}
		break;
		case play::id::interactEntity:
		{
			throw "Unhandled packet: interact entity";
		}
		break;
		case play::id::generateStructure:
		{
			throw "Unhandled packet: generate structure";
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
			throw "Unhandled packet: lock difficulty";
		}
		break;
		case play::id::playerPosition:
		{
			throw "Unhandled packet: player position";
		}
		break;
		case play::id::playerPositionAndRotation_serverbound:
		{
			throw "Unhandled packet: player position and rotation";
		}
		break;
		case play::id::playerRotation:
		{
			throw "Unhandled packet: player rotation";
		}
		break;
		case play::id::playerMovement:
		{
			throw "Unhandled packet: player movement";
		}
		break;
		case play::id::vehicleMove:
		{
			throw "Unhandled packet: vehicle move";
		}
		break;
		case play::id::steerBoat:
		{
			throw "Unhandled packet: steer boat";
		}
		break;
		case play::id::pickItem:
		{
			throw "Unhandled packet: pick item";
		}
		break;
		case play::id::craftRecipeRequest:
		{
			throw "Unhandled packet: craft recipe request";
		}
		break;
		case play::id::playerAbilities_serverbound:
		{
			throw "Unhandled packet: player abilities";
		}
		break;
		case play::id::playerDigging:
		{
			throw "Unhandled packet: player digging";
		}
		break;
		case play::id::entityAction:
		{
			throw "Unhandled packet: entity action";
		}
		break;
		case play::id::steerVehicle:
		{
			throw "Unhandled packet: steer vehicle";
		}
		break;
		case play::id::pong:
		{
			throw "Unhandled packet: pong";
		}
		break;
		case play::id::setRecipeBookState:
		{
			throw "Unhandled packet: set recipe book state";
		}
		break;
		case play::id::setDisplayedRecipe:
		{
			throw "Unhandled packet: set displayed recipe";
		}
		break;
		case play::id::nameItem:
		{
			throw "Unhandled packet: name item";
		}
		break;
		case play::id::resourcePackStatus:
		{
			throw "Unhandled packet: resource pack status";
		}
		break;
		case play::id::advancementTab:
		{
			throw "Unhandled packet: advancement tab";
		}
		break;
		case play::id::selectTrade:
		{
			throw "Unhandled packet: select trade";
		}
		break;
		case play::id::setBeaconEffect:
		{
			throw "Unhandled packet: set beacon effect";
		}
		break;
		case play::id::heldItemChange_serverbound:
		{
			throw "Unhandled packet: held item change";
		}
		break;
		case play::id::updateCommandBlock:
		{
			throw "Unhandled packet: update command block";
		}
		break;
		case play::id::updateCommandBlockMinecart:
		{
			throw "Unhandled packet: update command block minecart";
		}
		break;
		case play::id::creativeInventoryAction:
		{
			throw "Unhandled packet: creative inventoty action";
		}
		break;
		case play::id::updateJigsawBlock:
		{
			throw "Unhandled packet: update jigsaw block";
		}
		break;
		case play::id::updateStructureBlock:
		{
			throw "Unhandled packet: update structure block";
		}
		break;
		case play::id::updateSign:
		{
			throw "Unhandled packet: update sign";
		}
		break;
		case play::id::animation_serverbound:
		{
			throw "Unhandled packet: animation";
		}
		break;
		case play::id::spectate:
		{
			throw "Unhandled packet: spectate";
		}
		break;
		case play::id::playerBlockPlacement:
		{
			throw "Unhandled packet: player block placement";
		}
		break;
		case play::id::useItem:
		{
			throw "Unhandled packet: use item";
		}
		break;
		default:
			p->disconnect();
			throw "Invalid packet id";
		}
		break;
	default:
		p->disconnect();
		throw "Invalid connection state";
	}
}