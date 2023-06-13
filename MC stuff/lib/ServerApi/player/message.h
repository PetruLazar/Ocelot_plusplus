#pragma once

#include "player.h"
#include "../nbt.h"
#include "../json.h"
#include "../types.h"

//for Player::disconnect(), use broadcastMessageSafe
//broadcasting destionation is "player_macro"
#define broadcastMessage(msg_f_call) for (Player* player_macro : Player::players) ignoreExceptions(msg_f_call);
//for Player::disconnect(), use broadcastMessageSafe
//broadcasting destionation is "player_macro"
#define broadcastMessageOmit(msg_f_call, omit_player_ptr) for (Player* player_macro : Player::players) if (player_macro != omit_player_ptr) ignoreExceptions(msg_f_call);

//for outside the Player class, use broadcastMessage
//broadcasting destionation is "player_macro"
#define broadcastMessageSafe(msg_f_call) for (Player* player_macro : Player::players) if (player_macro->connected) ignoreExceptions(msg_f_call);
//for outside the Player class, use broadcastMessage
//broadcasting destionation is "player_macro"
#define broadcastMessageOmitSafe(msg_f_call, omit_player_ptr) for (Player* player_macro : Player::players) if (player_macro != omit_player_ptr && player_macro->connected) ignoreExceptions(msg_f_call);

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
			SERVER_API static void standard(Player*, varInt protocolVersion, const mcString& serverAdress, Port port, varInt nextState);
			//no length prefix
			//id is 'Byte'
			SERVER_API static void legacy(Player*, Byte payload);
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
			SERVER_API static void respose(Player*, const mcString& jsonResponse);
			SERVER_API static void pong(Player*, blong);
		};
		struct receive
		{
			SERVER_API static void request(Player*);
			SERVER_API static void ping(Player*, blong);
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
			SERVER_API static void disconnect(Player*, const mcString& reason);
			SERVER_API static void encryptionRequest(Player*, varInt publicKeyLength, Byte* publicKey, varInt verifyTokenLength, Byte* verifyToken);
			SERVER_API static void success(Player*, const mcUUID& uuid, const mcString& username);
			SERVER_API static void setCompression(Player*, varInt threshold);
			//SERVER_API static void loginPluginRequest(Player*, ...); - Identifier not implemented
		};
		struct receive
		{
			SERVER_API static void start(Player*, const mcString& username);
			SERVER_API static void encryptionResponse(Player*, varInt sharedSecretLength, Byte* sharedSecret, varInt verifyTokenLength, Byte* verifyToken);
			//SERVER_API static void loginPluginResponse(Player*, varInt messageId, bool successful, Byte* data);
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
			chunkDataAndLight,
			effect,
			particle,
			updateLight,
			joinGame,
			mapData,
			tradeList,
			entityPosition,
			entityPositionAndRotation,
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
			updateSimulationDistance,
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
			SERVER_API static void spawnEntity(Player*, Entity::entity* entity, bshort velocityX = 0, bshort velocityY = 0, bshort velocityZ = 0);
			SERVER_API static void spawnEntity(Player*, varInt eid, const mcUUID& uuid, Entity::type type, bdouble x, bdouble y, bdouble z, Angle pitch, Angle yaw, bint data, bshort velocityX = 0, bshort velocityY = 0, bshort velocityZ = 0);
			SERVER_API static void spawnXPorb(Player*, varInt eid, bdouble x, bdouble y, bdouble z, bshort xpCount);
			SERVER_API static void spawnLivingEntity(Player*, varInt eid, const mcUUID& uuid, Entity::type type, bdouble x, bdouble y, bdouble z, Angle yaw, Angle pitch, Angle headPitch, bshort velocityX, bshort velocityY, bshort velocityZ);
			SERVER_API static void spawnPainting(Player*, varInt eid, const mcUUID& uuid, Entity::Painting::motive motive, const Position& location, Entity::direction direction);
			SERVER_API static void spawnPlayer(Player*, varInt eid, const mcUUID& uuid, bdouble x, bdouble y, bdouble z, Angle yaw, Angle pitch);
			SERVER_API static void sculkVibrationSignal(Player*, const Position& source, Entity::Sculk::destinationType destinationType, const Entity::Sculk::destination& destination, varInt arrivalTicks);
			SERVER_API static void entityAnimation(Player*, varInt eid, Entity::animation animation);
			//SERVER_API static void statistics(Player*, varInt count, statistic* stats);
			SERVER_API static void acknowledgePlayerDigging(Player*, const Position& location, varInt block, varInt status, bool successful);
			SERVER_API static void blockBreakAnimation(Player*, varInt eid, const Position& location, Byte destroyStage);
			SERVER_API static void blockEntityData(Player*, const Position& location, varInt blockEntityType, const nbt& data);
			SERVER_API static void blockEntityData(Player*, BlockEntity* blockEntity);
			SERVER_API static void blockAction(Player*, const Position& location, Byte actionId, Byte actionParam, varInt blockType);
			SERVER_API static void blockChange(Player*, const Position& location, varInt blockId);
			SERVER_API static void bossBar(Player*, const mcUUID& uuid, bossBar::action action, bossBar::mode* actionField);
			SERVER_API static void serverDifficulty(Player*, Byte difficulty, bool isLocked);
			SERVER_API static void chatMessage(Player*, const Chat& data, Byte position, const mcUUID& sender);
			SERVER_API static void clearTitles(Player*, bool reset);
			//SERVER_API static void tabComplete(Player*, varInt id, varInt start, varInt length, varInt count, );
			SERVER_API static void declareCommands(Player*);
			SERVER_API static void declareCommands(Player*, const std::vector<Commands::Node*>& commandGraphNodes, const Commands::RootNode& rootNode);
			SERVER_API static void closeWindow(Player*, Byte winId);
			SERVER_API static void windowItems(Player*, Byte winId, varInt stateId, varInt count, Slot** slots, Slot* carried);
			SERVER_API static void windowProperty(Player*, Byte winId, bshort property, bshort value);
			SERVER_API static void setSlot(Player*, Byte winId, varInt stateId, bshort slot, Slot* data);
			SERVER_API static void setCooldown(Player*, varInt itemId, varInt cooldown);
			SERVER_API static void pluginMessage(Player*, const mcString& channel, ull dataSize, const char* data);
			SERVER_API static void namedSoundEffect(Player*, const mcString& soundName, varInt category, bint effectX, bint effectY, bint effectZ, bfloat volume, bfloat pitch);
			SERVER_API static void disconnect(Player*, const Chat& reason);
			SERVER_API static void entityStatus(Player*, bint eid, Byte status);
			SERVER_API static void explosion(Player*, bfloat x, bfloat y, bfloat z, bfloat strength, varInt recordCount, Byte* records, bfloat playerMotionX, bfloat playerMotionY, bfloat playerMotionZ);
			SERVER_API static void unloadChunk(Player*, bint x, bint z);
			SERVER_API static void changeGameState(Player*, Byte reason, bfloat value);
			SERVER_API static void openHorseWindow(Player*, Byte winId, varInt slotCount, bint eid);
			SERVER_API static void initializeWorldBorder(Player*, bdouble x, bdouble z, bdouble oldDiameter, bdouble newDiameter, varLong speed, varInt portalTeleportBoundary, varInt warningBlocks, varInt warningTime);
			SERVER_API static void keepAlive(Player*, blong keepAlive_id);
			SERVER_API static void chunkDataAndLight(Player*, bint cX, bint cZ, const nbt_compound& heightMaps, varInt dataSize, char* chunkData, const std::vector<BlockEntity*>& blockEntities, bool trustEdges, const BitArray& skyLightMask, const BitArray& blockLightMask, const BitArray& emptySkyLightMask, const BitArray& emptyBlockLightMask, varInt skyLightArrayCount, BitArray** skyLightArrays, varInt blockLightArrayCount, BitArray** blockLightArrays);
			SERVER_API static void chunkDataAndLight(Player*, Chunk* chunk, bint cX, bint cZ);
			SERVER_API static void effect(Player*, bint effectId, const Position& location, bint data, bool disableRelativeVolume);
			SERVER_API static void particle(Player*, bint particleId, bool longDistance, bdouble x, bdouble y, bdouble z, bfloat offsetX, bfloat offsetY, bfloat offsetZ, bfloat particleData, bint count, particle::Particle* particle);
			//SERVER_API static void updateLight(Player*, varInt cX, varInt cZ, bool trustEdges, varInt length1, blong* skyLightMask, varInt length2, blong* blockLightMask, varInt length3, blong* emptySkyLightMask, varInt length4, blong* emptyBlockLightMask, varInt skyLightArrayCount, char** skyLightArrays, varInt blockLightArrayCount, char** blockLightArrays);
			SERVER_API static void updateLight(Player*, varInt cX, varInt cZ, bool trustEdges, const BitArray& skyLightMask, const BitArray& blockLightMask, const BitArray& emptySkyLightMask, const BitArray& emptyBlockLightMask, varInt skyLightArrayCount, BitArray** skyLightArrays, varInt blockLightArrayCount, BitArray** blockLightArrays);
			SERVER_API static void updateLight(Player*, varInt cX, varInt cZ);
			SERVER_API static void joinGame(Player*, bint Eid, bool isHardcore, gamemode gm, gamemode prev_gm, varInt worldCount, mcString* worldNames, const nbt_compound& dimensionCodec, const nbt_compound& dimension, const mcString& worldName, int64 hashedSeedHigh, varInt maxPlayers, varInt viewDistance, varInt simulationDistance, bool reducedDebugInfo, bool respawnScreen, bool isDebug, bool isFlat);
			SERVER_API static void mapData(Player*, varInt mapId, Byte scale, bool locked, bool trackingPosition, varInt iconCount, map::icon* icons, Byte optColumns, Byte optRows, Byte optX, Byte optZ, varInt optLength, Byte* data);
			SERVER_API static void tradeList(Player*, varInt winId, Byte size, trade* trades, varInt villagerLevel, varInt experience, bool isRegularVillager, bool canRestock);
			SERVER_API static void entityPosition(Player*, varInt eid, bshort deltaX, bshort deltaY, bshort deltaZ, bool onGround);
			SERVER_API static void entityPositionAndRotation(Player*, varInt eid, bshort deltaX, bshort deltaY, bshort deltaZ, Angle yaw, Angle pitch, bool onGround);
			SERVER_API static void entityRotation(Player*, varInt eid, Angle yaw, Angle pitch, bool onGround);
			SERVER_API static void vehicleMove(Player*, bdouble x, bdouble y, bdouble z, bfloat yaw, bfloat pitch);
			SERVER_API static void openBook(Player*, Hand whichHand);
			SERVER_API static void openWindow(Player*, varInt winId, window::type winType, const Chat& winTitle);
			SERVER_API static void openSignEditor(Player*, const Position& location);
			SERVER_API static void ping(Player*, bint pingId);
			SERVER_API static void craftRecipeResponse(Player*, Byte winId, const mcString& recipeIdentifier);
			SERVER_API static void playerAbilities(Player*, bool invulnerable, bool flying, bool allowFlying, bool creative, bfloat flyingSpeed, bfloat fovModifier);
			SERVER_API static void endCombatEvent(Player*, varInt duration, bint eid);
			SERVER_API static void enterCombatEvent(Player*);
			SERVER_API static void deathCombatEvent(Player*, varInt playerId, bint eid, const Chat& message);
			SERVER_API static void playerInfo(Player*, varInt action, varInt playerCount, Player**);
			SERVER_API static void facePlayer(Player*, varInt pivot, bdouble targetX, bdouble targetY, bdouble targetZ, bool isEntity, varInt eid, varInt targetPivot);
			SERVER_API static void playerPosAndLook(Player*, bdouble x, bdouble y, bdouble z, bfloat yaw, bfloat pitch, Byte flags, bool dismountVehicle);
			SERVER_API static void unlockRecipes(Player*, varInt action, bool bookOpen, bool filterActive, bool smeltingOpen, bool smeltingFilter, bool blastOpen, bool blastFilter, bool smokerOpen, bool smokerFilter, varInt size1, std::vector<mcString>* array1, varInt size2, std::vector<mcString>* array2);
			SERVER_API static void destroyEntity(Player*, varInt eid); //singular of destroyEntities
			SERVER_API static void destroyEntities(Player*, varInt count, varInt* eids);
			SERVER_API static void removeEntityEffect(Player*, varInt eid, varInt effectId);
			SERVER_API static void resourcePackSend(Player*, const mcString& url, const mcString& hash, bool forced, bool hasPromptMessage, const Chat& promptMessage);
			SERVER_API static void respawn(Player*, const nbt_compound& dimension, const mcString& worldName, blong hashedSeed, gamemode gm, gamemode prev_gm, bool isDebug, bool isFlat, bool copyMetadata);
			SERVER_API static void entityHeadLook(Player*, varInt eid, Angle headYaw);
			SERVER_API static void multiBlockChange(Player*, blong sectionPosition, bool unknown, varInt size, varLong* blocks);
			SERVER_API static void selectAdvancementTab(Player*, bool hasId, const mcString& id = "");
			SERVER_API static void actionBar(Player*, const Chat& actionBarText);
			SERVER_API static void worldBorderCenter(Player*, bdouble x, bdouble z);
			SERVER_API static void worldBorderLerpSize(Player*, bdouble oldDiameter, bdouble newDiameter, varLong speed);
			SERVER_API static void worldBorderSize(Player*, bdouble diameter);
			SERVER_API static void worldBorderWarningDelay(Player*, varInt warningTime);
			SERVER_API static void worldBorderWarningReach(Player*, varInt warningBlocks);
			SERVER_API static void camera(Player*, varInt camId);
			SERVER_API static void heldItemChange(Player*, Byte slot);
			SERVER_API static void updateViewPosition(Player*, varInt chunkX, varInt chunkZ);
			SERVER_API static void updateViewDistance(Player*, varInt distance);
			SERVER_API static void spawnPosition(Player*, const Position& location, bfloat angle);
			SERVER_API static void displayScoreboard(Player*, Byte position, const mcString& scoreName);
			SERVER_API static void entityMetadata(Player*, varInt eid, const std::vector<Entity::Metadata>& metadatas);
			SERVER_API static void entityMetadata(Player*, varInt eid, const Entity::Metadata& metadatas);
			SERVER_API static void attachEntity(Player*, bint attachedEid, bint holdingEid);
			SERVER_API static void entityVelocity(Player*, varInt eid, bshort velocityX, bshort velocityY, bshort velocityZ);
			SERVER_API static void entityEquipment(Player*, varInt eid, Equipment::Type type, Slot* item);
			SERVER_API static void entityEquipment(Player*, varInt eid, const std::vector<Equipment*>& equipments);
			SERVER_API static void setXp(Player*, bfloat xpBar, varInt level, varInt totalXp);
			SERVER_API static void updateHp(Player*, bfloat hp, varInt food, bfloat saturation);
			SERVER_API static void scoreboardObjective(Player*, const mcString& name, Byte mode, const Chat& value, varInt type);
			SERVER_API static void setPassengers(Player*, varInt vehicleEid, varInt count, varInt* passengers);
			SERVER_API static void teams(Player*, const mcString& name, Byte mode, teamsUpdate::mode* teamUpdateMode);
			SERVER_API static void updateScore(Player*, const mcString& name, Byte action, const mcString& objective, varInt value);
			SERVER_API static void updateSimulationDistance(Player*, varInt value);
			SERVER_API static void setTitleSubtitle(Player*, const Chat& subtitle);
			SERVER_API static void timeUpdate(Player*, blong worldAge, blong timeOfDay);
			SERVER_API static void setTitleText(Player*, const Chat& title);
			SERVER_API static void setTitleTimes(Player*, bint fadeIn, bint stay, bint fadeOut);
			SERVER_API static void entitySoundEffect(Player*, varInt soundId, varInt category, varInt eid, bfloat volume, bfloat pitch);
			SERVER_API static void soundEffect(Player*, varInt soundId, varInt category, bint effectPosX, bint effectPosY, bint effectPosZ, bfloat volume, bfloat pitch);
			SERVER_API static void stopSound(Player*, Byte flags, varInt source, const mcString& name);
			SERVER_API static void playerListHeaderAndFooter(Player*, const Chat& header, const Chat& footer);
			SERVER_API static void nbtQueryResponse(Player*, varInt transactionId, const nbt& tag);
			SERVER_API static void collectItem(Player*, varInt collectedEid, varInt collectorEid, varInt pickupCount);
			SERVER_API static void entityTeleport(Player*, varInt eid, bdouble x, bdouble y, bdouble z, Angle yaw, Angle pitch, bool onGround);
			SERVER_API static void advancements(Player*, bool reset, varInt mappingSize, mcString* advancementsIdentifiers, advancement** advancements, varInt listSize, mcString* removedIdentifiers, varInt progressSize, mcString* progressIdentifiers, advancementProgress** advancementProgresses);
			SERVER_API static void entityProperties(Player*, varInt eid, varInt nOfProperties, Entity::Property* properties);
			SERVER_API static void entityEffect(Player*, varInt entityId, varInt effectId, char amplifier, varInt duration, Byte flags);
			SERVER_API static void declareRecipes(Player*, varInt nOfRecipes, std::vector<recipe::Recipe*>* recipes);
			SERVER_API static void tags(Player*);
			SERVER_API static void tags(Player*, varInt tagCategoryCount, TagGroup* tags);
		};
		struct receive
		{
			SERVER_API static void teleportConfirm(Player*, varInt id);
			SERVER_API static void queryBlockNbt(Player*, varInt transactionId, const Position& location);
			SERVER_API static void setDifficulty(Player*, Byte difficulty);
			SERVER_API static void chatMessage(Player*, mcString& content);
			SERVER_API static void clientStatus(Player*, varInt actionId);
			SERVER_API static void clientSettings(Player*, const mcString& locale, Byte viewDistance, ChatMode chatMode, bool chatColors, Byte displayedSkinParts, varInt mainHand, bool enableTextFiltering, bool allowServerListings);
			SERVER_API static void tabComplete(Player*, varInt transactionId, const mcString& text);
			SERVER_API static void clickWindowButton(Player*, Byte windowID, Byte buttonID);
			SERVER_API static void clickWindow(Player*, Byte windowID, varInt stateID, bshort clickedSlot, Byte button, varInt mode, varInt length, bshort* slotNumbers, Slot** slots, Slot* clickedItem);
			SERVER_API static void closeWindow(Player*, Byte winId);
			SERVER_API static void pluginMessage(Player*, const mcString& channel, ull dataSize, char* data);
			SERVER_API static void editBook(Player*, varInt hand, varInt count, const std::vector<mcString>& pages, bool hasTitle, const mcString& title);
			SERVER_API static void queryEntityNbt(Player*, varInt transactionId, varInt eid);
			SERVER_API static void interactEntity(Player*, varInt eid, varInt type, bfloat targetX, bfloat targetY, bfloat targetZ, Hand mainHand, bool sneaking);
			SERVER_API static void generateStructure(Player*, const Position& location, varInt levels, bool keepJigsaw);
			SERVER_API static void keepAlive(Player*, blong keepAlive_id);
			SERVER_API static void lockDifficulty(Player*, bool locked);
			SERVER_API static void playerPosition(Player*, bdouble x, bdouble feetY, bdouble Z, bool onGround);
			SERVER_API static void playerPositionAndRotation(Player*, bdouble X, bdouble Y, bdouble Z, bfloat yaw, bfloat pitch, bool onGround);
			SERVER_API static void playerRotation(Player*, bfloat yaw, bfloat pitch, bool onGround);
			SERVER_API static void playerMovement(Player*, bool onGround);
			SERVER_API static void vehicleMove(Player*, bdouble x, bdouble y, bdouble z, bfloat yaw, bfloat pitch);
			SERVER_API static void steerBoat(Player*, bool leftPaddleTurning, bool rightPaddleTurning);
			SERVER_API static void pickItem(Player*, varInt slot);
			SERVER_API static void craftRecipeRequest(Player*, Byte winId, const mcString& recipe, bool makeAll);
			SERVER_API static void playerAbilities(Player*, Byte falgs);
			SERVER_API static void playerDigging(Player*, varInt status, const Position& location, Byte face);
			SERVER_API static void entityAction(Player*, varInt eid, varInt actionId, varInt jumpBoost);
			SERVER_API static void steerVehicle(Player*, bfloat sideways, bfloat forward);
			SERVER_API static void pong(Player*, bint id);
			SERVER_API static void setRecipeBookState(Player*, varInt bookid, bool open, bool filterActive);
			SERVER_API static void setDisplayedRecipe(Player*, const mcString& recipeId);
			SERVER_API static void nameItem(Player*, const mcString& newName);
			SERVER_API static void resourcePackStatus(Player*, varInt result);
			SERVER_API static void advancementTab(Player*, varInt action, const mcString& tabId);
			SERVER_API static void selectTrade(Player*, varInt selectedSlot);
			SERVER_API static void setBeaconEffect(Player*, varInt primary, varInt secondary);
			SERVER_API static void heldItemChange(Player*, bshort slot);
			SERVER_API static void updateCommandBlock(Player*, const Position& location, const mcString& command, varInt mode, Byte flags);
			SERVER_API static void updateCommandBlockMinecart(Player*, varInt eid, const mcString& command, bool trackOutput);
			SERVER_API static void creativeInventoryAction(Player*, bshort slot, Slot* clickedItem);
			SERVER_API static void updateJigsawBlock(Player*, const Position& location, const mcString& name, const mcString& target, const mcString& pool, const mcString& finalState, const mcString& jointType);
			SERVER_API static void updateStructureBlock(Player*, const Position& location, varInt action, varInt mode, const mcString& name, char offsetX, char offsetY, char offsetZ, Byte sizeX, Byte sizeY, Byte sizeZ, varInt mirror, varInt rotation, const mcString& metadata, bfloat integrity, varLong seed, Byte flags);
			SERVER_API static void updateSign(Player*, const Position& location, const mcString& line1, const mcString& line2, const mcString& line3, const mcString& line4);
			SERVER_API static void animation(Player*, varInt hand);
			SERVER_API static void spectate(Player*, const mcUUID& target);
			SERVER_API static void playerBlockPlacement(Player*, Hand hand, const Position& location, BlockFace face, bfloat cursorX, bfloat cursorY, bfloat cursorZ, bool insideBlock);
			SERVER_API static void useItem(Player*, Hand hand);
		};
	};

	//transform a scheduled chunk to raw data for sending
	//void message::translateChunk();
	//void message::translateLight();

	//prepare packet for sending (compress or append size, depending on the needs)
	SERVER_API static void preparePacket(Player*, char*& data, ull& size, char*& toDelete);
	//SERVER_API static void sendPacketData(Player*, char*, ull, bool disconnectAfter = false);

	//dispatch a message from a player
	SERVER_API static void dispatch(Player*, char* data, uint compressedSize, uint decompressedSize);
	SERVER_API static void dispatch(Player*, char* data, uint size);
};