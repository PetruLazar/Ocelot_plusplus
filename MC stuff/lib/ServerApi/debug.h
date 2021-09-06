#pragma once
#include "types/apidef.h"

//comment or decomment this define to switch between custom new operator and usual new operator
#define DEBUG_ALLOCATIONS
#ifdef DEBUG_ALLOCATIONS

void* operator new(size_t s);
void* operator new[](size_t s);

void operator delete(void*, size_t);
void operator delete[](void*, size_t);

#endif

//comment or decomment this define to switch to turn on or off chunk debug
//#define CHUNK_DEBUG
#ifdef CHUNK_DEBUG
#define IF_CHUNK_DEBUG(x) x
#else
#define IF_CHUNK_DEBUG(x)
#endif

//comment or decomment this define to switch to turn on or off region debug
//#define REGION_DEBUG
#ifdef REGION_DEBUG
#define IF_REGION_DEBUG(x) x
#else
#define IF_REGION_DEBUG(x)
#endif