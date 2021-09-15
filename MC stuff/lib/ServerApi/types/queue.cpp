#include "queue.h"

//MessageBuffer::Iterator::Value::Value(char* msg, ull bufferSize, char* toDelete) : buffer(msg), bufferSize(bufferSize) { }

MessageBuffer::Iterator::Value::Raw::Raw(char* msg, size_t size, char* toDelete) : buffer(msg), bufferSize(size), toDelete(toDelete) { }
MessageBuffer::Iterator::Value::Chunk::Chunk(int chunkX, int chunkZ) : x(chunkX), z(chunkZ) { }

MessageBuffer::Iterator::Value::Value(char* msg, ull bufferSize, char* toDelete) : raw(msg, bufferSize, toDelete) { }
MessageBuffer::Iterator::Value::Value(int chunkX, int chunkZ) : chunk(chunkX, chunkZ) { }
MessageBuffer::Iterator::Value::Value() : disconnect() { }
MessageBuffer::Iterator::Iterator(char* msg, ull size, char* toDelete, Iterator* next) : msgType(raw), next(next), data(msg, size, toDelete) { }
MessageBuffer::Iterator::Iterator(int chunkX, int chunkZ, Iterator* next) : msgType(chunk), next(next), data(chunkX, chunkZ) { }
MessageBuffer::Iterator::Iterator(Iterator* next) : msgType(disconnect), next(next), data() { }
MessageBuffer::Iterator::~Iterator()
{
	if (msgType == raw) delete data.raw.toDelete;
}

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
void MessageBuffer::push(int chunkX, int chunkZ)
{
	Iterator* it = new Iterator(chunkX, chunkZ);

	if (!first) first = it;
	else last->next = it;
	last = it;

	it = new Iterator(chunkX, chunkZ);
	it->msgType = MessageType::light;

	if (!first) first = it;
	else last->next = it;
	last = it;
}
void MessageBuffer::push()
{
	Iterator* it = new Iterator();

	if (!first) first = it;
	else last->next = it;
	last = it;
}
bool MessageBuffer::empty()
{
	return !first;
}