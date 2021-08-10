#include "player.h"
#include "message.h"

const char* invalidPacketLengthError = "Invalid Packet Length";
const char* socketError = "Socket error occured";
const char* socketDisconnected = "Socket Disconnected unexpectedly";

std::vector<Player*> Player::players;

Player::Player(sf::TcpSocket* socket) : state(ConnectionState::handshake), socket(socket)
{
	socket->setBlocking(false);
}
Player::~Player()
{
	delete socket;
	if (buffer) delete buffer;
}

void Player::disconnect()
{
	socket->disconnect();
	connected = false;
}
void Player::updateNet(clock_t time)
{
	if (!connected) return;
	if (time > nextKeepAlive && lastKeepAliveId == -1)
	{
		nextKeepAlive = time + keepAliveInterval;
		keepAliveTimeoutPoint = time + keepAliveTimeoutAfter;
		lastKeepAliveId = time;
		message::play::send::keepAlive(this, lastKeepAliveId);
	}
	else if (lastKeepAliveId != -1 && time > keepAliveTimeoutPoint)
	{
		disconnect();
		std::cout << '\n' << username << " was timed out.";
		return;
	}
	ull received = 0;
	if (info & bufferingLength)
	{
		ull oldReceived = current - lengthBuffer;
		switch (socket->receive(current, 1, received))
		{
		case sockStat::Done:
			if (len.valid(lengthBuffer, received + oldReceived))
			{
				char* b = lengthBuffer;
				len.read(b);
				buffer = new char[len];
				current = buffer;
				info = mask(info & ~bufferingLength | bufferingMessage);
			}
			else
			{
				if (received + oldReceived == 3) throw invalidPacketLengthError;
				current += received;
			}
			break;
		case sockStat::Error:
			disconnect();
			throw socketError;
		case sockStat::Disconnected:
			disconnect();
			throw socketDisconnected;
		}
	}
	else if (info & bufferingMessage)
	{
		ull oldReceived = current - buffer;
		switch (socket->receive(current, len - oldReceived, received))
		{
		case sockStat::Done:
			if (len == oldReceived + received)
			{
				info = mask(info & ~bufferingMessage);

				message::dispatch(this, buffer, len);

				delete[] buffer;
				buffer = 0;
			}
			else
			{
				current += received;
			}
			break;
		case sockStat::Error:
			disconnect();
			throw socketError;
		case sockStat::Disconnected:
			disconnect();
			throw socketDisconnected;
		}
	}
	else
	{
		switch (socket->receive(lengthBuffer, 1, received))
		{
		case sockStat::Done:
			if (len.valid(lengthBuffer, received))
			{
				char* b = lengthBuffer;
				len.read(b);
				buffer = new char[len];
				current = buffer;
				info = mask(info | bufferingMessage);
			}
			else
			{
				if (received == 3) throw invalidPacketLengthError;
				info = mask(info | bufferingLength);
				current = lengthBuffer + received;
			}
			break;
		case sockStat::Error:
			disconnect();
			throw socketError;
		case sockStat::Disconnected:
			disconnect();
			throw socketDisconnected;
		}
	}
}
void Player::send(char* buffer, ull size)
{
	ull sent;
	sockStat stat = sockStat::Error;
	do
	{
		stat = socket->send(buffer, size, sent);
		buffer += sent;
		size -= sent;
	} while (stat == sockStat::Partial);

	switch (stat)
	{
	case sockStat::Disconnected:
		disconnect();
		throw socketDisconnected;
	case sockStat::Error:
		disconnect();
		throw socketError;
	}
}

bool Player::operator==(sf::TcpSocket* s)
{
	return socket == s;
}

void Player::clearDisconnectedPlayers()
{
	ull size = players.size();
	for (ull i = 0; i < size; i++) if (!players[i]->connected)
	{
		delete players[i];
		for (ull j = i-- + 1; j < size; j++) players[j - 1] = players[j];
		players.pop_back();
		size--;
	}
}