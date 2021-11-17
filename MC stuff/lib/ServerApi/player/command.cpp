#include "command.h"
#include "player.h"
#include "message.h"
#include "../server/log.h"

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
				y = fastfloor((double)executingPlayer->Y) - 1,
				z = fastfloor((double)executingPlayer->Z);

			y = executingPlayer->world->AbsToRelHeight(y);
			if (!executingPlayer->world->checkCoordinates(y)) throw Chat("Cannot place blocks outside world", Chat::color::red);
			executingPlayer->world->setBlock(x, y, z, 1);
			message::play::send::sendFullChunk(executingPlayer, executingPlayer->chunkX, executingPlayer->chunkZ, false);
			message::play::send::chatMessage(executingPlayer, Chat("Done"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		}
		break;
		case 2:
		{
			static int lvl = 0;

			int x = fastfloor((double)executingPlayer->X),
				y = fastfloor((double)executingPlayer->Y),
				z = fastfloor((double)executingPlayer->Z);

			y = executingPlayer->world->AbsToRelHeight(y);
			if (!executingPlayer->world->checkCoordinates(y)) throw Chat("Cannot place blocks outside world", Chat::color::red);

			executingPlayer->world->setBlock(x, y, z, 34 + lvl);
			message::play::send::sendFullChunk(executingPlayer, executingPlayer->chunkX, executingPlayer->chunkZ, false);
			message::play::send::chatMessage(executingPlayer, Chat(("Done - " + std::to_string(lvl++)).c_str()), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
			if (lvl == 16) lvl = 0;
		}
		break;
		case 3:
		{
			executingPlayer->teleport(executingPlayer->X, executingPlayer->Y, executingPlayer->Z, 45.f, executingPlayer->pitch);
			message::play::send::chatMessage(executingPlayer, Chat("Done"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
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
		bool ret = dispatch(executingPlayer, command, &Commands::root, argumentStack);
		for (void* ptr : argumentStack) delete ptr;
		return ret;
	}
}