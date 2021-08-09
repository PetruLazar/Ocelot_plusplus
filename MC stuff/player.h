#pragma once
#include "basic.h"

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
	int protocolVersion;
	ConnectionState state;

	Player(sf::TcpSocket*);
	~Player();

	void disconnect();
	//check for incoming data on the socket
	void updateNet();
	//send data to player
	void send(char* data, ull data_length);

	bool operator==(sf::TcpSocket*);

	static std::vector<Player*> players;

	//clears the players list of disconnected players
	static void clearDisconnectedPlayers();
};