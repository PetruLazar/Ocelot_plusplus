#pragma once
#include "basic.h"
#include "world.h"

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
		bufferingMessage = 0x02
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

	int protocolVersion;
	ConnectionState state;

	//data for keep alive messages
	static const clock_t keepAliveInterval = 10000; // how often to send keep alive packets
	static const clock_t keepAliveTimeoutAfter = 30000; // kick the client if it doesn't respond for this many ms
	clock_t nextKeepAlive = 0x7fffffff; //when to send the next keep alive message
	clock_t keepAliveTimeoutPoint; //when will the keep alive timeout end?
	int64 lastKeepAliveId = -1; //the expected id for which a keep alive is pending or -1 if there is no pending response

	//world and position information
	World* world; // pointer to the world the player is in
	bdouble X, Y, Z;
	bfloat yaw, pitch;
	int chunkX, chunkZ;
	bool onGround;

	int viewDistance;

	Player(sf::TcpSocket*);
	~Player();

	void disconnect();
	//check for incoming data on the socket
	void updateNet(clock_t time);
	//send data to player
	void send(char* data, ull data_length);

	bool operator==(sf::TcpSocket*);

	static std::vector<Player*> players;

	//clears the players list of disconnected players
	static void clearDisconnectedPlayers();
};