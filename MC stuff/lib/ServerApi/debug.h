#pragma once
#include "types/apidef.h"

//comment or decomment this define to switch between custom new operator and usual new operator (not functional for now)
//#define DEBUG_ALLOCATIONS
#ifdef DEBUG_ALLOCATIONS

void* operator new(size_t s);
void* operator new[](size_t s);

void operator delete(void*, size_t);
void operator delete[](void*, size_t);

#endif

//comment or decomment this define to turn on or off chunk debug
//#define CHUNK_DEBUG

//comment or decomment this define to turn on or off region debug
//#define REGION_DEBUG

//comment or decomment this define to turn on or off the "unhandled packet" and "partially handled" warnings
//#define PROTOCOL_WARNINGS

//comment or decomment this define to turn on or off the monitoring and logging of the amounts of data sent/received over the network
//#define MONITOR_CONNECTIONS

//comment or decomment this define to turn on or off the logging of the outgoing message buffering
//#define LOG_MESSAGE_BUFFER

#ifdef CHUNK_DEBUG
#define IF_CHUNK_DEBUG(x) x
#else
#define IF_CHUNK_DEBUG(x)
#endif

#ifdef REGION_DEBUG
#define IF_REGION_DEBUG(x) x
#else
#define IF_REGION_DEBUG(x)
#endif

#ifdef UNHANDLED_PACKETS
#define IF_LOG_UNHANDLED(x) x
#else
#define IF_LOG_UNHANDLED(x)
#endif

#ifdef MONITOR_CONNECTIONS
#define IF_MONITOR_CONNECTIONS(x) x
#else
#define IF_MONITOR_CONNECTIONS(x)
#endif

#ifdef LOG_MESSAGE_BUFFER
#define IF_LOG_MESSAGE_BUFFER(x) x
#else
#define IF_LOG_MESSAGE_BUFFER(x)
#endif