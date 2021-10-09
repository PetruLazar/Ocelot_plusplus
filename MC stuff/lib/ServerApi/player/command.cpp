#include "command.h"
#include "player.h"
#include "message.h"

namespace CommandHandlers
{
	void gamemodeSurvival(CommandHandlerArguments)
	{
		executingPlayer->gm = gamemode::survival;
		message::play::send::changeGameState(executingPlayer, ChangeGameState::changeGamemode, (char)gamemode::survival);
	}
	void gamemodeCreative(CommandHandlerArguments)
	{
		executingPlayer->gm = gamemode::creative;
		message::play::send::changeGameState(executingPlayer, ChangeGameState::changeGamemode, (char)gamemode::creative);
	}
	void gamemodeSpectator(CommandHandlerArguments)
	{
		executingPlayer->gm = gamemode::spectator;
		message::play::send::changeGameState(executingPlayer, ChangeGameState::changeGamemode, (char)gamemode::spectator);
	}
	void gamemodeAdventure(CommandHandlerArguments)
	{
		executingPlayer->gm = gamemode::adventure;
		message::play::send::changeGameState(executingPlayer, ChangeGameState::changeGamemode, (char)gamemode::adventure);
	}
	void worldChange(CommandHandlerArguments)
	{
		mcString& arg0 = *(mcString*)argumentStack[0];
		if (executingPlayer->world->name == arg0) message::play::send::chatMessage(executingPlayer, Chat("You already are in that world", Chat::color::red), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
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