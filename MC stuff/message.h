#pragma once
#include "basic.h"
#include "varData.h"
#include "mcString.h"
#include "player.h"
#include "endian.h"
#include "uuid.h"
#include "nbt.h"
#include "angle.h"
#include "position.h"

//The vanilla server sends:
// join game - done
// plugin message
// server difficulty
// player abilities - done
// held item change
// declare recipes
// tags
// entity status
// declare commands
// player position and look - done
// player info - done
// spawn entity
// update view position
//...
//window Items?

namespace painting
{
	enum direction : byte
	{
		south,
		west,
		north,
		east
	};
	enum motive
	{
		kebab,
		aztec,
		alban,
		aztec2,
		bomb,
		plant,
		wasteland,
		pool,
		courbet,
		sea,
		sunset,
		creebet,
		wanderer,
		graham,
		match,
		bust,
		stage,
		void_,
		skull_and_roses,
		wither,
		fighters,
		pointer,
		pigscene,
		burning_skull,
		skeleton,
		donkey_kong
	};
}

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
namespace entityAnimation
{
	enum animation : byte
	{
		swingMainArm,
		takeDamage,
		leaveBed,
		swingOffhand,
		criticalEffect,
		magicCriticalEffect
	};
}
namespace
{
	enum digging
	{
		started,
		cancelled,
		finished
	};
}
namespace blockEntityData
{
	enum action
	{
		setMobSpawnerData,
		setCommandBlockText,
		setBeaconData,
		mobHeadData,
		declareConduit,
		setBannerData,
		setStructureBlockData,
		setEndGatewayDest,
		setSignText,
		unused,
		declareBed,
		setJigsawBlockData,
		setCampfireItems,
		beehiveInfo
	};
}

class statistic
{
public:
	enum category
	{
		mined,
		crafted,
		used,
		broken,
		picked_up,
		dropped,
		killed,
		killed_by,
		custom
	};
	enum custom
	{
		leave_game,
		play_one_minute,
		time_since_death,
		time_since_rest,
		sneak_time,
		walk_one_cm,
		crouch_one_cm,
		sprint_one_cm,
		walk_on_water_one_cm,
		fall_one_cm,
		climb_one_cm,
		fly_one_cm,
		walk_under_water_one_cm,
		minecart_one_cm,
		boat_one_cm,
		pig_one_cm,
		horse_one_cm,
		aviate_one_cm,
		swim_one_cm,
		strider_one_cm,
		jump,
		drop,
		damage_dealt,
		damage_dealt_absorbed,
		damage_dealt_resisted,
		damage_taken,
		damage_blocked_by_shield,
		damage_absorbed,
		damage_resisted,
		deaths,
		mob_kills,
		animals_bred,
		player_kills,
		fish_caught,
		talked_to_villager,
		traded_with_villager,
		eat_cake_slice,
		fill_cauldron,
		use_cauldron,
		clean_armor,
		clean_banner,
		clean_shulker_box,
		interact_with_brewingstand,
		interact_with_beacon,
		inspect_dropper,
		inspect_hopper,
		inspect_dispenser,
		play_noteblock,
		tune_noteblock,
		pot_flower,
		trigger_trapped_chest,
		open_enderchest,

	};

	varInt categoryId;
	varInt statisticId;
	varInt value;
};

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
			spawnEntity,
			spawnXPorb,
			spawnLivingEntity,
			spawnPainting,
			spawnPlayer,
			sculkVibrationSignal,
			entityAnimation,
			statistics,
			acknowledgePlayerDigging,
			blockBreakAnimation,
			blockEntityData,
			blockAction,
			blockChange,
			bossBar,
			serverDIfficulty,
			chatMessage_clientbound,
			clearTitles,
			tabComplete_clientbound,
			declare_commands,
			closeWindow,
			windowItems,
			windowProperty,
			setSlot,
			setCooldown,
			pluginMessage,
			namedSoundEffect,
			disconnect,
			entityStatus,
			explosion,
			unloadChunk,
			changeGameState,
			openHorseWindow,
			initializeWorldBorder,
			keepAlive_clientbound,
			chunkData,
			effect,
			particle,
			updateLight,
			joinGame = 0x26,

			playerAbilities = 0x32,
			playerInfo = 0x36,
			playerPosAndLook = 0x38,
			timeUpdate = 0x58,

			keepAlive_serverbound = 0x0f
		};

		struct send
		{
			static void spawnEntity(Player*, varInt eid, const mcUUID& uuid, varInt type, bigEndian<double> x, bigEndian<double> y, bigEndian<double> z, Angle pitch, Angle yaw, bint data, bshort velocityX, bshort velocityY, bshort velocityZ);
			static void spawnXPorb(Player*, varInt eid, bdouble x, bdouble y, bdouble z, bigEndian<short> xpCount);
			static void spawnLivingEntity(Player*, varInt eid, const mcUUID& uuid, varInt type, bdouble x, bdouble y, bdouble z, Angle yaw, Angle pitch, Angle headPitch, bshort velocityX, bshort velocityY, bshort velocityZ);
			static void spawnPainting(Player*, varInt eid, const mcUUID& uuid, varInt motive, Position location, painting::direction direction);
			static void spawnPlayer(Player*, varInt eid, const mcUUID& uuid, bdouble x, bdouble y, bdouble z, Angle yaw, Angle pitch);
			//static void sculkVibrationSignal(Player*, Position source, const mcString& destinationIdentifier);
			static void entityAnimation(Player*, varInt eid, entityAnimation::animation animation);
			static void statistics(Player*, varInt count, statistic* stats);
			static void acknowledgePlayerDigging(Player*, Position location, varInt block, varInt status, bool successful);
			static void blockBreakAnimation(Player*, varInt eid, Position location, byte destroyStage);
			static void blockEntityData(Player*, Position location, byte action, const nbt& data);
			static void blockAction(Player*, Position location, byte actionId, byte actionParam, varInt blockType);
			static void blockChange(Player*, Position location, varInt blockId);
			//static void bossBar();
			static void serverDifficulty(Player*, byte difficulty, bool isLocked);
			//static void chatMessage(Player*, Chat data, byte position, const mcUUID& sender);
			static void clearTitles(Player*, bool reset);
			//static void tabComplete(Player*, varInt id, varInt start, varInt length, varInt count, );
			//static void declareCommands(Player*, varInt count, Node* nodes, varInt root);
			static void closeWindow(Player*, byte winId);
			//static void windowItems(Player*, byte winId, varInt stateId, varInt count, Slot* slots, const Slot& carried);
			static void windowProperty(Player*, byte winId, bshort property, bshort value);
			//static void setSlot(Player*, byte winId, varInt stateId, bshort slot, Slot data);
			static void setCooldown(Player*, varInt itemId, varInt cooldown);
			static void pluginMessage(Player*, const mcString& channel, char* data);
			static void namedSoundEffect(Player*, const mcString& soundName, varInt category, bint effectX, bint effectY, bint effectZ, bfloat volume, bfloat pitch);
			//static void disconnect(Player*, Chat reason);
			static void entityStatus(Player*, bint eid, byte status);
			static void explosion(Player*, bfloat x, bfloat y, bfloat z, bfloat strength, varInt recordCount, byte* records, bfloat playerMotionX, bfloat playerMotionY, bfloat playerMotionZ);
			static void unloadChunk(Player*, bint x, bint z);
			static void changeGameState(Player*, byte reason, bfloat value);
			static void openHorseWindow(Player*, byte winId, varInt slotCount, bint eid);
			static void initializeWorldBorder(Player*, bdouble x, bdouble z, bdouble oldDiameter, bdouble newDiameter, varLong speed, varInt portalTeleportBoundary, varInt warningBlocks, varInt warningTime);
			static void keepAlive(Player*, blong keepAlive_id);
			static void chunkData(Player*, bint cX, bint cZ, varInt bitMaskLength, blong* bitMask, const nbt_compound& heightMaps, varInt biomesLength, varInt* biomes, varInt dataSize, char* chunkData, varInt nOfBlockEntities, nbt_compound* blockEntities);
			static void effect(Player*, bint effectId, Position location, bint data, bool disableRelativeVolume);
			//static void particle(Player*, bint particleId, bool longDistance, bdouble X, bdouble Y, bdouble Z, bfloat offsetX, bfloat offsetY, bfloat offsetZ, bfloat data, bint count, ???);
			//static void updateLight(Player*, varInt cX, varInt cZ, bool trustEdges, varInt length1, blong* skyLightMask, varInt length2, blong* blockLight, varInt length3, blong* emptySkyLightMask, varInt length4, blong* emptyBlockLightMask, .....);
			static void joinGame(Player*, bint Eid, bool isHardcore, gamemode gm, gamemode prev_gm, varInt worldCount, mcString* worldNames, const nbt_compound& dimensionCodec, const nbt_compound& dimension, const mcString& worldName, int64 hashedSeedHigh, varInt maxPlayers, varInt viewDistance, bool reducedDebugInfo, bool respawnScreen, bool isDebug, bool isFlat);

			static void playerAbilities(Player*, bool invulnerable, bool flying, bool allowFlying, bool creative, bigEndian<float> flyingSpeed, bigEndian<float> fovModifier);
			static void playerInfo(Player*, varInt action, varInt playerCount, playerInfo::Player*);
			static void playerPosAndLook(Player*, bigEndian<double> x, bigEndian<double> y, bigEndian<double> z, bigEndian<float> yaw, bigEndian<float> pitch, byte flags, varInt teleportId, bool dismountVehicle);
			static void timeUpdate(Player*, blong worldAge, blong timeOfDay);
		};
		struct receive
		{
			static void keepAlive(Player*, blong keepAlive_id);
		};
	};

	//dispatch a message from a player
	static void dispatch(Player*, char*, size_t);
};