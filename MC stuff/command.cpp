#include "command.h"
#include "player.h"
#include "message.h"

namespace Command
{
	void parse(Player* p, char* command)
	{
		//temporary implementation
		std::string comm = command;
		if (comm == "/gamemode survival")
		{
			p->gm = gamemode::survival;
			message::play::send::changeGameState(p, ChangeGameState::changeGamemode, (char)gamemode::survival);
			return;
		}
		if (comm == "/gamemode creative")
		{
			p->gm = gamemode::creative;
			message::play::send::changeGameState(p, ChangeGameState::changeGamemode, (char)gamemode::creative);
			return;
		}
		if (comm == "/gamemode adventure")
		{
			p->gm = gamemode::adventure;
			message::play::send::changeGameState(p, ChangeGameState::changeGamemode, (char)gamemode::adventure);
			return;
		}
		if (comm == "/gamemode spectator")
		{
			p->gm = gamemode::spectator;
			message::play::send::changeGameState(p, ChangeGameState::changeGamemode, (char)gamemode::spectator);
			return;
		}
		if (comm == "/world world")
		{
			if (p->world != World::worlds[0])
			{
				p->changeWorld(World::worlds[0]);
				return;
			}
			message::play::send::chatMessage(p, Chat("Ba cplm, tu nu vezi ca deja esti in lumea aia?"), 1, mcUUID(0, 0, 0, 0));
			return;
		}
		if (comm == "/world old")
		{
			if (p->world != World::worlds[1])
			{
				p->changeWorld(World::worlds[1]);
				return;
			}
			message::play::send::chatMessage(p, Chat("Ba cplm, tu nu vezi ca deja esti in lumea aia?"), 1, mcUUID(0, 0, 0, 0));
			return;
		}
		if (comm == "/world")
		{
			message::play::send::chatMessage(p, Chat(("You are in world \"" + p->world->name + "\"").c_str()), 1, mcUUID(0, 0, 0, 0));
			return;
		}
		if (comm == "/test")
		{
			//unused
		}
		message::play::send::chatMessage(p, Chat("Ba da atata esti de prost de nici nu stii sa scrii o comanda...", Chat::red), 1, mcUUID(0, 0, 0, 0));
	}
}
