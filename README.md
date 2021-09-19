# Minecraft-server
Minecraft server implemented in c++

# Documentation
https://wiki.vg/Protocol

# To-do
class for 'Slot'

class for 'Chat'

classes for world/region/chunk (in progress)

	- Arrange regions in order in a world, and implement binary search

	- Save regions/chunks to file when needed

Increase code efficiency for world generation/load/unload/etc.

Better commands system

Better Tags system

Split logging into log.txt (normal log) and debug.txt (for protocol/runtime warnings and error)

finish message.h : statistics::custom and create header/source files for it

class for Identifier?

code for working with UUID

code for working with Entity metadata

command processing system

fill out contents for following tags:

Blocks:

	minecraft:enderman_holdable
	minecraft:soul_fire_base_blocks
	minecraft:infiniburn_nether
	minecraft:flower_pots
	minecraft:infiniburn_overworld,
	minecraft:wooden_fences
	minecraft:piglin_repellents
	minecraft:wall_post_override
	minecraft:wooden_slabs
	minecraft:portals
	minecraft:occludes_vibration_signals,
	minecraft:small_flowers
	minecraft:wooden_trapdoors
	minecraft:pressure_plates
	minecraft:jungle_logs
	minecraft:wooden_stairs
	minecraft:spruce_logs
	minecraft:signs
	minecraft:wool
	minecraft:stairs
	minecraft:wither_summon_base_blocks
	minecraft:logs
	minecraft:dripstone_replaceable_blocks
	minecraft:stone_bricks
	minecraft:hoglin_repellents
	minecraft:fire
	minecraft:lava_pool_stone_replaceables
	minecraft:ice
	minecraft:needs_diamond_tool
	minecraft:dragon_immune
	minecraft:wall_signs
	minecraft:features_cannot_replace
	minecraft:slabs
	minecraft:valid_spawn
	minecraft:mushroom_grow_block,
	minecraft:guarded_by_piglins
	minecraft:wooden_doors
	minecraft:warped_stems
	minecraft:standing_signs
	minecraft:infiniburn_end
	minecraft:emerald_ores,
	minecraft:stone_ore_replaceables
	minecraft:trapdoors
	minecraft:redstone_ores
	minecraft:needs_stone_tool
	minecraft:inside_step_sound_blocks
	minecraft:flowers
	minecraft:prevent_mob_spawning_inside
	minecraft:wart_blocks
	minecraft:planks
	minecraft:soul_speed_blocks
	minecraft:geode_invalid_blocks,
	minecraft:non_flammable_wood
	minecraft:leaves
	minecraft:walls
	minecraft:strider_warm_blocks
	minecraft:fence_gates
	minecraft:shulker_boxes
	minecraft:bee_growables
	minecraft:wooden_pressure_plates
	minecraft:wither_immune
	minecraft:tall_flowers
	minecraft:lapis_ores
	minecraft:moss_replaceable
	minecraft:wall_corals
	minecraft:underwater_bonemeals
	minecraft:stone_pressure_plates
	minecraft:lush_ground_replaceable
	minecraft:impermeable
	minecraft:nylium
	minecraft:snow
	minecraft:gold_ores
	minecraft:small_dripleaf_placeable
	minecraft:fences
	minecraft:logs_that_burn
	minecraft:saplings
	minecraft:mineable / hoe,
	minecraft:iron_ores
	minecraft:oak_logs
	minecraft:unstable_bottom_center
	minecraft:doors
	minecraft:needs_iron_tool

code related to the following packets (either implement the packet itself or code to ease the use of the packet or other infrastructure):

	handshake::receive::legacy
	login::send::encryptionRequest
	login::send::loginPluginRequest
	login::receive::start
	login::receive::encryptionResponse
	login::receive::loginPluginResponse
	play::send::spawnEntity
	play::send::spawnPainting
	play::send::sculkVibrationSignal
	play::send::entityAnimation
	play::send::statistics
	play::send::acknowledge	playerDigging
	play::send::blockBreakAnimation
	play::send::blockEntityData
	play::send::blockAction
	play::send::blockChange
	play::send::bossBar
	play::send::serverDifficulty
	play::send::chatMessage
	play::send::clearTitles
	play::send::tabComplete
	play::send::declareCommands
	play::send::closeWindow
	play::send::windowItems
	play::send::windowProperty
	play::send::setSlot
	play::send::setCooldown
	play::send::pluginMessage
	play::send::namedSoundEffect
	play::send::disconnect
	play::send::entityStatus
	play::send::explosion
	play::send::unloadChunk
	play::send::changeGameState
	play::send::openHorseWindow
	play::send::initializeWorldBorder
	play::send::chunkData
	play::send::effect
	play::send::particle
	play::send::updateLight
	play::send::joinGame
	play::send::mapData
	play::send::tradeList
	play::send::entityPosition
	play::send::entitypositionAndRotation
	play::send::entityRotation
	play::send::vehicleMove
	play::send::openBook
	play::send::openWindow
	play::send::openSignEditor
	play::send::ping
	play::send::craftRecipeResponse
	play::send::playerAbilities
	play::send::endCombatEvent
	play::send::enterCombatEvent
	play::send::deathCombatEvent
	play::send::playerInfo
	play::send::faceplayer
	play::send::playerPosAndLook
	play::send::unlockRecipes
	play::send::destroyEntities
	play::send::removeEntityEffect
	play::send::resourcePackSend
	play::send::respawn
	play::send::entityHeadLook
	play::send::multiBLockChange
	play::send::selectAdvancementTab
	play::send::actionBar
	play::send::worldBorderCenter
	play::send::worldBorderLerpSize
	play::send::worldBorderSize
	play::send::worldBorderWarningDelay
	play::send::worldBorderWarningReach
	play::send::camera
	play::send::heldItemChange
	play::send::updateViewPosition
	play::send::updateViewDistance
	play::send::spawnPosition
	play::send::displayScoreboard
	play::send::entityMetadata
	play::send::attachEntity
	play::send::entityVelocity
	play::send::entityEquipment
	play::send::setXp
	play::send::updateHp
	play::send::scoreboardObjective
	play::send::setPassengers
	play::send::teams
	play::send::updateScore
	play::send::setTitleSubtitle
	play::send::timeUpdate
	play::send::setTitleText
	play::send::setTitleTimes
	play::send::entitySoundEffect
	play::send::soundEffect
	play::send::stopSound
	play::send::playerListHeaderAndFooter
	play::send::nbtQueryResponse
	play::send::collectItem
	play::send::entityTeleport
	play::send::advancements
	play::send::entityProperties
	play::send::entityEffect
	play::send::declareRecipes
	play::send::tags
	play::receive::teleportConfirm
	play::receive::queryBlockNbt
	play::receive::setDifficulty
	play::receive::chatMessage
	play::receive::clientStatus
	play::receive::clientSettings
	play::receive::tabComplete
	play::receive::clickWindowButton
	play::receive::clickWindow
	play::receive::closeWindow
	play::receive::pluginMessage
	play::receive::editBook
	play::receive::queryEntityNbt
	play::receive::interactEntity
	play::receive::generateStructure
	play::receive::lockDifficulty
	play::receive::playerPosition
	play::receive::playerPositionAndRotation
	play::receive::playerRotation
	play::receive::playerMovement
	play::receive::vehicleMove
	play::receive::steerBoat
	play::receive::pickItem
	play::receive::craftRecipeRequest
	play::receive::playerAbilities
	play::receive::playerDigging
	play::receive::entityAction
	play::receive::steerVehicle
	play::receive::pong
	play::receive::setRecipeBookState
	play::receive::setDisplayedRecipe
	play::receive::nameItem
	play::receive::resourcePack	status
	play::receive::advancementTab
	play::receive::selectTrade
	play::receive::setBeaconEffect
	play::receive::heldItemChange
	play::receive::updateCommandBlock
	play::receive::updateCommandBlockMinecart
	play::receive::creativeInventoryAction
	play::receive::updateJigsawBlock
	play::receive::updateStructureBlock
	play::receive::updateSign
	play::receive::animation
	play::receive::spectate
	play::receive::playerBlockPlacement
	play::receive::useItem
