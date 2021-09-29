#include "command.h"
#include "../types/enums.h"
#include "message.h"
#include "../types/chat.h"

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
		if (comm == "/world experimental")
		{
			if (p->world != World::worlds[2])
			{
				p->changeWorld(World::worlds[2]);
				return;
			}
			message::play::send::chatMessage(p, Chat("Ba cplm, tu nu vezi ca deja esti in lumea aia?"), 1, mcUUID(0, 0, 0, 0));
			return;
		}
		if (comm == "/world custom")
		{
			if (p->world != World::worlds[3])
			{
				p->changeWorld(World::worlds[3]);
				return;
			}
			message::play::send::chatMessage(p, Chat("Ba cplm, tu nu vezi ca deja esti in lumea aia?"), 1, mcUUID(0, 0, 0, 0));
			return;
		}
		if (comm == "/world")
		{
			message::play::send::chatMessage(p, Chat(("You are in world \\\"" + p->world->name + "\\\"").c_str()), 1, mcUUID(0, 0, 0, 0));
			return;
		}
		if (comm == "/ping")
		{
			message::play::send::chatMessage(p, Chat(("Your ping is " + std::to_string((int)p->ping)).c_str()), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
			return;
		}
		if (comm == "/test")
		{
			//EntityProperty* props = new EntityProperty("uab.domsa", 40);
			//message::play::send::entityProperties(p, p->eid, 1, props);
			//message::play::send::updateHp(p, 0, 20, 0.f);
			//delete props;
			Player* temp = new Player(new sf::TcpSocket);
			temp->uuid = new mcUUID(mcUUID::type::player);
			temp->username = "Dna Ma-ta";
			temp->gm = gamemode::creative;
			temp->ping = -1;
			temp->hasDisplayName = false;
			message::play::send::playerInfo(p, playerInfo::addPlayer, 1, &temp);
			message::play::send::spawnPlayer(p, 0x27, *temp->uuid, p->X, p->Y + 2, p->Z, (float)p->yaw, (float)p->pitch);
			message::play::send::entityHeadLook(p, 0x27, (double)p->yaw);
			delete temp;
			return;
		}
		message::play::send::chatMessage(p, Chat("Ba da atata esti de prost de nici nu stii sa scrii o comanda...", Chat::color::red), 1, mcUUID(0, 0, 0, 0));
	}
}
