#pragma once
#include "player.h"

#define CommandHandlerArguments Player* executingPlayer, const std::vector<mcString>& argumentStack

namespace CommandHandlers
{
	void gamemodeSurvival(CommandHandlerArguments);
	void gamemodeCreative(CommandHandlerArguments);
	void gamemodeSpectator(CommandHandlerArguments);
	void gamemodeAdventure(CommandHandlerArguments);
	void worldChange(CommandHandlerArguments);
}