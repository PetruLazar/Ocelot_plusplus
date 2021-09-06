#include "debug.h"

#ifdef DEBUG_ALLOCATIONS

#include "server/log.h"
#include <iostream>

//decomment this and select a block id if needed, or comment if you are not targeting a specific id
#define TARGET_MEMORY_BLOCK 1078

void* operator new(size_t s)
{
	void* p = malloc(s);
	int bid = ((int*)p)[-2];
#ifdef TARGET_MEMORY_BLOCK
	if (bid == TARGET_MEMORY_BLOCK)
		std::cout << "";
#endif
	//if (!log::txt().is_open())  
	//	log::initialize();
	//log::txt() << "Allocated object at: " << p << " (size " << s << ", block id " << bid << ")\n";
	//else std::cout << "Allocated object at: " << p << " (size " << s << ", block id " << bid << ")\n";
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
	//if (!log::txt().is_open())  
	//	log::initialize();
	//log::txt() << "Allocated block at:  " << p << " (size " << s << ", block id " << bid << ")\n";
	//else std::cout << "Allocated block at:  " << p << " (size " << s << ", block id " << bid << ")\n";
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
	//if (!log::txt().is_open())  
	//	log::initialize();
	//log::txt() << "Freed object at:     " << p << " (size " << s << ", block id " << bid << ")\n";
}
void operator delete[](void* p, size_t s)
{
	int bid = ((int*)p)[-2];
#ifdef TARGET_MEMORY_BLOCK
	if (bid == TARGET_MEMORY_BLOCK)
		std::cout << "";
#endif
	free(p);
	//if (!log::txt().is_open())  
	//	log::initialize();
	//log::txt() << "Freed block at:      " << p << " (size " << s << ", block id " << bid << ")\n";
}

#endif