#pragma once
#include "basic.h"
#include "player.h"
#include "varData.h"

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
	mask info = mask(0);
	varInt len;
	char lengthBuffer[3], * buffer = 0, * current;

public:
	ConnectionState state;

	Player(sf::TcpSocket*);
	~Player();

	void disconnect();
	void updateNet();
	void send(char*, ull);

	bool operator==(sf::TcpSocket*);

	static std::vector<Player*> players;

	//static void registerDisconnection(Player*);
	//static void registerDisconnection(ull i);
	static void clearPlayers();
};