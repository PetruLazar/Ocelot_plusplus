#include "command.h"
#include "player.h"
#include "message.h"

namespace CommandHandlers
{
	void gamemodeChangeFooter(CommandHandlerArguments)
	{
		message::play::send::changeGameState(executingPlayer, ChangeGameState::changeGamemode, (char)executingPlayer->gm);
		broadcastMessage(message::play::send::playerInfo(player_macro, playerInfo::updateGamemode, 1, &executingPlayer)); //maybe other players' tab too?
	}
	void gamemodeSurvival(CommandHandlerArguments)
	{
		executingPlayer->gm = gamemode::survival;
		gamemodeChangeFooter(executingPlayer, argumentStack);
	}
	void gamemodeCreative(CommandHandlerArguments)
	{
		executingPlayer->gm = gamemode::creative;
		gamemodeChangeFooter(executingPlayer, argumentStack);
	}
	void gamemodeSpectator(CommandHandlerArguments)
	{
		executingPlayer->gm = gamemode::spectator;
		gamemodeChangeFooter(executingPlayer, argumentStack);
	}
	void gamemodeAdventure(CommandHandlerArguments)
	{
		executingPlayer->gm = gamemode::adventure;
		gamemodeChangeFooter(executingPlayer, argumentStack);
	}
	void worldChange(CommandHandlerArguments)
	{
		mcString& arg0 = *(mcString*)argumentStack[0];
		if (executingPlayer->world->name == arg0)
		{
			message::play::send::chatMessage(executingPlayer, Chat("You already are in that world", Chat::color::red), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
			return;
		}
		for (World* wld : World::worlds)
		{
			if (arg0 == wld->name)
			{
				executingPlayer->changeWorld(wld);
				return;
			}
		}
		message::play::send::chatMessage(executingPlayer, Chat("World not found", Chat::color::red), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
	}
	void tellWorld(CommandHandlerArguments)
	{
		message::play::send::chatMessage(executingPlayer, Chat(("You are in world \\\"" + executingPlayer->world->name + "\\\"").c_str()), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
	}
	void tellPing(CommandHandlerArguments)
	{
		message::play::send::chatMessage(executingPlayer, Chat(("Your ping is " + std::to_string((int&)executingPlayer->ping)).c_str()), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
	}
	void test(CommandHandlerArguments)
	{
		switch (*(int*)argumentStack[0])
		{
		case 1:
		{
			int x = fastfloor((double)executingPlayer->X),
				y = fastfloor((double)executingPlayer->Y) - 1 - executingPlayer->world->min_y,
				z = fastfloor((double)executingPlayer->Z),
				cx = x >> 4,
				cz = z >> 4;
			x &= 0xf; z &= 0xf;

			int cy = y >> 4; y &= 0xf;
			Section& section = executingPlayer->world->get(cx, cz)->sections[cy];
			for (int i = 0; i < 16; i++) for (int j = 0; j < 15; j++) section.setBlock(i, y, j, Registry::getBlockState(rand() % 10000));

			//Chunk* chunk = executingPlayer->world->get(cx, cz);
			//chunk->setBlock(x, y, z, Registry::getBlockState(rand() % 10000));
			message::play::send::sendFullChunk(executingPlayer, cx, cz, false);
			message::play::send::chatMessage(executingPlayer, Chat("Done"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		}
		break;
		case 2:
		{
			int x = fastfloor((double)executingPlayer->X),
				y = fastfloor((double)executingPlayer->Y) - 1 - executingPlayer->world->min_y,
				z = fastfloor((double)executingPlayer->Z),
				cx = x >> 4,
				cz = z >> 4;
			x &= 0xf; z &= 0xf;

			//int cy = y >> 4; y &= 0xf;
			//Section& section = executingPlayer->world->get(cx, cz)->sections[cy];
			//for (int i = 0; i < 16; i++) for (int j = 0; j < 15; j++) section.setBlock(i, y, j, Registry::getBlockState(rand() % 10000));

			Chunk* chunk = executingPlayer->world->get(cx, cz);
			chunk->setBlock(x, y, z, Registry::getBlockState(rand() % 10000));
			message::play::send::sendFullChunk(executingPlayer, cx, cz, false);
			message::play::send::chatMessage(executingPlayer, Chat("Done"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		}
		break;
		case 3:
		{
			int x = fastfloor((double)executingPlayer->X),
				y = fastfloor((double)executingPlayer->Y) - 1 - executingPlayer->world->min_y,
				z = fastfloor((double)executingPlayer->Z),
				cx = x >> 4,
				cz = z >> 4;
			x &= 0xf; z &= 0xf;

			int cy = y >> 4; y &= 0xf;
			Section& section = executingPlayer->world->get(cx, cz)->sections[cy];
			
			if (section.useGlobalPallete) message::play::send::chatMessage(executingPlayer, Chat("global palette"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
			else message::play::send::chatMessage(executingPlayer, Chat("local palette"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		}
		break;
		//add tests here - starting at 1
		default:
			message::play::send::chatMessage(executingPlayer, Chat("Invalid test", Chat::color::red), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		}
	}
}

namespace Commands
{
	bool dispatch(Player* executingPlayer, mcString& command, Node* currentNode, ArgumentStack& argumentStack)
	{
		//if command is empty, try to run command
		if (command.empty())
		{
			//if (currentNode)
			try
			{
				currentNode->execute(executingPlayer, argumentStack);
				return true;
			}
			catch (...) { throw; }
		}

		for (varInt child : currentNode->children)
		{
			Node* nextNode = Commands::commands[child];
			if (nextNode->extract(command, argumentStack)) return dispatch(executingPlayer, command, nextNode, argumentStack);
		}
		return false;
	}

	bool dispatch(Player* executingPlayer, mcString& command)
	{
		ArgumentStack argumentStack;
		return dispatch(executingPlayer, command, &Commands::root, argumentStack);
	}
}