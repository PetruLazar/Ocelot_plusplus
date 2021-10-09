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
	void tellPing(CommandHandlerArguments)
	{
		message::play::send::chatMessage(executingPlayer, Chat(("Your ping is " + std::to_string((int&)executingPlayer->ping)).c_str()), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
	}
	void test(CommandHandlerArguments)
	{
		switch (*(int*)argumentStack[0])
		{
			//add tests here
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