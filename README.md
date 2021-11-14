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

Better exceptions system - based on std::exception

System for recipes

Better system for biomes, so that custom generators can have custom biomes

Split logging into log.txt (normal log) and debug.txt (for protocol/runtime warnings and error)

code related to the following packets (either implement the packet itself or code to ease the use of the packet or other infrastructure):

	handshake::receive::legacy
	login::send::encryptionRequest
	login::send::loginPluginRequest
	login::receive::encryptionResponse
	login::receive::loginPluginResponse

	play::send::statistics
	play::send::tabComplete

	play::receive::queryBlockNbt
	play::receive::setDifficulty
	play::receive::tabComplete
	play::receive::clickWindowButton
	play::receive::clickWindow
	play::receive::pluginMessage
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
