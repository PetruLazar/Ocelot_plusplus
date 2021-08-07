#pragma once
#include "basic.h"
#include "varData.h"
#include "mcString.h"
#include "player.h"
#include "endian.h"
#include "uuid.h"
#include "nbt.h"

//The vanilla server sends:
// join game
// plugin message
// server difficulty
// player abilities
// held item change
// declare recipes
// tags
// entity status
// 
//

namespace playerInfo
{
	class Player
	{
	public:
		mcUUID uuid;
		mcString name;
		varInt nOfProperties = 0;
		class property
		{
			mcString name;
			mcString value;
			bool isSigned = false;
			mcString signature;
		} *properties = nullptr;
		varInt gm;
		varInt ping;
		bool hasDisplayName = false;
		//Chat displayName - optional

		Player(const mcUUID& uuid, const mcString& name, gamemode gm, int ping);
	};
	enum action
	{
		addPlayer,
		updateGm,
		updateLatency,
		updateDisplayName,
		removePlayer
	};
}
namespace playerPosAndLook
{

}

struct message
{
	//struct for each connection state
	struct handshake
	{
		//possible packet ids during handshake
		enum class id : int
		{
			standard,
			legacy = 0xfe
		};

		struct receive
		{
			static void standard(Player*, varInt protocolVersion, const mcString& serverAdress, Port port, varInt nextState);
			//no length prefix
			//id is 'byte'
			static void legacy(Player*, byte payload);
		};
	};
	struct status
	{
		//possible packet ids during status
		enum class id : int
		{
			response,
			pong,

			request = 0x00,
			ping,
		};

		struct send
		{
			static void respose(Player*, const mcString& jsonResponse);
			static void pong(Player*, blong);
		};
		struct receive
		{
			static void request(Player*);
			static void ping(Player*, blong);
		};
	};
	struct login
	{
		//possible packet ids during login
		enum class id : int
		{
			disconnect,
			encryptionRequest,
			success,
			setCompression,
			loginPluginRequest,

			start = 0x00,
			encryptionResponse,
			loginPluginResponse
		};

		struct send
		{
			static void disconnect(Player*, const mcString& reason);
			static void encryptionRequest(Player*, varInt publicKeyLength, byte* publicKey, varInt verifyTokenLength, byte* verifyToken);
			static void success(Player*, const mcUUID& uuid, const mcString& username);
			static void setCompression(Player*, varInt threshold);
			//static void loginPluginRequest(Player*, ...); - Identifier not implemented
		};
		struct receive
		{
			static void start(Player*, const mcString& username);
			static void encryptionResponse(Player*, varInt sharedSecretLength, byte* sharedSecret, varInt verifyTokenLength, byte* verifyToken);
			//static void loginPluginResponse(Player*, varInt messageId, bool successful, byte* data);
		};
	};
	struct play
	{
		//possible packet ids during play
		enum class id
		{
			keepAlive_clientbound = 0x21,
			chunkData = 0x22,
			joinGame = 0x26,
			playerAbilities = 0x32,
			playerInfo = 0x36,
			playerPosAndLook = 0x38,
			timeUpdate = 0x58,

			keepAlive_serverbound = 0x0f
		};

		struct send
		{
			static void chunkData(Player*, bint cX, bint cZ, varInt bitMaskLength, blong* bitMask, const nbt_compound& heightMaps, varInt biomesLength, varInt* biomes, varInt dataSize, char* chunkData, varInt nOfBlockEntities, nbt_compound* blockEntities);
			static void joinGame(Player*, bint Eid, bool isHardcore, gamemode gm, gamemode prev_gm, varInt worldCount, mcString* worldNames, const nbt_compound& dimensionCodec, const nbt_compound& dimension, mcString worldName, int64 hashedSeedHigh, varInt maxPlayers, varInt viewDistance, bool reducedDebugInfo, bool respawnScreen, bool isDebug, bool isFlat);
			static void playerAbilities(Player*, bool invulnerable, bool flying, bool allowFlying, bool creative, bigEndian<float> flyingSpeed, bigEndian<float> fovModifier);
			static void playerInfo(Player*, varInt action, varInt playerCount, playerInfo::Player*);
			static void playerPosAndLook(Player*, bigEndian<double> x, bigEndian<double> y, bigEndian<double> z, bigEndian<float> yaw, bigEndian<float> pitch, byte flags, varInt teleportId, bool dismountVehicle);
			static void timeUpdate(Player*, blong worldAge, blong timeOfDay);
			static void keepAlive(Player*, blong keepAlive_id);
		};
		struct receive
		{
			static void keepAlive(Player*, blong keepAlive_id);
		};
	};

	//dispatch a message from a player
	static void dispatch(Player*, char*, size_t);
};