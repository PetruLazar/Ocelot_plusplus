#pragma once
#include "player.h"
#include "nbt.h"
#include "json.h"

//A log of the vanilla client playing on a vanilla server:
//S: join game - done
//S : plugin message - done
//S : server difficulty - done
//S : player abilities - done
//S : held item change
//S : declare recipes - done
//S : tags
//										C: client settings
//S : entity status
//										C: plugin message
//S : declare commands
//S : unlock recipes
//S : player position and look - done
//S : player info
//S : player info
//S : update view position
//S : update light
//S : chunk data
//S : update light
//S : chunk data
//S : update light
//S : chunk data
//S : update light
//S : chunk data
//S : update light
//S : chunk data
//S : update light
//S : chunk data
//S : update light
//S : chunk data
//S : update light
//S : chunk data
//S : update light
//S : chunk data
//S : spawn living entity
//S : entity metadata
//S : entity properties
//S : spawn living entity
//S : entity metadata
//S : entity properties
//S : update light
//S : chunk data
//S : update light
//S : chunk data
//S : update light
//S : chunk data
//S : update light
//S : chunk data
//S : update light
//S : chunk data
//S : update light
//S : chunk data
//S : update light
//S : chunk data
//S : spawn entity
//S : entity metadata
//S : entity velocity
//S : spawn living entity
//S : entity metadata
//S : entity properties
//S : spawn entity
//S : entity metadata
//S : entity velocity
//S : spawn living entity
//S : entity metadata
//S : entity properties
//S : update light
//S : chunk data
//S : spawn living entity
//S : entity metadata
//S : entity properties
//S : update light
//S : chunk data
//S : update light
//S : chunk data
//S : update light
//S : chunk data
//S : update light
//S : chunk data
//S : update light
//S : chunk data
//S : update light
//S : chunk data
//S : update light
//S : chunk data
//S : update light
//S : chunk data
//S : spawn living entity
//S : entity metadata
//S : entity properties
//										C: teleport confirm
//S : spawn living entity
//										C: player position and rotation
//S : entity metadata
//S : entity properties
//										C: player position and rotation
//...
//window Items?
//spawnPosition? - done

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
class tags
{
	mcString id;
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
			serverDifficulty,
			chatMessage_clientbound,
			clearTitles,
			tabComplete_clientbound,
			declareCommands,
			closeWindow,
			windowItems,
			windowProperty,
			setSlot,
			setCooldown,
			pluginMessage_clientbound,
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
			joinGame,
			mapData,
			tradeList,
			entityPosition,
			entitypositionAndRotation,
			entityRotation,
			vehicleMove_clientbound,
			openBook,
			openWindow,
			openSignEditor,
			ping,
			craftRecipeResponse,
			playerAbilities_clientbound,
			endCombatEvent,
			enterCombatEvent,
			deathCombatEvent,
			playerInfo,
			facePlayer,
			playerPosAndLook_clientbound,
			unlockRecipes,
			destroyEntities,
			removeEntityEffect,
			resourcePackSend,
			respawn,
			entityHeadLook,
			multiBLockChange,
			selectAdvancementTab,
			actionBar,
			worldBorderCenter,
			worldBorderLerpSize,
			worldBorderSize,
			worldBorderWarningDelay,
			worldBorderWarningReach,
			camera,
			heldItemChange_clientbound,
			updateViewPosition,
			updateViewDistance,
			spawnPosition,
			displayScoreboard,
			entityMetadata,
			attachEntity,
			entityVelocity,
			entityEquipment,
			setXp,
			updateHp,
			scoreboardObjective,
			setPassengers,
			teams,
			updateScore,
			setTitleSubtitle,
			timeUpdate,
			setTitleText,
			setTitleTimes,
			entitySoundEffect,
			soundEffect,
			stopSound,
			playerListHeaderAndFooter,
			nbtQueryResponse,
			collectItem,
			entityTeleport,
			advancements, //auto tab starts here, why?
			entityProperties,
			entityEffect,
			declareRecipes,
			tags,

			teleportConfirm = 0x00,
			queryBlockNbt,
			setDifficulty,
			chatMessage_serverbound,
			clientStatus,
			clientSettings,
			tabComplete_serverbound,
			clickWindowButton,
			clickWindow,
			closeWindow_serverbound,
			pluginMessage_serverbound,
			editBook,
			queryEntityNbt,
			interactEntity,
			generateStructure,
			keepAlive_serverbound,
			lockDifficulty,
			playerPosition,
			playerPositionAndRotation_serverbound,
			playerRotation,
			playerMovement,
			vehicleMove,
			steerBoat,
			pickItem,
			craftRecipeRequest,
			playerAbilities_serverbound,
			playerDigging,
			entityAction,
			steerVehicle,
			pong,
			setRecipeBookState,
			setDisplayedRecipe,
			nameItem,
			resourcePackStatus,
			advancementTab,
			selectTrade,
			setBeaconEffect,
			heldItemChange_serverbound,
			updateCommandBlock,
			updateCommandBlockMinecart,
			creativeInventoryAction,
			updateJigsawBlock,
			updateStructureBlock,
			updateSign,
			animation_serverbound,
			spectate,
			playerBlockPlacement,
			useItem
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
			static void chatMessage(Player*, const Chat& data, byte position, const mcUUID& sender);
			static void clearTitles(Player*, bool reset);
			//static void tabComplete(Player*, varInt id, varInt start, varInt length, varInt count, );
			//static void declareCommands(Player*, varInt count, Node* nodes, varInt root);
			static void closeWindow(Player*, byte winId);
			//static void windowItems(Player*, byte winId, varInt stateId, varInt count, Slot* slots, const Slot& carried);
			static void windowProperty(Player*, byte winId, bshort property, bshort value);
			//static void setSlot(Player*, byte winId, varInt stateId, bshort slot, const Slot& data);
			static void setCooldown(Player*, varInt itemId, varInt cooldown);
			static void pluginMessage(Player*, const mcString& channel, ull dataSize, const char* data);
			static void namedSoundEffect(Player*, const mcString& soundName, varInt category, bint effectX, bint effectY, bint effectZ, bfloat volume, bfloat pitch);
			static void disconnect(Player*, const Chat& reason);
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
			static void updateLight(Player*, varInt cX, varInt cZ, bool trustEdges, varInt length1, blong* skyLightMask, varInt length2, blong* blockLightMask, varInt length3, blong* emptySkyLightMask, varInt length4, blong* emptyBlockLightMask, varInt skyLightArrayCount, char** skyLightArrays, varInt blockLightArrayCount, char** blockLightArrays);
			static void joinGame(Player*, bint Eid, bool isHardcore, gamemode gm, gamemode prev_gm, varInt worldCount, mcString* worldNames, const nbt_compound& dimensionCodec, const nbt_compound& dimension, const mcString& worldName, int64 hashedSeedHigh, varInt maxPlayers, varInt viewDistance, bool reducedDebugInfo, bool respawnScreen, bool isDebug, bool isFlat);
			//static void mapData(Player*, varInt mapId, byte scale, bool locked, bool trackingPosition, varInt iconCount, Icon* icons, byte optColumns, byte optRows, byte optX, byte optZ, varInt optLength, byte* data);
			//static void tradeList(Player*, varInt winId, byte size, Trade* trades, varInt villagerLevel, varInt experience, bool isRegularVillager, bool canRestock);
			static void entityPosition(Player*, varInt eid, bshort deltaX, bshort deltaY, bshort deltaZ, bool onGround);
			static void entityPositionAndRotation(Player*, varInt eid, bshort deltaX, bshort deltaY, bshort deltaZ, Angle yaw, Angle pitch, bool onGround);
			static void entityRotation(Player*, varInt eid, Angle yaw, Angle pitch, bool onGround);
			static void vehicleMove(Player*, bdouble x, bdouble y, bdouble z, bfloat yaw, bfloat pitch);
			static void openBook(Player*, varInt hand);
			//static void openWindow(Player*, varInt winId, varInt winType, const Chat& winTitle);
			static void openSignEditor(Player*, Position location);
			static void ping(Player*, bint pingId);
			static void craftRecipeResponse(Player*, byte winId, const mcString& recipeIdentifier);
			static void playerAbilities(Player*, bool invulnerable, bool flying, bool allowFlying, bool creative, bigEndian<float> flyingSpeed, bigEndian<float> fovModifier);
			static void endCombatEvent(Player*, varInt duration, bint eid);
			static void enterCombatEvent(Player*);
			//static void deathCombatEvent(Player*, varInt playerId, bint eid, const Chat& message);
			static void playerInfo(Player*, varInt action, varInt playerCount, playerInfo::Player*);
			static void facePlayer(Player*, varInt pivot, bdouble targetX, bdouble targetY, bdouble targetZ, bool isEntity, varInt eid, varInt targetPivot);
			static void playerPosAndLook(Player*, bigEndian<double> x, bigEndian<double> y, bigEndian<double> z, bigEndian<float> yaw, bigEndian<float> pitch, byte flags, varInt teleportId, bool dismountVehicle);
			static void unlockRecipes(Player*, varInt action, bool bookOpen, bool filterActive, bool smeltingOpen, bool smeltingFilter, bool blastOpen, bool blastFilter, bool smokerOpen, bool smokerFilter, varInt size1, mcString* array1, varInt size2, mcString* array2);
			static void destroyEntities(Player*, varInt count, varInt* eids);
			static void removeEntityEffect(Player*, varInt eid, byte effectId);
			//static void resourcePackSend(Player*, const mcString& url, const mcString& hash, bool forced, bool hasPromptMessage, const Chat& promptMessage);
			static void respawn(Player*, const nbt_compound& dimension, const mcString& worldName, blong hashedSeed, byte gm, byte prev_gm, bool isDebug, bool isFlat, bool copyMetadata);
			static void entityHeadLook(Player*, varInt eid, Angle headYaw);
			static void multiBlockChange(Player, blong sectionPosition, bool unknown, varInt size, varLong* blocks);
			static void selectAdvancementTab(Player*, bool hasId, const mcString& id);
			//static void actionBar(Player*, const Chat& actionBarText);
			static void worldBorderCenter(Player*, bdouble x, bdouble z);
			static void worldBorderLerpSize(Player*, bdouble oldDiameter, bdouble newDiameter, varLong speed);
			static void worldBorderSize(Player*, bdouble diameter);
			static void worldBorderWarningDelay(Player*, varInt warningTime);
			static void worldBorderWarningReach(Player*, varInt warningBlocks);
			static void camera(Player*, varInt camId);
			static void heldItemChange(Player*, byte slot);
			static void updateViewPosition(Player*, varInt chunkX, varInt chunkZ);
			static void updateViewDistance(Player*, varInt distance);
			static void spawnPosition(Player*, Position location, bfloat angle);
			static void displayScoreboard(Player*, byte position, const mcString& scoreName);
			//static void entityMetadata(varInt eid, const Metadata& metadata);
			static void attachEntity(Player*, bint attachedEid, bint holdingEid);
			static void entityVelocity(Player*, varInt eid, bshort velocityX, bshort velocityY, bshort velocityZ);
			//static void entityEquipment(Player*, varInt eid, Equipment* equipment);
			static void setXp(Player*, bfloat xpBar, varInt level, varInt totalXp);
			static void updateHp(Player*, bfloat hp, varInt food, bfloat saturation);
			//static void scoreboardObjective(Player*, const mcString& name, byte mode, const Chat& value, varInt type);
			static void setPassengers(Player*, varInt vehicleEid, varInt count, varInt* passenders);
			//static void teams(Player*, const mcString& name, byte mode, ...);
			static void updateScore(Player*, const mcString& name, byte action, const mcString& objective, varInt value);
			//static void setTitleSubtitle(Player*, const Chat& subtitle);
			static void timeUpdate(Player*, blong worldAge, blong timeOfDay);
			//static void setTitleText(Player*, const Chat& title);
			static void setTitleTimes(Player*, bint fadeIn, bint stay, bint fadeOut);
			static void entitySoundEffect(Player*, varInt soundId, varInt category, varInt eid, bfloat volume, bfloat pitch);
			static void soundEffect(Player*, varInt soundId, varInt category, bint effectPosX, bint effectPosY, bint effectPosZ, bfloat volume, bfloat pitch);
			static void stopSound(Player*, byte flags, varInt source, const mcString& name);
			//static void playerListHeaderAndFooter(Player*, const Chat& header, const Chat& footer);
			static void nbtQueryResponse(Player*, varInt transactionId, const nbt& tag);
			static void collectItem(Player*, varInt collectedEid, varInt collectorEid, varInt pickupCount);
			static void entityTeleport(Player*, varInt eid, bdouble x, bdouble y, bdouble z, Angle yaw, Angle pitch, bool onGround);
			//static void advancements(Player*, bool reset,)
			//static void entityProperties(Player*, varInt eid, varInt nOfProperties, Property* properties);
			static void entityEffect(Player*, varInt entityId, byte effectId, char amplifier, varInt duration, byte flags);
			static void declareRecipes(Player*, varInt nOfRecipes);
			//static void tags(Player*, varInt length, TagGroup* tags);
		};
		struct receive
		{
			static void teleportConfirm(Player*, varInt id);
			static void queryBlockNbt(Player*, varInt transactionId, Position location);
			static void setDifficulty(Player*, byte difficulty);
			static void chatMessage(Player*, const mcString& content);
			static void clientStatus(Player*, varInt actionId);
			static void clientSettings(Player*, const mcString& locale, byte viewDistance, varInt chatMode, bool chatColors, byte displayedSkinParts, varInt mainHand, bool disableTextFiltering);
			static void tabComplete(Player*, varInt transactionId, const mcString& text);
			static void clickWindowButton(Player*, byte winId, byte buttonId);
			//static void clickWindow(Player*, byte winId, bshort slot, varInt stateId, byte button, varInt mode, varInt length, );
			static void closeWindow(Player*, byte winId);
			static void pluginMessage(Player*, const mcString& channel, ull dataSize, char* data);
			//static void editBook(Player*, varInt hand, varInt count, mcString* pages,???);
			static void queryEntityNbt(Player*, varInt transactionId, varInt eid);
			static void interactEntity(Player*, varInt eid, varInt type, bfloat targetX, bfloat targetY, bfloat targetZ, varInt  hand, bool sneaking);
			static void generateStructure(Player*, Position location, varInt levels, bool keepJigsaw);
			static void keepAlive(Player*, blong keepAlive_id);
			static void lockDifficulty(Player*, bool locked);
			static void playerPosition(Player*, bdouble x, bdouble feetY, bdouble Z, bool onGround);
			static void playerPositionAndRotation(Player*, bdouble X, bdouble Y, bdouble Z, bfloat yaw, bfloat pitch, bool onGround);
			static void playerRotation(Player*, bfloat yaw, bfloat pitch, bool onGround);
			static void playerMovement(Player*, bool onGround);
			static void vehicleMove(Player*, bdouble x, bdouble y, bdouble z, bfloat yaw, bfloat pitch);
			static void steerBoat(Player*, bool leftPaddleTurning, bool rightPaddleTurning);
			static void pickItem(Player*, varInt slot);
			static void craftRecipeRequest(Player*, byte winId, const mcString& recipe, bool makeAll);
			static void playerAbilities(Player*, byte falgs);
			static void playerDigging(Player*, varInt status, Position location, byte face);
			static void entityAction(Player*, varInt eid, varInt actionId, varInt jumpBoost);
			static void steerVehicle(Player*, bfloat sideways, bfloat forward);
			static void pong(Player*, bint id);
			static void setRecipeBookState(Player*, varInt bookid, bool open, bool filterActive);
			static void setDisplayedRecipe(Player*, const mcString& recipeId);
			static void nameItem(Player*, const mcString newName);
			static void resourcePackStatus(Player*, varInt result);
			static void advancementTab(Player*, varInt action, const mcString& tabId);
			static void selectTrade(Player*, varInt selectedSlot);
			static void setBeaconEffect(Player*, varInt primary, varInt secondary);
			static void heldItemChange(Player*, bshort slot);
			static void updateCommandBlock(Player*, Position location, const mcString& command, varInt mode, byte flags);
			static void updateCommandBlockMinecart(Player*, varInt eid, const mcString& command, bool trackOutput);
			//static void creativeInventoryAction(Player*, bshort slot, const Slot& clickedItem);
			static void updateJigsawBlock(Player*, Position location, const mcString& name, const mcString& target, const mcString& pool, const mcString& finalState, const mcString& jointType);
			static void updateStructureBlock(Player*, Position location, varInt action, varInt mode, const mcString& name, char offsetX, char offsetY, char offsetZ, byte sizeX, byte sizeY, byte sizeZ, varInt mirror, varInt rotation, const mcString& metadata, bfloat integrity, varLong seed, byte flags);
			static void updateSign(Player*, Position location, const mcString& line1, const mcString& line2, const mcString& line3, const mcString& line4);
			static void animation(Player*, varInt hand);
			static void spectate(Player*, const mcUUID& target);
			static void playerBlockPlacement(Player*, varInt hand, Position location, varInt face, bfloat cursorX, bfloat cursorY, bfloat cursorZ, bool insideBlock);
			static void useItem(Player*, varInt hand);
		};
	};

	//dispatch a message from a player
	static void sendPacketData(Player*, char*, ull);
	static void dispatch(Player*, char*, size_t);
};