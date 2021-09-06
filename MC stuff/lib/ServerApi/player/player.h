#pragma once
#include "../world.h"
#include "../types/uuid.h"
#include "../types/enums.h"
#include "SFML/Network/TcpSocket.hpp"
#include "../types/chat.h"

enum class ConnectionState : byte
{
	handshake,
	status,
	login,
	play
};

class Player
{
public:

	enum mask : byte
	{
		bufferingLength = 0x01,
		bufferingMessage = 0x02,
		bufferingCompressedLength = 0x04
	};

private:
	sf::TcpSocket* socket;
	bool connected = true;
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

public:
	mcString username;
	mcUUID* uuid = 0;
	varInt eid;

	int protocolVersion;
	ConnectionState state;

	//data for keep alive messages
	static const clock_t keepAliveInterval = 1000; // how often to send keep alive packets
	static const clock_t keepAliveTimeoutAfter = 30000; // kick the client if it doesn't respond for this many ms
	clock_t nextKeepAlive = 0x80000000; //when to send the next keep alive message
	clock_t keepAliveTimeoutPoint; //when will the keep alive timeout end?
	int64 lastKeepAliveId = -1; //the expected id for which a keep alive is pending or -1 if there is no pending response

	//world and position information
	World* world; // pointer to the world the player is in
	bdouble X, Y, Z;
	bfloat yaw, pitch;
	int chunkX, chunkZ;
	bool onGround;

	//player info
	varInt ping;
	bool hasDisplayName;
	Chat* displayName = nullptr;

	//set compression
	uint compressionThreshold = (uint)(-1);
	bool compressionEnabled = false;
	varInt compressedLength;

	gamemode gm;

	int viewDistance;

	SERVER_API Player(sf::TcpSocket*);
	SERVER_API ~Player();

	SERVER_API void changeWorld(World* newWorld);
	SERVER_API void changeWorld(const mcString& worldName);

	SERVER_API void disconnect();
	//check for incoming data on the socket
	SERVER_API void updateNet();
	//send data to player
	SERVER_API void send(char* data, ull data_length);

	SERVER_API bool Connected();

	SERVER_API static std::vector<Player*> players;

	//clears the players list of disconnected players
	SERVER_API static void clearDisconnectedPlayers();
	SERVER_API static void broadcastChat(const Chat&, Player* ignore = nullptr);
};