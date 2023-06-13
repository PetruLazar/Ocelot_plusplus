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
#include "../command.h"

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
void message::play::send::blockEntityData(Player* p, const Position& location, varInt blockEntityType, const nbt& blockData)
{
	varInt id = (int)id::blockEntityData;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	location.write(data);
	blockEntityType.write(data);
	blockData.write(data);

	finishSendMacro;
}
void message::play::send::blockEntityData(Player* p, BlockEntity* blockEntity)
{
	varInt id = (int)id::blockEntityData;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	blockEntity->writeExplicit(data);

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
void message::play::send::chunkDataAndLight(Player* p, bint cX, bint cZ, const nbt_compound& heightMaps, varInt dataSize, char* chunkData, const std::vector<BlockEntity*>& blockEntities,
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
	varInt(0).write(data);

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

	for (auto& blockEntity : blockEntities)
	{
		blockEntityData(p, blockEntity);
	}
}
void message::play::send::chunkDataAndLight(Player* p, Chunk* chunk, bint cX, bint cZ)
{
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
		section.blockStates.write(chunkData);
		section.biomes.write(chunkData);
	}
	uint dataSize = (uint)(chunkData - chunkDataStart);

	//change value to be ready for light sections
	sectionCount += 2;
	std::vector<BitArray*> skyLightArrays, blockLightArrays;
	for (uint i = 0; i < sectionCount; i++)
	{
		if (chunk->skyLightMask->getElement(i)) skyLightArrays.emplace_back(chunk->lightSections[i].skyLight);
		if (chunk->blockLightMask->getElement(i)) blockLightArrays.emplace_back(chunk->lightSections[i].blockLight);
	}

	chunkDataAndLight(p, cX, cZ, heightMaps, dataSize, chunkDataStart, chunk->blockEntities, true, *chunk->skyLightMask, *chunk->blockLightMask, *chunk->emptySkyLightMask, *chunk->emptyBlockLightMask, (uint)skyLightArrays.size(), skyLightArrays.data(), (uint)blockLightArrays.size(), blockLightArrays.data());
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
void message::play::send::entityMetadata(Player* p, varInt eid, const Entity::Metadata& metadata)
{
	varInt id = (int)id::entityMetadata;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);

	metadata.write(data);

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
void message::play::send::entityEquipment(Player* p, varInt eid, Equipment::Type type, Slot* item)
{
	varInt id = (int)id::entityEquipment;
	prepareSendMacro(1024 * 1024);

	id.write(data);
	eid.write(data);

	Byte typeByte = static_cast<Byte>(type);
	typeByte &= ~(0x80);

	*(data++) = typeByte;
	item->write(data);

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
	uint sectionCount = (uint)chunk->lightSections.size();

	std::vector<BitArray*> skyLightArrays, blockLightArrays;
	for (uint i = 0; i < sectionCount; i++)
	{
		if (chunk->skyLightMask->getElement(i)) skyLightArrays.emplace_back(chunk->lightSections[i].skyLight);
		if (chunk->blockLightMask->getElement(i)) blockLightArrays.emplace_back(chunk->lightSections[i].blockLight);
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
void message::play::send::destroyEntity(Player* p, varInt eid)
{
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
