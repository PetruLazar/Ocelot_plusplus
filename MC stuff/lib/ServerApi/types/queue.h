#pragma once
#include "apidef.h"
#include "typedefs.h"

class MessageBuffer
{
public:
	class Iterator
	{
	public:
		class Value
		{
		public:
			char* msg, * buffer;
			size_t bufferSize;

			Value(char* msg, size_t size);
			~Value();
		} data;
		Iterator* next;

		Iterator(char* msg, size_t size, Iterator* next = nullptr);
	};

	Iterator* first, * last;

	SERVER_API MessageBuffer();
	SERVER_API ~MessageBuffer();
	SERVER_API void pop();
	SERVER_API void push(char* msg, size_t size);
	SERVER_API bool empty();
};

