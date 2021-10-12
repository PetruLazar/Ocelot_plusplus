# Minecraft-server
Minecraft server implemented in c++

# Documentation
https://wiki.vg/Protocol

# To-do
class for 'Chat'

classes for world/region/chunk (in progress)

	- Arrange regions in order in a world, and implement binary search

	- Save regions/chunks to file when needed

Increase code efficiency for world generation/load/unload/etc.

System for assigning entity ids

Better commands system

Better exceptions system - based on std::exception

System for recipes

Better system for biomes, so that custom generators can have custom biomes

Fix spelling mistake in Section: pallete -> palette

Split logging into log.txt (normal log) and debug.txt (for protocol/runtime warnings and error)

finish message.h : statistics::custom and create header/source files for it

class for Identifier?

code for working with UUID

code for working with Entity metadata

command processing system

code related to the following packets (either implement the packet itself or code to ease the use of the packet or other infrastructure):

	handshake::receive::legacy
	login::send::encryptionRequest
	login::send::loginPluginRequest
	login::receive::start
	login::receive::encryptionResponse
	login::receive::loginPluginResponse
	play::send::spawnEntity
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
