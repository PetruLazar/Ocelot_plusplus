#pragma once
#include "../world.h"
#include "../types/uuid.h"
#include "../types/enums.h"
#include "SFML/Network/TcpSocket.hpp"
#include "../types/chat.h"
#include "../types/queue.h"
#include "../types/entity.h"
#include "../types/slot.h"

enum class ConnectionState : Byte
{
	handshake,
	status,
	login,
	play
};
enum class Hand
{
	main,
	offhand
};
enum class ChatMode
{
	enabled,
	commands,
	hidden
};

class Player : public Entity::player
{
public:
	enum mask : Byte
	{
		bufferingLength = 0x01,
		bufferingMessage = 0x02,
		bufferingCompressedLength = 0x04
	};

private:
	sf::TcpSocket* socket;
	bool connected = true,
		scheduledDisconnect = false;
	//info about buffering (bit mask)
	//	bit 0 - buffering packet length
	//  bit 1 - buffering packet
	mask info = mask(0);
	//length of awaiting packet
	varInt len;
	//buffer for incoming packet length
	char lengthBuffer[3],
		//buffer for incoming packet
		* buffer = 0,
		//current position of any of the receiving buffers
		* current;

	MessageBuffer sendBuffer;

public:
	mcString username;

	//ip address + port
	std::string netId();

	int protocolVersion;
	ConnectionState state;

	//data for keep alive messages
	static const clock_t keepAliveInterval = 1000; // how often to send keep alive packets
	static const clock_t keepAliveTimeoutAfter = 30000; // kick the client if it doesn't respond for this many ms
	static const clock_t keepAliveTimeoutPointAfterScheduledDisconnect = 5000;
	clock_t nextKeepAlive = 0x80000000; //when to send the next keep alive message
	clock_t keepAliveTimeoutPoint; //when will the keep alive timeout end?
	int64 lastKeepAliveId = -1; //the expected id for which a keep alive is pending or -1 if there is no pending response

	//world and position information
	World* world; // pointer to the world the player is in
	int chunkX, chunkZ;
	bool onGround;
	void updatePosition(bdouble X, bdouble Y, bdouble Z);
	void updateRotation(bfloat yaw, bfloat pitch);
	bool positionInRange(Position);

	class _inventory {
	private:
		Slot* slots[46];
		bshort selectedHotbar = 0; //main hand selected slot
		Slot* floatingItem;

	public:
		_inventory();
		~_inventory();

		void setSelectedSlot(bshort selectedSlot);
		bshort getSelectedIndex(bool raw = false);

		Slot*& getSelectedSlot();
		Slot*& getOffhandSlot();
		Slot*& getHotbarSlot(bshort index);
		Slot*& getInventorySlot(bshort index);

		bshort getSlotWithLeastID(varInt itemID);
		bshort getFreeSlot();

		unsigned add(Slot& theItem, unsigned& addedIndex);

		void setInventorySlot(bshort index, Slot* slot);
	} *inventory;

	//player info
	varInt ping;
	bool hasDisplayName;
	Chat* displayName = nullptr;

	//set compression
	uint compressionThreshold = (uint)(-1);
	bool compressionEnabled = false;
	varInt compressedLength;

	//teleport/teleport confirm
	varInt pendingTpId = -1;
	int nextTpId = 0;
	void teleport(bdouble tpX, bdouble tpY, bdouble tpZ);
	void teleport(bdouble tpX, bdouble tpY, bdouble tpZ, bfloat yaw, bfloat pitch);

	//other players visibility
	std::vector<Player*> seenBy;
	void enterSight(Player*);
	void exitSight(Player*);
	void exitSight(ull);

	gamemode gm;

	//user settings
	mcString locale = "en_US";
	int viewDistance;
	int simulationDistance;
	ChatMode chatMode;		//---\/
	bool chatColors;		//to do: processing chat
	bool enableTextFiltering;
	bool allowServerListings;

	SERVER_API Player(sf::TcpSocket*);
	SERVER_API ~Player();

	SERVER_API void setWorld(World* world);
	SERVER_API void leaveWorld(World* world);
	SERVER_API void changeWorld(World* newWorld);
	SERVER_API void changeWorld(const mcString& worldName);

	SERVER_API void disconnect();
	//check for incoming data on the socket
	SERVER_API void updateNet();
	//send data to player
	SERVER_API void schedulePacket(char* data, ull data_length, char* toDelete, bool disconnectAfter = false);

	SERVER_API bool Connected();
	SERVER_API bool ScheduledDisconnect();

	SERVER_API static std::vector<Player*> players;
	SERVER_API static eidDispenser::Player eidDispenser;

	//clears the players list of disconnected players
	SERVER_API static void clearDisconnectedPlayers();
	SERVER_API static void broadcastChat(const Chat&, Player* ignore = nullptr);
};