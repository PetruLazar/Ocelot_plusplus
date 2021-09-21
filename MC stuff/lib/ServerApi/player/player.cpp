#include "player.h"
#include "message.h"
#include "../types/error.h"
#include <iostream>
#include "../types/enums.h"
#include "../server/log.h"

const char* invalidPacketLengthError = "Invalid Packet Length";
const char* socketError = "Socket error occured";
const char* socketDisconnected = "Socket Disconnected unexpectedly";

std::vector<Player*> Player::players;

Player::Player(sf::TcpSocket* socket) : state(ConnectionState::handshake), socket(socket)
{
	socket->setBlocking(false);

	//player initializations
	keepAliveTimeoutPoint = cycleTime + keepAliveTimeoutAfter;
}
Player::~Player()
{
	delete socket;
	if (buffer) delete buffer;
	if (uuid) delete uuid;
}

void Player::updatePosition(bdouble X, bdouble Y, bdouble Z)
{
	int newChunkX = fastfloor(X) >> 4,
		newChunkZ = fastfloor(Z) >> 4;

	if (newChunkX != chunkX && newChunkZ != chunkZ)
	{
		message::play::send::updateViewPosition(this, newChunkX, newChunkZ);
		if (newChunkX < chunkX && newChunkZ < chunkZ)
		{
			//towards negative X
			//towards negative Z
			bint unloadX = chunkX + viewDistance;
			for (int z = chunkZ - viewDistance; z <= chunkZ + viewDistance; z++) message::play::send::unloadChunk(this, unloadX, z);
			bint unloadZ = chunkZ + viewDistance;
			for (int x = chunkX - viewDistance; x < chunkX + viewDistance; x++) message::play::send::unloadChunk(this, x, unloadZ);

			bint loadX = newChunkX - viewDistance;
			for (int z = newChunkZ - viewDistance; z <= newChunkZ + viewDistance; z++)
			{
				message::play::send::sendFullChunk(this, (int)loadX, z);
			}
			bint loadZ = newChunkZ - viewDistance;
			for (int x = newChunkX - viewDistance + 1; x <= newChunkX + viewDistance; x++)
			{
				message::play::send::sendFullChunk(this, x, (int)loadZ);
			}
		}
		else if (newChunkX < chunkX)
		{
			//towards negative X
			//towards positive Z
			bint unloadX = chunkX + viewDistance;
			for (int z = chunkZ - viewDistance; z <= chunkZ + viewDistance; z++) message::play::send::unloadChunk(this, unloadX, z);
			bint unloadZ = chunkZ - viewDistance;
			for (int x = chunkX - viewDistance; x < chunkX + viewDistance; x++) message::play::send::unloadChunk(this, x, unloadZ);

			bint loadX = newChunkX - viewDistance;
			for (int z = newChunkZ - viewDistance; z <= newChunkZ + viewDistance; z++)
			{
				message::play::send::sendFullChunk(this, (int)loadX, z);
			}
			bint loadZ = newChunkZ + viewDistance;
			for (int x = newChunkX - viewDistance + 1; x <= newChunkX + viewDistance; x++)
			{
				message::play::send::sendFullChunk(this, x, (int)loadZ);
			}
		}
		else if (newChunkZ < chunkZ)
		{
			//towards positive X
			//towards negative Z
			bint unloadX = chunkX - viewDistance;
			for (int z = chunkZ - viewDistance; z <= chunkZ + viewDistance; z++) message::play::send::unloadChunk(this, unloadX, z);
			bint unloadZ = chunkZ + viewDistance;
			for (int x = chunkX - viewDistance + 1; x <= chunkX + viewDistance; x++) message::play::send::unloadChunk(this, x, unloadZ);

			bint loadX = newChunkX + viewDistance;
			for (int z = newChunkZ - viewDistance; z <= newChunkZ + viewDistance; z++)
			{
				message::play::send::sendFullChunk(this, (int)loadX, z);
			}
			bint loadZ = newChunkZ - viewDistance;
			for (int x = newChunkX - viewDistance; x < newChunkX + viewDistance; x++)
			{
				message::play::send::sendFullChunk(this, x, (int)loadZ);
			}
		}
		else
		{
			//towards positive X
			//towards positive Z
			bint unloadX = chunkX - viewDistance;
			for (int z = chunkZ - viewDistance; z <= chunkZ + viewDistance; z++) message::play::send::unloadChunk(this, unloadX, z);
			bint unloadZ = chunkZ - viewDistance;
			for (int x = chunkX - viewDistance + 1; x <= chunkX + viewDistance; x++) message::play::send::unloadChunk(this, x, unloadZ);

			bint loadX = newChunkX + viewDistance;
			for (int z = newChunkZ - viewDistance; z <= newChunkZ + viewDistance; z++)
			{
				message::play::send::sendFullChunk(this, (int)loadX, z);
			}
			bint loadZ = newChunkZ + viewDistance;
			for (int x = newChunkX - viewDistance; x < newChunkX + viewDistance; x++)
			{
				message::play::send::sendFullChunk(this, x, (int)loadZ);
			}
		}
	}
	else if (newChunkX != chunkX)
	{
		message::play::send::updateViewPosition(this, newChunkX, newChunkZ);
		if (newChunkX < chunkX)
		{
			//towards negative X
			bint unloadX = chunkX + viewDistance;
			for (int z = chunkZ - viewDistance; z <= chunkZ + viewDistance; z++) message::play::send::unloadChunk(this, unloadX, z);
			bint loadX = newChunkX - viewDistance;
			for (int z = newChunkZ - viewDistance; z <= newChunkZ + viewDistance; z++)
			{
				message::play::send::sendFullChunk(this, (int)loadX, z);
			}
		}
		else
		{
			//towards positive X
			bint unloadX = chunkX - viewDistance;
			for (int z = chunkZ - viewDistance; z <= chunkZ + viewDistance; z++) message::play::send::unloadChunk(this, unloadX, z);
			bint loadX = newChunkX + viewDistance;
			for (int z = newChunkZ - viewDistance; z <= newChunkZ + viewDistance; z++)
			{
				message::play::send::sendFullChunk(this, (int)loadX, z);
			}
		}
	}
	else if (newChunkZ != chunkZ)
	{
		message::play::send::updateViewPosition(this, newChunkX, newChunkZ);
		if (newChunkZ < chunkZ)
		{
			//towards negative Z
			bint unloadZ = chunkZ + viewDistance;
			for (int x = chunkX - viewDistance; x <= chunkX + viewDistance; x++) message::play::send::unloadChunk(this, x, unloadZ);
			bint loadZ = newChunkZ - viewDistance;
			for (int x = newChunkX - viewDistance; x <= newChunkX + viewDistance; x++)
			{
				message::play::send::sendFullChunk(this, x, (int)loadZ);
			}
		}
		else
		{
			//towards positive Z
			bint unloadZ = chunkZ - viewDistance;
			for (int x = chunkX - viewDistance; x <= chunkX + viewDistance; x++) message::play::send::unloadChunk(this, x, unloadZ);
			bint loadZ = newChunkZ + viewDistance;
			for (int x = newChunkX - viewDistance; x <= newChunkX + viewDistance; x++)
			{
				message::play::send::sendFullChunk(this, x, (int)loadZ);
			}
		}
	}

	Player::X = X;
	Player::Y = Y;
	Player::Z = Z;
	Player::chunkX = newChunkX;
	Player::chunkZ = newChunkZ;
}
void Player::updateRotation(bfloat yaw, bfloat pitch)
{
	Player::yaw = yaw;
	Player::pitch = pitch;
}

void Player::setWorld(World* world)
{
	IF_DEBUG_SIGHT(Log::txt() << '\n' << this << " is entering world " << world->name);
	//set world and position
	Player::world = world;
	X = world->spawn.X;
	Y = world->spawn.Y;
	Z = world->spawn.Z;
	chunkX = world->spawn.ChunkX;
	chunkZ = world->spawn.ChunkZ;
	yaw = world->spawn.Yaw;
	pitch = world->spawn.Pitch;

	//send necessary packets
	message::play::send::updateViewPosition(this, chunkX, chunkZ);

	message::play::send::timeUpdate(this, 6000i64, 6000i64);

	message::play::send::spawnPosition(this, world->spawn.Absolute, 0.f);

	for (int x = chunkX - viewDistance; x <= chunkX + viewDistance; x++) for (int z = chunkZ - viewDistance; z <= chunkZ + viewDistance; z++)
	{
		message::play::send::sendFullChunk(this, x, z);
		//message::play::send::updateLight(this, x, z);
		//message::play::send::chunkData(this, x, z);
	}

	message::play::send::playerPosAndLook(this, X, Y, Z, yaw, pitch, 0, false);

	//check for players in sight
	for (Player* otherP : world->players)
	{
		if (otherP->state == ConnectionState::play && abs(otherP->chunkX - chunkX) <= viewDistance && abs(otherP->chunkZ - chunkZ) <= viewDistance)
		{
			enterSight(otherP);
			otherP->enterSight(this);
		}
	}
	//add the player to the world's player list
	world->players.push_back(this);
	IF_DEBUG_SIGHT(Log::txt() << "\nPlayer list for " << world->name << " is now " << world->players.size());
}
void Player::leaveWorld(World* world)
{
	IF_DEBUG_SIGHT(Log::txt() << "\nPlayer " << this << " is leaving world " << world->name);
	//unload chunks in the world
	for (int x = chunkX - viewDistance; x <= chunkX + viewDistance; x++) for (int z = chunkZ - viewDistance; z <= chunkZ + viewDistance; z++) ignoreExceptions(world->unload(x, z));

	//destroy the player entity for the players who see this player
	ull size = seenBy.size();
	for (ull i = 0; i < size--; i++)
	{
		seenBy[i]->exitSight(this);
		exitSight(i--);
	}

	//remove the player from the world's player list
	size = world->players.size();
	for (ull i = 0; i < size; i++) if (world->players[i] == this)
	{
		world->players.erase(world->players.begin() + i);
		break;
	}
	IF_DEBUG_SIGHT(Log::txt() << "\nPlayer list of " << world->name << " is now " << world->players.size());
}
void Player::changeWorld(World* newWorld)
{
	message::play::send::respawn(this, newWorld->characteristics, newWorld->name, 0x5f19a34be6c9129a, gm, gamemode::none, false, newWorld->isFlat, true);

	leaveWorld(world);

	setWorld(newWorld);
}
void Player::changeWorld(const mcString& worldName)
{
	for (uint i = 0; i < World::worlds.size(); i++) if (World::worlds[i]->name == worldName) changeWorld(World::worlds[i]);
}

void Player::enterSight(Player* other)
{
	IF_DEBUG_SIGHT(Log::txt() << "\nPlayer " << other << " entering sight of " << this);
	ignoreExceptions(message::play::send::spawnPlayer(this, other->eid + 1, *other->uuid, other->X, other->Y, other->Z, (float)other->yaw, (float)other->pitch));
	ignoreExceptions(message::play::send::entityHeadLook(this, other->eid + 1, (float)other->yaw));
	seenBy.push_back(other);
	IF_DEBUG_SIGHT(Log::txt() << "\nSight of " << this << " is now " << seenBy.size());
}
void Player::exitSight(Player* other)
{
	ull size = seenBy.size();
	for (ull i = 0; i < size; i++) if (seenBy[i] == other)
	{
		exitSight(i);
		return;
	}
}
void Player::exitSight(ull otherI)
{
	IF_DEBUG_SIGHT(Log::txt() << "\nPlayer " << seenBy[otherI] << " exiting sight of " << this);
	varInt* eids = new varInt[1]{ seenBy[otherI]->eid + 1 };
	ignoreExceptions(message::play::send::destroyEntities(this, 1, eids));
	delete[] eids;
	seenBy.erase(seenBy.begin() + otherI);
	IF_DEBUG_SIGHT(Log::txt() << "\nSight of " << this << " is now " << seenBy.size());
}

void Player::disconnect()
{
	socket->disconnect();
	connected = false;
	if (state == ConnectionState::play)
	{
		leaveWorld(world);
		broadcastChat(Chat((username + " left the game").c_str(), Chat::yellow), this);
		Player* p = this;
		broadcastMessageOmitSafe(message::play::send::playerInfo(player_macro, playerInfo::removePlayer, 1, &p), this)
	}
}
void Player::updateNet()
{
	//check for keepAlive interval
	if (!connected) return;
	if (cycleTime > nextKeepAlive && lastKeepAliveId == -1 && state == ConnectionState::play)
	{
		nextKeepAlive = cycleTime + keepAliveInterval;
		keepAliveTimeoutPoint = cycleTime + keepAliveTimeoutAfter;
		lastKeepAliveId = cycleTime;
		message::play::send::keepAlive(this, lastKeepAliveId);
	}
	else if ((lastKeepAliveId != -1 || state != ConnectionState::play) && cycleTime > keepAliveTimeoutPoint)
	{
		disconnect();
		std::cout << '\n' << username << " was timed out.";
		return;
	}

	//send data
	if (!sendBuffer.empty())
	{
		MessageBuffer::Iterator& it = *sendBuffer.first;
		switch (it.msgType)
		{
		case MessageBuffer::chunk:
		case MessageBuffer::raw:
		{
			ull sent;
			MessageBuffer::Iterator::Value::Raw& val = it.data.raw;
			switch (socket->send(val.buffer, val.bufferSize, sent))
			{
			case sockStat::Disconnected:
				disconnect();
				throw protocolError(socketDisconnected);
			case sockStat::Error:
				disconnect();
				throw protocolError(socketError);
			case sockStat::Done:
				sendBuffer.pop();
				break;
			case sockStat::Partial:
				val.buffer += sent;
				val.bufferSize -= sent;
				break;
			}
		}
		break;
		case MessageBuffer::disconnect:
			disconnect();
			break;
		}
	}

	//receive data
	if (scheduledDisconnect) return;
	ull received = 0;
	if (compressionEnabled)
		received = 0;
	//length or uncompressedLength (if compression is enabled)
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
				if (compressionEnabled)
				{
					compressedLength -= (int)varInt::size(len);
					buffer = new char[compressedLength];
				}
				else
				{
					if (!len) throw protocolError(invalidPacketLengthError);
					buffer = new char[len];
				}
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
			throw protocolError(socketError);
		case sockStat::Disconnected:
			disconnect();
			throw protocolError(socketDisconnected);
		}
	}
	else if (info & bufferingMessage)
	{
		ull oldReceived = current - buffer;
		int expectedLength = (compressionEnabled ? compressedLength : len);
		switch (socket->receive(current, expectedLength - oldReceived, received))
		{
		case sockStat::Done:
			if (expectedLength == oldReceived + received)
			{
				info = mask(info & ~bufferingMessage);
				//if (compressionEnabled) info = mask(info | buffering)

				try
				{
					if (compressionEnabled)
						message::dispatch(this, buffer, compressedLength, len);
					else
						message::dispatch(this, buffer, len);
				}
				catch (...)
				{
					delete[] buffer;
					buffer = 0;
					throw;
				}

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
			throw protocolError(socketError);
		case sockStat::Disconnected:
			disconnect();
			throw protocolError(socketDisconnected);
		}
	}
	else if (info & bufferingCompressedLength)
	{
		ull oldReceived = current - lengthBuffer;
		switch (socket->receive(current, 1, received))
		{
		case sockStat::Done:
			if (compressedLength.valid(lengthBuffer, received + oldReceived))
			{
				char* b = lengthBuffer;
				compressedLength.read(b);
				current = lengthBuffer;
				info = mask(info & ~bufferingCompressedLength | bufferingLength);
			}
			else
			{
				if (received + oldReceived == 3) throw invalidPacketLengthError;
				current += received;
			}
			break;
		case sockStat::Error:
			disconnect();
			throw protocolError(socketError);
		case sockStat::Disconnected:
			disconnect();
			throw protocolError(socketDisconnected);
		}
	}
	else if (compressionEnabled)
	{
		switch (socket->receive(lengthBuffer, 1, received))
		{
		case sockStat::Done:
			if (compressedLength.valid(lengthBuffer, received))
			{
				char* b = lengthBuffer;
				compressedLength.read(b);
				current = lengthBuffer;
				info = mask(info | bufferingLength);
			}
			else
			{
				if (received == 3) throw invalidPacketLengthError;
				info = mask(info | bufferingCompressedLength);
				current = lengthBuffer + received;
			}
			break;
		case sockStat::Error:
			disconnect();
			throw protocolError(socketError);
		case sockStat::Disconnected:
			disconnect();
			throw protocolError(socketDisconnected);
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
				if (!len) throw protocolError(invalidPacketLengthError);
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
			throw protocolError(socketError);
		case sockStat::Disconnected:
			disconnect();
			throw protocolError(socketDisconnected);
		}
	}
}
void Player::schedulePacket(char* buffer, ull size, char* toDelete, bool disconnectAfter)
{
	if (scheduledDisconnect) return;
	sendBuffer.push(buffer, size, toDelete);
	if (disconnectAfter)
	{
		sendBuffer.push();
		scheduledDisconnect = true;
		keepAliveTimeoutPoint = cycleTime + keepAliveTimeoutPointAfterScheduledDisconnect;
	}
	/*ull sent;
	sockStat stat = sockStat::Error;
	do
	{
		stat = socket->send(buffer, size, sent);
		buffer += sent;
		size -= sent;
	} while (stat == sockStat::Partial || stat == sockStat::NotReady);

	switch (stat)
	{
	case sockStat::Disconnected:
		disconnect();
		throw protocolError(socketDisconnected);
	case sockStat::Error:
		disconnect();
		throw protocolError(socketError);
	}*/
}

bool Player::Connected() { return connected; }
bool Player::ScheduledDisconnect() { return scheduledDisconnect; }

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
void Player::broadcastChat(const Chat& msg, Player* ignore)
{
	for (Player* p : players) if (p != ignore && !p->scheduledDisconnect) ignoreExceptions(message::play::send::chatMessage(p, msg, ChatMessage::systemMessage, mcUUID(0, 0, 0, 0)));
}