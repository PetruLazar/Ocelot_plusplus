#include "debug.h"

#if DEBUG_ALLOCATIONS

#include "server/log.h"
#include <iostream>

//decomment this and select a block id if needed, or comment if you are not targeting a specific id
//#define TARGET_MEMORY_BLOCK 1078

void* operator new(size_t s)
{
	void* p = malloc(s);
	int bid = ((int*)p)[-2];
#ifdef TARGET_MEMORY_BLOCK
	if (bid == TARGET_MEMORY_BLOCK)
		std::cout << "";
#endif
	Log::debug(DEBUG_ALLOCATIONS) << "Allocated object at: " << p << " (size " << s << ", block id " << bid << ")" << Log::endl;
	return p;
}
void* operator new[](size_t s)
{
	void* p = malloc(s);
	int bid = ((int*)p)[-2];
#ifdef TARGET_MEMORY_BLOCK
	if (bid == TARGET_MEMORY_BLOCK)
		std::cout << "";
#endif
	Log::debug(DEBUG_ALLOCATIONS) << "Allocated block at:  " << p << " (size " << s << ", block id " << bid << ")" << Log::endl;
	return p;
}

void operator delete(void* p, size_t s)
{
	int bid = ((int*)p)[-2];
#ifdef TARGET_MEMORY_BLOCK
	if (bid == TARGET_MEMORY_BLOCK)
		std::cout << "";
#endif
	free(p);
	Log::debug(DEBUG_ALLOCATIONS) << "Freed object at:     " << p << " (size " << s << ", block id " << bid << ")" << Log::endl;
}
void operator delete[](void* p, size_t s)
{
	int bid = ((int*)p)[-2];
#ifdef TARGET_MEMORY_BLOCK
	if (bid == TARGET_MEMORY_BLOCK)
		std::cout << "";
#endif
	free(p);
	Log::debug(DEBUG_ALLOCATIONS) << "Freed block at:      " << p << " (size " << s << ", block id " << bid << ")" << Log::endl;
}

#endif