#pragma once
#include "player.h"
#include "../types/node.h"

namespace CommandHandlers
{
	void gamemodeSurvival(CommandHandlerArguments);
	void gamemodeCreative(CommandHandlerArguments);
	void gamemodeSpectator(CommandHandlerArguments);
	void gamemodeAdventure(CommandHandlerArguments);
	void worldChange(CommandHandlerArguments);
	void tellWorld(CommandHandlerArguments);
	void tellPing(CommandHandlerArguments);
	void test(CommandHandlerArguments);
	void teleport(CommandHandlerArguments);
	void spawn(CommandHandlerArguments);
	void setSpawn(CommandHandlerArguments);
	void stopServer(CommandHandlerArguments);
}

namespace SuggestionsHandler
{
	void world();
}

namespace Commands
{
	bool dispatch(Player* executingPlayer, mcString&, Node*, ArgumentStack&);
	SERVER_API bool dispatch(Player* executingPlayer, mcString& command);
}