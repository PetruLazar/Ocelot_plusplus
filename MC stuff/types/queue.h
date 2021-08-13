#pragma once

class MessageBuffer
{
public:
	struct Iterator
	{
		struct Value
		{
			char* msg;
			size_t size;
		} element;
		Iterator* next, * prev;
	};

private:
	Iterator* first, * last;

public:
	MessageBuffer();
	//MessageBuffer()


};

