#include "player.h"
#include "message.h"
#include "../types/error.h"
#include "../types/enums.h"
#include "../server/log.h"
#include "../types/entity.h"
#include "../server/options.h"

const char* invalidPacketLengthError = "Invalid Packet Length";
const char* socketError = "Socket error occured";
const char* socketDisconnected = "Socket Disconnected unexpectedly";

std::vector<Player*> Player::players;
eidDispenser::Player Player::eidDispenser;

Player::Player(sf::TcpSocket* socket) : state(ConnectionState::handshake), socket(socket), Entity::player(&eidDispenser, Entity::type::minecraft_player, 0, 0, 0, .0, .0), chunkLoaderHelper(Options::viewDistance())
{
	socket->setBlocking(false);

	//player initializations
	keepAliveTimeoutPoint = cycleTime + keepAliveTimeoutAfter;

	inventory = new _inventory();
	windower = new _windower();
}
Player::~Player()
{
	delete socket;
	if (buffer) delete buffer;

	delete inventory;
	delete windower;
}

std::string Player::netId()
{
	return socket->getRemoteAddress().toString() + ':' + std::to_string(socket->getRemotePort());
}

void Player::ChunkLoaderHelper::ResetEdge()
{
	AdvanceIndex();
	e = 0;
}
void Player::ChunkLoaderHelper::ResetIndex()
{
	AdvanceRadius();
	if (a > v) i = a - v;
	else i = 0;
}
void Player::ChunkLoaderHelper::ResetRadius()
{
	enabled = false;
	a = 0;
}

bool Player::ChunkLoaderHelper::EdgeOverflow()
{
	return e >= 4;
}
bool Player::ChunkLoaderHelper::IndexOverflow()
{
	//return (a > v && i > v) || (a <= v && i >= a)

	if (a > v) return i > v;
	return i >= a;
}
bool Player::ChunkLoaderHelper::RadiusOverflow()
{
	return a > 2 * v;
}

void Player::ChunkLoaderHelper::AdvanceEdge()
{
	e++;
	if (EdgeOverflow()) ResetEdge();
}
void Player::ChunkLoaderHelper::AdvanceIndex()
{
	i++;
	if (IndexOverflow()) ResetIndex();
}
void Player::ChunkLoaderHelper::AdvanceRadius()
{
	a++;
	if (RadiusOverflow()) ResetRadius();
}

sf::Vector2i Player::ChunkLoaderHelper::Generate()
{
	int x = i, y = a - x;
	switch (e)
	{
	case 0:
		return sf::Vector2i(x, y);
	case 1:
		return sf::Vector2i(y, -x);
	case 2:
		return sf::Vector2i(-x, -y);
	}
	return sf::Vector2i(-y, x);
}

Player::ChunkLoaderHelper::ChunkLoaderHelper(int viewDistance) : v(viewDistance), a(0), i(0), e(3), matrix(viewDistance) {}
void Player::ChunkLoaderHelper::UpdateViewDistance(int viewDistance)
{
	v = viewDistance;
	Reset();
	matrix.resize(v);
}

void Player::ChunkLoaderHelper::Reset()
{
	enabled = true;
	a = i = 0;
	e = 3;
}
sf::Vector2i Player::ChunkLoaderHelper::Next()
{
	if (!enabled) return sf::Vector2i(0, 0);

	sf::Vector2i ret = Generate();
	AdvanceEdge();
	return ret;
}
bool Player::ChunkLoaderHelper::Finished() const { return !enabled; }

Player::ChunkLoaderHelper::ChunkMatrix::SubMatrix::SubMatrix(Byte height, Byte width) : width(width), values(height, 0) {}

void Player::ChunkLoaderHelper::ChunkMatrix::SubMatrix::resize(Byte newHeight, Byte newWidth)
{
	if (newWidth > width)
	{
		bitsettype map = ~((bitsettype)(-1) << width);
		for (bitsettype& v : values) v &= map;
	}
	width = newWidth;

	values.resize(newHeight, 0);
}
bool Player::ChunkLoaderHelper::ChunkMatrix::SubMatrix::get(Byte i, Byte j) const
{
	return values[i] & (1 << j);
}
void Player::ChunkLoaderHelper::ChunkMatrix::SubMatrix::set(Byte i, Byte j, bool v)
{
	if (v) values[i] |= (1 << j);
	else values[i] &= ~(1 << j);
}

Byte Player::ChunkLoaderHelper::ChunkMatrix::SubMatrix::Height() const
{
	return (Byte)values.size();
}
Byte Player::ChunkLoaderHelper::ChunkMatrix::SubMatrix::Width() const
{
	return width;
}

void Player::ChunkLoaderHelper::ChunkMatrix::SubMatrix::Shift_px(bitsettype val)
{
	values.pop_back();
	values.insert(values.begin(), val);
}
void Player::ChunkLoaderHelper::ChunkMatrix::SubMatrix::Shift_pz(Byte x, bool val)
{
	bitsettype& value = values[x];
	value <<= 1;
	if (val) value |= 1;
	else value &= ~1;
}
Player::ChunkLoaderHelper::ChunkMatrix::bitsettype Player::ChunkLoaderHelper::ChunkMatrix::SubMatrix::Shift_nx()
{
	bitsettype ret = values[0];
	values.erase(values.begin());
	values.push_back(0);
	return ret;
}
bool Player::ChunkLoaderHelper::ChunkMatrix::SubMatrix::Shift_nz(Byte x)
{
	bitsettype& value = values[x];
	bool ret = value & 1;
	value >>= 1;
	value &= ~(1 << (width - 1));
	return ret;
}

void Player::ChunkLoaderHelper::ChunkMatrix::SubMatrix::Empty()
{
	for (bitsettype& v : values) v = 0;
}

Player::ChunkLoaderHelper::ChunkMatrix::ChunkMatrix(int viewDistance) : pos_pos(viewDistance + 1, viewDistance + 1), pos_neg(viewDistance + 1, viewDistance), neg_pos(viewDistance, viewDistance + 1), neg_neg(viewDistance, viewDistance), viewDistance(viewDistance) {}

void Player::ChunkLoaderHelper::ChunkMatrix::resize(int newViewDistance)
{
	pos_pos.resize(newViewDistance + 1, newViewDistance + 1);
	pos_neg.resize(newViewDistance + 1, newViewDistance);
	neg_pos.resize(newViewDistance, newViewDistance + 1);
	neg_neg.resize(newViewDistance, newViewDistance);
	viewDistance = newViewDistance;
}
bool Player::ChunkLoaderHelper::ChunkMatrix::get(int x, int z) const
{
	if (x < 0)
	{
		x = -x - 1;
		if (z < 0) return neg_neg.get(x, -z - 1);
		else return neg_pos.get(x, z);
	}
	else
	{
		if (z < 0) return pos_neg.get(x, -z - 1);
		else return pos_pos.get(x, z);
	}
}
void Player::ChunkLoaderHelper::ChunkMatrix::set(int x, int z, bool v)
{
	if (x < 0)
	{
		x = -x - 1;
		if (z < 0) neg_neg.set(x, -z - 1, v);
		else neg_pos.set(x, z, v);
	}
	else
	{
		if (z < 0) pos_neg.set(x, -z - 1, v);
		else pos_pos.set(x, z, v);
	}
}

int Player::ChunkLoaderHelper::ChunkMatrix::ViewDistance() const
{
	return viewDistance;
}

void Player::ChunkLoaderHelper::ChunkMatrix::Shift_positive_x()
{
	pos_pos.Shift_px(neg_pos.Shift_nx());
	pos_neg.Shift_px(neg_neg.Shift_nx());
}
void Player::ChunkLoaderHelper::ChunkMatrix::Shift_positive_z()
{
	for (int i = 0; i < viewDistance; i++)
	{
		pos_pos.Shift_pz(i, pos_neg.Shift_nz(i));
		neg_pos.Shift_pz(i, neg_neg.Shift_nz(i));
	}
	pos_pos.Shift_pz(viewDistance, pos_neg.Shift_nz(viewDistance));
}
void Player::ChunkLoaderHelper::ChunkMatrix::Shift_negative_x()
{
	neg_pos.Shift_px(pos_pos.Shift_nx());
	neg_neg.Shift_px(pos_neg.Shift_nx());
}
void Player::ChunkLoaderHelper::ChunkMatrix::Shift_negative_z()
{
	for (int i = 0; i < viewDistance; i++)
	{
		pos_neg.Shift_pz(i, pos_pos.Shift_nz(i));
		neg_neg.Shift_pz(i, neg_pos.Shift_nz(i));
	}
	pos_neg.Shift_pz(viewDistance, pos_pos.Shift_nz(viewDistance));
}

void Player::ChunkLoaderHelper::ChunkMatrix::Empty()
{
	pos_pos.Empty();
	pos_neg.Empty();
	neg_pos.Empty();
	neg_neg.Empty();
}

void Player::updatePosition(bdouble X, bdouble Y, bdouble Z)
{
	int newChunkX = fastfloor(X) >> 4,
		newChunkZ = fastfloor(Z) >> 4;

	bool chunkChanged = false;

	if (newChunkX != chunkX)
	{
		int delta = newChunkX - chunkX;
		if (abs(delta) > 2 * viewDistance + 1)
		{
			for (int x = -viewDistance; x < viewDistance; x++)
				for (int z = -viewDistance; z <= viewDistance; z++)
					if (chunkLoaderHelper.matrix.get(x, z))
						message::play::send::unloadChunk(this, x + chunkX, z + chunkZ);

			chunkLoaderHelper.matrix.Empty();
			chunkLoaderHelper.Reset();
			chunkX = newChunkX;
			chunkZ = newChunkZ;
			message::play::send::updateViewPosition(this, chunkX, chunkZ);
			Player::x = X;
			Player::y = Y;
			Player::z = Z;

			return;
		}
		if (delta > 0)
		{

			for (int x = -viewDistance; x < -viewDistance + delta; x++)
				for (int z = -viewDistance; z <= viewDistance; z++)
					if (chunkLoaderHelper.matrix.get(x, z))
						message::play::send::unloadChunk(this, x + chunkX, z + chunkZ);

			for (int i = 0; i < delta; i++)
				chunkLoaderHelper.matrix.Shift_negative_x();
		}
		else if (delta < 0)
		{
			for (int x = viewDistance; x > viewDistance - delta; x--)
				for (int z = -viewDistance; z <= viewDistance; z++)
					if (chunkLoaderHelper.matrix.get(x, z))
						message::play::send::unloadChunk(this, x + chunkX, z + chunkZ);

			for (int i = 0; i > delta; i--)
				chunkLoaderHelper.matrix.Shift_positive_x();
		}

		chunkLoaderHelper.Reset();
		chunkX = newChunkX;
		chunkChanged = true;
	}

	if (newChunkZ != chunkZ)
	{
		int delta = newChunkZ - chunkZ;
		if (abs(delta) > 2 * viewDistance + 1)
		{
			for (int x = -viewDistance; x < viewDistance; x++)
				for (int z = -viewDistance; z <= viewDistance; z++)
					if (chunkLoaderHelper.matrix.get(x, z))
						message::play::send::unloadChunk(this, x + chunkX, z + chunkZ);

			chunkLoaderHelper.matrix.Empty();
			chunkLoaderHelper.Reset();
			chunkZ = newChunkZ;
			message::play::send::updateViewPosition(this, chunkX, chunkZ);
			Player::x = X;
			Player::y = Y;
			Player::z = Z;

			return;
		}
		if (delta > 0)
		{
			for (int z = -viewDistance; z < -viewDistance + delta; z++)
				for (int x = -viewDistance; x <= viewDistance; x++)
					if (chunkLoaderHelper.matrix.get(x, z))
						message::play::send::unloadChunk(this, x + chunkX, z + chunkZ);

			for (int i = 0; i < delta; i++)
				chunkLoaderHelper.matrix.Shift_negative_z();
		}
		else if (delta < 0)
		{
			for (int z = viewDistance; z > viewDistance - delta; z--)
				for (int x = -viewDistance; x <= viewDistance; x++)
					if (chunkLoaderHelper.matrix.get(x, z))
						message::play::send::unloadChunk(this, x + chunkX, z + chunkZ);

			for (int i = 0; i > delta; i--)
				chunkLoaderHelper.matrix.Shift_positive_z();
		}

		chunkLoaderHelper.Reset();
		chunkZ = newChunkZ;
		chunkChanged = true;
		//chunkLoaderHelper.matrix.Show();
	}

	if (chunkChanged) message::play::send::updateViewPosition(this, chunkX, chunkZ);

	Player::x = X;
	Player::y = Y;
	Player::z = Z;
}
void Player::updateRotation(bfloat yaw, bfloat pitch)
{
	Player::yaw = yaw;
	Player::pitch = pitch;
}
bool Player::positionInRange(Position location)
{
	int cX = location.x() >> 4;
	int cZ = location.z() >> 4;
	if (cX < chunkX - viewDistance || cX > chunkX + viewDistance ||
		cZ < chunkZ - viewDistance || cZ > chunkZ + viewDistance) return false;
	return true;
}

Player::_inventory::_inventory()
{
	for (int i = 0; i < 46; i++)
		slots[i] = new Slot();

	floatingItem = new Slot();
}

Player::_inventory::~_inventory()
{
	for (int i = 0; i < 46; i++)
		delete slots[i];

	delete floatingItem;
}

void Player::_inventory::setSelectedSlot(bshort selectedSlot)
{
	this->selectedHotbar = selectedSlot;
}

bshort Player::_inventory::getSelectedIndex(bool raw)
{
	if (!raw)
		return this->selectedHotbar;

	return 36 + this->selectedHotbar;
}

Slot*& Player::_inventory::getSelectedSlot()
{
	return this->slots[36 + selectedHotbar];
}

Slot*& Player::_inventory::getOffhandSlot()
{
	return this->slots[45];
}

Slot*& Player::_inventory::getHotbarSlot(bshort index)
{
	return this->slots[36 + index];
}

Slot*& Player::_inventory::getInventorySlot(bshort index)
{
	return this->slots[index];
}

Slot*& Player::_inventory::getFloatingSlot()
{
	return this->floatingItem;
}
void Player::_inventory::setFloatingSlot(Slot* newSlot)
{
	delete this->floatingItem;
	this->floatingItem = newSlot;
}

bshort Player::_inventory::getSlotWithLeastID(varInt itemID)
{
	bshort indexMin = -1, minCount = MAXSHORT;
	for (int i = 36; i < 45; i++)
	{
		if (slots[i]->getItemId() == itemID && slots[i]->count != 64 && slots[i]->count < minCount)
		{ //change 64 to item maximum regarding to that item
			minCount = slots[i]->count;
			indexMin = i;
		}
	}

	for (int i = 9; i < 36; i++)
	{
		if (slots[i]->getItemId() == itemID && slots[i]->count != 64 && slots[i]->count < minCount)
		{ //change 64 to item maximum regarding to that item
			minCount = slots[i]->count;
			indexMin = i;
		}
	}

	return indexMin;
}
bshort Player::_inventory::getFreeSlot()
{
	for (int i = 36; i < 45; i++)
	{
		if (!this->slots[i]->isPresent())
			return i;
	}

	for (int i = 9; i < 36; i++)
	{
		if (!this->slots[i]->isPresent())
			return i;
	}

	return -1;
}

unsigned Player::_inventory::add(Slot& theItem, unsigned& addedIndex)
{
	Byte picked = 0;

	bshort index, stackableSize = items::getStackableSize(theItem.getItemId());

	if (stackableSize == 1)
	{
		index = this->getFreeSlot();

		if (index != -1)
		{
			picked = theItem.count;

			this->setInventorySlot(index, new Slot(theItem));

			addedIndex = index;
		}

		return picked;
	}

	index = this->getSlotWithLeastID(theItem.getItemId());

	if (index != -1)
	{
		Slot*& containedSlot = this->getInventorySlot(index);

		if (containedSlot->count + theItem.count < stackableSize + 1)
		{
			picked = theItem.count;
			containedSlot->count = containedSlot->count + theItem.count;
		}
		else
		{
			picked = stackableSize - containedSlot->count;
			containedSlot->count = (Byte)stackableSize;
		}

		addedIndex = index;
	}
	else
	{
		index = this->getFreeSlot();

		if (index != -1)
		{
			picked = theItem.count;

			this->setInventorySlot(index, new Slot(theItem));

			addedIndex = index;
		}
	}

	return picked;
}
void Player::_inventory::swapSlots(bshort a, bshort b)
{
	Slot* aSlot = this->getInventorySlot(a);
	Slot* bSlot = this->getInventorySlot(b);

	std::swap(aSlot, bSlot);

	Slot* newA = new Slot(*aSlot); //idfk
	Slot* newB = new Slot(*bSlot);
	this->setInventorySlot(a, newA);
	this->setInventorySlot(b, newB);
}

void Player::_inventory::setInventorySlot(bshort index, Slot* slot)
{
	delete this->slots[index];
	this->slots[index] = slot;
}

unsigned Player::_windower::open(window::type theWindow)
{
	this->que.emplace(std::make_pair(theWindow, indexer));
	return indexer++;
}
void Player::_windower::close(unsigned theID)
{
	if (theID == this->que.front().second)
	{
		this->que.pop();
		indexer--;
	}
	else
		Log::warn() << "Player closed a not-last-opened window." << Log::endl;
}
window::type Player::_windower::getLatest(unsigned theID)
{
	if (theID == this->que.back().second)
		return que.back().first;

	Log::warn() << "Player got a wrong ID latest." << Log::endl;
	return que.back().first;
}

void Player::teleport(bdouble tpX, bdouble tpY, bdouble tpZ)
{
	x = tpX;
	y = tpY;
	z = tpZ;
	message::play::send::playerPosAndLook(this, tpX, tpY, tpZ, yaw, pitch, 0, false);
	Player::updatePosition(tpX, tpY, tpZ);
}
void Player::teleport(bdouble tpX, bdouble tpY, bdouble tpZ, bfloat tpYaw, bfloat tpPitch)
{
	yaw = tpYaw;
	pitch = tpPitch;
	teleport(tpX, tpY, tpZ);
}

void Player::setWorld(World* world, const sf::Vector3<bdouble>* spawnPosition, const sf::Vector2f* spawnOrientation)
{
	Log::debug(DEBUG_SIGHT) << '\n' << this << " is entering world " << world->name;
	sizeof(*spawnPosition);
	//set world and position
	this->world = world;
	if (spawnPosition == nullptr)
	{
		x = world->spawn.X;
		y = world->spawn.Y;
		z = world->spawn.Z;
	}
	else
	{
		x = spawnPosition->x;
		y = spawnPosition->y;
		z = spawnPosition->z;
	}
	chunkX = fastfloor(x) >> 4;
	chunkZ = fastfloor(z) >> 4;
	if (spawnOrientation == nullptr)
	{
		yaw = world->spawn.Yaw;
		pitch = world->spawn.Pitch;
	}
	else
	{
		yaw = spawnOrientation->x;
		pitch = spawnOrientation->y;
	}

	//send necessary packets
	message::play::send::updateViewPosition(this, chunkX, chunkZ);

	message::play::send::timeUpdate(this, 6000i64, 6000i64);

	message::play::send::spawnPosition(this, Position(x, y, z), 0.f);

	/*for (int x = -viewDistance; x <= viewDistance; x++) for (int z = -viewDistance; z <= viewDistance; z++)
	{
		Log::debug(true) << "Sending chunk " << x + chunkX << ' ' << z + chunkZ << '\n';
		message::play::send::chunkDataAndLight(this, x + chunkX, z + chunkZ, true);
		chunkLoaderHelper.matrix.set(x, z, true);
		//message::play::send::updateLight(this, x, z);
		//message::play::send::chunkData(this, x, z);
	}*/

	message::play::send::playerPosAndLook(this, x, y, z, yaw, pitch, 0, false);

	//send the selected slot
	message::play::send::heldItemChange(this, (Byte)inventory->getSelectedIndex());

	//send old items
	//...

	//check for players in sight
	for (Player* otherP : world->players)
	{
		if (otherP->state == ConnectionState::play && Position::inRange(otherP->chunkX, otherP->chunkZ, chunkX, chunkZ, viewDistance))
		{
			enterSight(otherP);
			otherP->enterSight(this);
		}
	}
	//add the player to the world's player list
	world->players.emplace_back(this);
	Log::debug(DEBUG_SIGHT) << "Player list for " << world->name << " is now " << world->players.size() << Log::endl;
}
void Player::leaveWorld(World* world)
{
	Log::debug(DEBUG_SIGHT) << "Player " << this << " is leaving world " << world->name << Log::endl;
	//unload chunks in the world
	for (int x = chunkX - viewDistance; x <= chunkX + viewDistance; x++) for (int z = chunkZ - viewDistance; z <= chunkZ + viewDistance; z++) ignoreExceptions(world->unload(x, z));
	chunkLoaderHelper.Reset();
	chunkLoaderHelper.matrix.Empty();

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
	Log::debug(DEBUG_SIGHT) << "Player list of " << world->name << " is now " << world->players.size() << Log::endl;
}
void Player::changeWorld(World* newWorld, const sf::Vector3<bdouble>* spawnPosition, const sf::Vector2f* spawnOrientation)
{
	message::play::send::respawn(this, newWorld->characteristics, newWorld->name, 0x5f19a34be6c9129a, gm, gamemode::none, false, newWorld->isFlat, true);

	leaveWorld(world);

	setWorld(newWorld, spawnPosition, spawnOrientation);
}
void Player::changeWorld(const mcString& worldName, const sf::Vector3<bdouble>* spawnPosition, const sf::Vector2f* spawnOrientation)
{
	for (uint i = 0; i < World::worlds.size(); i++) if (World::worlds[i]->name == worldName) changeWorld(World::worlds[i], spawnPosition, spawnOrientation);
}

void Player::enterSight(Player* other)
{
	Log::debug(DEBUG_SIGHT) << "Player " << other << " entering sight of " << this << Log::endl;
	ignoreExceptions(message::play::send::spawnPlayer(this, other->getEid(), *other->euuid, other->x, other->y, other->z, (float)other->yaw, (float)other->pitch));
	ignoreExceptions(message::play::send::entityHeadLook(this, other->getEid(), (float)other->yaw));
	seenBy.emplace_back(other);
	Log::debug(DEBUG_SIGHT) << "Sight of " << this << " is now " << seenBy.size() << Log::endl;
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
	Log::debug(DEBUG_SIGHT) << "Player " << seenBy[otherI] << " exiting sight of " << this << Log::endl;
	ignoreExceptions(message::play::send::destroyEntities(this, 1, &seenBy[otherI]->eid));
	seenBy.erase(seenBy.begin() + otherI);
	Log::debug(DEBUG_SIGHT) << "Sight of " << this << " is now " << seenBy.size() << Log::endl;
}

void Player::disconnect()
{
	socket->disconnect();
	connected = false;
	if (state == ConnectionState::play)
	{
		leaveWorld(world);
		Log::info() << username << " (" << netId() << ", eid: " << eid << ") disconnected.\n";
		Player::eidDispenser.Free(eid);
		broadcastChat(Chat((username + " left the game").c_str(), Chat::color::yellow()), this);
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
		if (!username.empty()) Log::info() << username << " was timed out.\n";
		else Log::info() << netId() << " was timed out.\n";
		return;
	}

	//try sending a chunk
	while (!chunkLoaderHelper.Finished())
	{
		sf::Vector2i pos = chunkLoaderHelper.Next();
		if (chunkLoaderHelper.matrix.get(pos.x, pos.y)) continue;
		message::play::send::chunkDataAndLight(this, chunkX + pos.x, chunkZ + pos.y);
		chunkLoaderHelper.matrix.set(pos.x, pos.y, true);
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
			ull sent{};
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

void Player::UpdateViewDistance(int newViewDistance)
{
	int maxViewDistance = Options::viewDistance();
	if (newViewDistance > maxViewDistance) newViewDistance = maxViewDistance;
	else if (newViewDistance < viewDistance)
	{
		//unload chunks
		for (int x = viewDistance; x > newViewDistance; x--) //+x and -x edges
			for (int z = -viewDistance; z <= viewDistance; z++)
			{
				if (chunkLoaderHelper.matrix.get(x, z)) message::play::send::unloadChunk(this, chunkX + x, chunkZ + z);
				if (chunkLoaderHelper.matrix.get(-x, z)) message::play::send::unloadChunk(this, chunkX - x, chunkZ + z);
			}
		for (int z = viewDistance; z > newViewDistance; z--) //+z and -z edges
			for (int x = -newViewDistance; x <= newViewDistance; x++)
			{
				if (chunkLoaderHelper.matrix.get(x, z)) message::play::send::unloadChunk(this, chunkX + x, chunkZ + z);
				if (chunkLoaderHelper.matrix.get(x, -z)) message::play::send::unloadChunk(this, chunkX + x, chunkZ - z);
			}
	}
	viewDistance = newViewDistance;
	chunkLoaderHelper.UpdateViewDistance(viewDistance);
	//chunkLoaderHelper.matrix.Show();
}