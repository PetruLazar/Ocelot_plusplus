#include "log.h"

using namespace std;

const char log::binFileName[] = "debug.bin", log::txtFileName[] = "debug.txt";
ofstream log::binFile, log::txtFile;

void log::initialize()
{
	binFile.open(binFileName, /*ios::app |*/ ios::binary);
	txtFile.open(txtFileName /*,ios::app*/);
}
ofstream& log::txt()
{
	return txtFile;
}
void log::bin(const char* data, const ull length)
{
	binFile.write(data, length);
}

#ifdef DEBUG_ALLOCATIONS

/*class all_info
{
public:
	static vector<void*> blocks;
	static uint count;
};
vector <void*> all_info::blocks;
uint all_info::count;*/

//decomment this and select a block id if needed, or comment if you are not targeting a specific id
#define TARGET_MEMORY_BLOCK 237

void* operator new(size_t s)
{
	void* p = malloc(s);
	int bid = ((int*)p)[-2];
#ifdef TARGET_MEMORY_BLOCK
	if (bid == TARGET_MEMORY_BLOCK)
		0;
#endif
	log::txt() << "Allocated object at: " << p << " (size " << s << ", block id " << bid << ")\n";
	return p;
}
void* operator new[](size_t s)
{
	void* p = malloc(s);
	int bid = ((int*)p)[-2];
#ifdef TARGET_MEMORY_BLOCK
	if (bid == TARGET_MEMORY_BLOCK)
		log::txt();
#endif
	log::txt() << "Allocated block at:  " << p << " (size " << s << ", block id " << bid << ")\n";
	return p;
}

void operator delete(void* p, size_t s)
{
	int bid = ((int*)p)[-2];
#ifdef TARGET_MEMORY_BLOCK
	if (bid == TARGET_MEMORY_BLOCK)
		0;
#endif
	free(p);
	log::txt() << "Freed object at:     " << p << " (size " << s << ", block id " << bid << ")\n";
}
void operator delete[](void* p, size_t s)
{
	int bid = ((int*)p)[-2];
#ifdef TARGET_MEMORY_BLOCK
	if (bid == TARGET_MEMORY_BLOCK)
		0;
#endif
	free(p);
	log::txt() << "Freed block at:      " << p << " (size " << s << ", block id " << bid << ")\n";
}

#endif