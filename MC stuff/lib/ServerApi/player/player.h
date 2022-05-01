#pragma once
#include "../world.h"
#include "../types/uuid.h"
#include "../types/enums.h"
#include "SFML/Network/TcpSocket.hpp"
#include "SFML/System/Vector2.hpp"
#include "../types/chat.h"
#include "../types/queue.h"
#include "../types/entity.h"
#include "../types/slot.h"
#include "../types/window.h"

#include <queue>

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
#include <iostream>

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

	class ChunkLoaderHelper
	{
		int v, // viewDistance
			a, // current loop radius
			i, // current index in each edge
			e; // current edge

		bool enabled = false;

		void ResetEdge();
		void ResetIndex();
		void ResetRadius();

		bool EdgeOverflow();
		bool IndexOverflow();
		bool RadiusOverflow();

		void AdvanceEdge();
		void AdvanceIndex();
		void AdvanceRadius();

		sf::Vector2i Generate();

	public:
		ChunkLoaderHelper(int viewDistance);
		void UpdateViewDistance(int viewDistance);

		void Reset();
		sf::Vector2i Next();
		bool Finished() const;

		class ChunkMatrix
		{
		public:
			using bitsettype = uint;

		private:
			int viewDistance;
			class SubMatrix
			{

				Byte width;
				std::vector<bitsettype> values;

			public:
				SubMatrix(Byte height, Byte width);

				void resize(Byte newHeight, Byte newWidth);
				bool get(Byte i, Byte j) const;
				void set(Byte i, Byte j, bool v);

				Byte Height() const;
				Byte Width() const;

				void Shift_px(bitsettype val);
				//call for every x
				void Shift_pz(Byte x, bool val);
				bitsettype Shift_nx();
				//call for every x
				bool Shift_nz(Byte x);

				void Empty();
			} pos_pos, pos_neg, neg_pos, neg_neg;

		public:
			ChunkMatrix(int viewDistance);

			void resize(int newViewDistance);
			bool get(int x, int z) const;
			void set(int x, int z, bool v);

			int ViewDistance() const;

			void Shift_positive_x();
			void Shift_positive_z();
			void Shift_negative_x();
			void Shift_negative_z();

			void Empty();

			/*void Show()
			{
				int viewDistance = ViewDistance();
				for (int i = -viewDistance; i <= viewDistance; i++)
				{
					for (int j = -viewDistance; j <= viewDistance; j++) std::cout << get(i, j) << ' ';
					std::cout << '\n';
				}
				std::cout << '\n';
			}*/
		} matrix;
	} chunkLoaderHelper;

public:
	static constexpr auto maxViewDistance = sizeof(ChunkLoaderHelper::ChunkMatrix::bitsettype) * 8 - 1;

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

	class _inventory
	{
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

		Slot*& getFloatingSlot();
		void setFloatingSlot(Slot* newSlot);

		bshort getSlotWithLeastID(varInt itemID);
		bshort getFreeSlot();

		unsigned add(Slot& theItem, unsigned& addedIndex);
		void swapSlots(bshort a, bshort b);

		void setInventorySlot(bshort index, Slot* slot);
	} *inventory;

	class _windower
	{
	private:
		std::queue<std::pair<window::type, unsigned>> que;
		unsigned indexer = 1;

	public:
		unsigned open(window::type theWindow);
		void close(unsigned ID);

		window::type getLatest(unsigned ID);
	} *windower;

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
	void UpdateViewDistance(int newViewDistance);

	SERVER_API Player(sf::TcpSocket*);
	SERVER_API ~Player();

	SERVER_API void setWorld(World* world, const sf::Vector3<bdouble>* spawnPosition = nullptr, const sf::Vector2f* spawnOrientation = nullptr);
	SERVER_API void leaveWorld(World* world);
	SERVER_API void changeWorld(World* newWorld, const sf::Vector3<bdouble>* spawnPosition = nullptr, const sf::Vector2f* spawnOrientation = nullptr);
	SERVER_API void changeWorld(const mcString& worldName, const sf::Vector3<bdouble>* spawnPosition = nullptr, const sf::Vector2f* spawnOrientation = nullptr);

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