## To-do
- Encryption support
- Commands - implement suggestions
- Console - implement command input and show/hide functionality
- load advancements (incomplete)
- finish recipes system (incomplete)
- pickup items (incomplete)

Not implemented yet:
- mobs (they just stay in one place)

Known bugs:
- When placing a bucket of water on an already waterlogged block, the server tries to waterlog the block again instead of trating it like a solid block

Futures about worlds:
- Arrange regions in order in a world, and implement binary search
- Save regions/chunks to file when needed
- allow custom generators to add new biomes
- overhaul code optimization
- block updates
- block entities
- Increase code efficiency for world generation/load/unload/etc.

Features about players:
- check for moving too fast
- store player info into a file on quit (and every now and then), and read that info when rejoining, if available
- when changing world, resend the items if needed
- control player sightings while moving

Code related to the following packets (either implement the packet itself or code to ease the use of the packet or other infrastructure):

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
	play::receive::vehicleMove
	play::receive::steerBoat
	play::receive::pickItem
	play::receive::playerAbilities
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
