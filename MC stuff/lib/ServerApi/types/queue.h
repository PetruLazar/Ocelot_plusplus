#pragma once
#include "apidef.h"
#include "typedefs.h"

class MessageBuffer
{
public:
	enum MessageType
	{
		raw,
		light,
		chunk,
		disconnect
	};

	struct Iterator
	{
		MessageType msgType;
		union Value
		{
		public:
			struct Raw
			{
				char* buffer, * toDelete;
				size_t bufferSize;

				Raw(char* msg, size_t size, char* toDelete);
			} raw;
			struct Chunk
			{
				int x, z;

				Chunk(int chunkX, int chunkZ);
			} chunk;
			struct Disconnect
			{

			} disconnect;
			//no data for scheduled disconnect

			Value(char* msg, size_t size, char* toDelete);
			Value(int chunkX, int chunkZ);
			Value();
		} data;
		Iterator* next;

		//constructor for raw message type
		Iterator(char* msg, size_t size, char* toDelete, Iterator* next = nullptr);
		//constructor for chunk message type
		Iterator(int chunkX, int chunkZ, Iterator* next = nullptr);
		//constructor for scheduled disconnect 
		Iterator(Iterator* next = nullptr);
		~Iterator();
	};

	Iterator* first, * last;

	SERVER_API MessageBuffer();
	SERVER_API ~MessageBuffer();
	SERVER_API void pop();
	SERVER_API void push(char* msg, size_t size, char* toDelete);
	SERVER_API void push(int chunkX, int chunkZ);
	SERVER_API void push();
	SERVER_API bool empty();
};

