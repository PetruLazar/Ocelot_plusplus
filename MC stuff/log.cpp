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

void* operator new(size_t s)
{
	void* p = malloc(s);
	log::txt() << "Allocated at: " << p << " (size " << s << ")\n";
	return p;
}
void* operator new[](size_t s)
{
	void* p = malloc(s);
	log::txt() << "Allocated at: " << p << " (size " << s << ")\n";
	return p;
}

void operator delete(void* p, size_t s)
{
	free(p);
	log::txt() << "Freed at:     " << p << " (size " << s << ")\n";
}
void operator delete[](void* p, size_t s)
{
	free(p);
	log::txt() << "Freed at:     " << p << " (size " << s << ")\n";
}

#endif