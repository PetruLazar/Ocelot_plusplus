#include "queue.h"

MessageBuffer::Iterator::Value::Value(char* msg, ull bufferSize) : msg(msg), buffer(msg), bufferSize(bufferSize) { }
MessageBuffer::Iterator::Value::~Value() { delete msg; }
MessageBuffer::Iterator::Iterator(char* msg, ull size, Iterator* next) : data(msg, size), next(next) { }

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
void MessageBuffer::push(char* msg, size_t size)
{
	Iterator* it = new Iterator(msg, size);

	if (!first) first = it;
	else last->next = it;
	last = it;
}
bool MessageBuffer::empty()
{
	return !first;
}