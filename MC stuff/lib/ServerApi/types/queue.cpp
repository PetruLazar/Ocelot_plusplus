#include "queue.h"

MessageBuffer::Iterator::Value::Value(char* msg, ull bufferSize, char* toDelete) : buffer(msg), bufferSize(bufferSize) { }
MessageBuffer::Iterator::Value::~Value() { delete toDelete; }
MessageBuffer::Iterator::Iterator(char* msg, ull size, char* toDelete, Iterator* next) : data(msg, size, toDelete), next(next) { }

MessageBuffer::MessageBuffer() : first(0), last(0) { }
MessageBuffer::~MessageBuffer()
{
	while (first) pop();
}
void MessageBuffer::pop()
{
	Iterator* second = first->next;
	delete first;
	first = second;

	if (!first) last = 0;
}

void MessageBuffer::push(char* msg, size_t size, char* toDelete)
{
	Iterator* it = new Iterator(msg, size, toDelete);

	if (!first) first = it;
	else last->next = it;
	last = it;
}
bool MessageBuffer::empty()
{
	return !first;
}