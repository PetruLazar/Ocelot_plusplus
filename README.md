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
	login::receive::encryptionResponse
	login::receive::loginPluginResponse

	play::send::statistics
	play::send::acknowledgePlayerDigging
	play::send::blockBreakAnimation
	play::send::blockAction
	play::send::blockChange
	play::send::tabComplete
	play::send::namedSoundEffect
	play::send::explosion
	play::send::initializeWorldBorder
	play::send::vehicleMove
	play::send::endCombatEvent
	play::send::enterCombatEvent
	play::send::deathCombatEvent
	play::send::faceplayer
	play::send::multiBLockChange
	play::send::worldBorderCenter
	play::send::worldBorderLerpSize
	play::send::worldBorderSize
	play::send::worldBorderWarningDelay
	play::send::worldBorderWarningReach
	play::send::entityMetadata
	play::send::attachEntity
	play::send::setPassengers
	play::send::entitySoundEffect
	play::send::soundEffect
	play::send::stopSound
	play::send::nbtQueryResponse

	play::receive::queryBlockNbt
	play::receive::setDifficulty
	play::receive::tabComplete
	play::receive::clickWindowButton
	play::receive::clickWindow
	play::receive::pluginMessage
	play::receive::editBook
	play::receive::queryEntityNbt
	play::receive::generateStructure
	play::receive::lockDifficulty
	play::receive::playerMovement
	play::receive::vehicleMove
	play::receive::steerBoat
	play::receive::pickItem
	play::receive::playerAbilities
	play::receive::playerDigging
	play::receive::entityAction
	play::receive::steerVehicle
	play::receive::pong
	play::receive::setRecipeBookState
	play::receive::setDisplayedRecipe
	play::receive::nameItem
	play::receive::advancementTab
	play::receive::selectTrade
	play::receive::setBeaconEffect
	play::receive::updateCommandBlock
	play::receive::updateCommandBlockMinecart
	play::receive::updateJigsawBlock
	play::receive::updateStructureBlock
	play::receive::updateSign
	play::receive::spectate
	play::receive::playerBlockPlacement
	play::receive::useItem
