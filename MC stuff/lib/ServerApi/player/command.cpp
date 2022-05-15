#include "command.h"
#include "message.h"
#include "../debug/log.h"
#include "../server/server.h"

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
			message::play::send::chatMessage(executingPlayer, Chat("You already are in that world", Chat::color::red()), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
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
		message::play::send::chatMessage(executingPlayer, Chat("World not found", Chat::color::red()), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
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
			int x = fastfloor((double)executingPlayer->x),
				y = fastfloor((double)executingPlayer->y) - 1,
				z = fastfloor((double)executingPlayer->z);

			y = executingPlayer->world->AbsToRelHeight(y);
			if (!executingPlayer->world->checkCoordinates(y)) throw Chat("Cannot place blocks outside world", Chat::color::red());
			executingPlayer->world->setBlock(x, y, z, 1);
			message::play::send::chatMessage(executingPlayer, Chat("Done"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		}
		break;
		case 2:
		{
			static int lvl = 0;

			int x = fastfloor((double)executingPlayer->x),
				y = fastfloor((double)executingPlayer->y),
				z = fastfloor((double)executingPlayer->z);

			y = executingPlayer->world->AbsToRelHeight(y);
			if (!executingPlayer->world->checkCoordinates(y)) throw Chat("Cannot place blocks outside world", Chat::color::red());

			executingPlayer->world->setBlock(x, y, z, 34 + lvl);
			message::play::send::chatMessage(executingPlayer, Chat(("Done - " + std::to_string(lvl++)).c_str()), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
			if (lvl == 16) lvl = 0;
		}
		break;
		case 3:
		{
			executingPlayer->teleport(executingPlayer->x, executingPlayer->y, executingPlayer->z, 45.f, executingPlayer->pitch);
			message::play::send::chatMessage(executingPlayer, Chat("Done"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		}
		break;
		case 4:
		{
			World* wld = executingPlayer->world;

			int x = fastfloor((double)executingPlayer->x),
				y = fastfloor((double)executingPlayer->y) - 1,
				z = fastfloor((double)executingPlayer->z);

			int yloc = y;
			y = wld->AbsToRelHeight(y);
			if (!wld->checkCoordinates(y)) throw Chat("Cannot place blocks outside world", Chat::color::red());
			for (int blockid = 0; blockid < 898; blockid++)
			{
				int row = (blockid / 30 - 14) * 3,
					col = (blockid % 30 - 14) * 3;
				BlockState state = &Registry::getBlockState(Registry::getName(Registry::blockRegistry, blockid));
				wld->setBlock(x + row, y, z + col, state);
			}
			//for (int i = -5; i <= 5; i++) for (int j = -5; j <= 5; j++) message::play::send::sendFullChunk(executingPlayer, executingPlayer->chunkX + i, executingPlayer->chunkZ + j, false);
			message::play::send::chatMessage(executingPlayer, Chat("Done"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		}
		break;
		case 5:
		{
			World* wld = executingPlayer->world;

			int x = fastfloor((double)executingPlayer->x),
				y = fastfloor((double)executingPlayer->y) - 1,
				z = fastfloor((double)executingPlayer->z);

			int yloc = y;
			y = wld->AbsToRelHeight(y);
			if (!wld->checkCoordinates(y)) throw Chat("Cannot place blocks outside world", Chat::color::red());
			for (int blockid = 0; blockid < 898; blockid++)
			{
				int row = (blockid / 30 - 14) * 3,
					col = (blockid % 30 - 14) * 3;
				wld->setBlock(x + row, y, z + col, 1);
			}
			message::play::send::chatMessage(executingPlayer, Chat("Done"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		}
		break;
		case 6:
		{
			Log::info() << Log::endl << "----------INVENTORY DUMP----------" << Log::endl;
			Log::info() << "Crafting: " << Log::endl;
			for (unsigned i = 0; i < 5; i++)
				Log::none() << "\t" << i << ": " << executingPlayer->inventory->getSlotByIndex(i)->getItemId()<<"-" << (int)executingPlayer->inventory->getSlotByIndex(i)->count;
			Log::none() << Log::endl;
			Log::info() << "Armor: " << Log::endl;
			for (unsigned i = 5; i < 9; i++)
				Log::none() << "\t" << i << ": " << executingPlayer->inventory->getSlotByIndex(i)->getItemId()<<"-" << (int)executingPlayer->inventory->getSlotByIndex(i)->count;
			Log::none() << Log::endl;
			Log::info() << "Offhand: " << Log::endl;
			Log::none() << "\t" << 45 << ": " << executingPlayer->inventory->getSlotByIndex(45)->getItemId() << "-" << (int)executingPlayer->inventory->getSlotByIndex(45)->count<< Log::endl;
			Log::info() << "Inventory: " << Log::endl;
			for (unsigned i = 1; i < 4; i++) {
				for (unsigned j = 0; j < 9; j++) {
					Slot* inventorySlot = executingPlayer->inventory->getSlotByIndex(9 * i + j);
					Log::none() << "\t" << 9 * i + j << ": " << inventorySlot->getItemId() << "-" << (int)inventorySlot->count << "  ";
				}
				Log::none() << Log::endl;
			}
			Log::info() << "Hotbar: " << Log::endl;
			for (unsigned i = 36; i < 45; i++)
				Log::none() << "\t" << i << ": " << executingPlayer->inventory->getSlotByIndex(i)->getItemId()<<"-"<< (int)executingPlayer->inventory->getSlotByIndex(i)->count;
			Log::none() << Log::endl;
			Log::info() << "Floating item: " << Log::endl << "\t" << executingPlayer->inventory->getFloatingSlot()->getItemId() << "-" << (int)executingPlayer->inventory->getFloatingSlot()->count << Log::endl << Log::endl;
		}
		break;
		//add tests here - starting at 1
		default:
			message::play::send::chatMessage(executingPlayer, Chat("Invalid test", Chat::color::red()), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		}
	}
	void teleport(CommandHandlerArguments)
	{
		switch (argumentStack.size())
		{
		case 1:
		{
			throw Chat("WIP", Chat::color::red());
			mcString& destination = *(mcString*)argumentStack[0];
		}
		break;
		case 2:
		{
			throw Chat("WIP", Chat::color::red());
			mcString& target = *(mcString*)argumentStack[0];
			mcString& destination = *(mcString*)argumentStack[1];
		}
		break;
		case 3:
		{
			executingPlayer->teleport(*(double*)argumentStack[0], *(double*)argumentStack[1], *(double*)argumentStack[2]);
		}
		break;
		case 4:
		{
			throw Chat("WIP", Chat::color::red());
			mcString& target = *(mcString*)argumentStack[0];
		}
		}

	}
	void spawn(CommandHandlerArguments)
	{
		World* wld = executingPlayer->world;
		executingPlayer->teleport(wld->spawn.X, wld->spawn.Y, wld->spawn.Z);
	}
	void setSpawn(CommandHandlerArguments)
	{
		executingPlayer->world->setSpawn(*(double*)argumentStack[0], *(double*)argumentStack[1], *(double*)argumentStack[2]);
	}
	void stopServer(CommandHandlerArguments)
	{
		Server::keepServerOpen = false;
	}
	void restartServer(CommandHandlerArguments)
	{
		Server::restartOnClose = true;
		Server::keepServerOpen = false;
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