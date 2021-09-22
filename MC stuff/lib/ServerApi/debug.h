#pragma once
#include "types/apidef.h"

//comment or decomment this define to switch between custom new operator and usual new operator
//#define DEBUG_ALLOCATIONS

//comment or decomment this define to turn on or off chunk debug
//#define CHUNK_DEBUG

//comment or decomment this define to turn on or off region debug
//#define REGION_DEBUG

//comment or decomment this define to turn on or off the "unhandled packet" and "partially handled" warnings
//#define PROTOCOL_WARNINGS

//comment or decomment this define to turn on or off the monitoring and logging of the amounts of data sent/received over the network
//#define NETWORK_TRAFFIC_DEBUG

//comment or decomment this define to turn on or off the logging of the outgoing message buffering
//#define LOG_PACKETS

//comment or decomment this define to turn on or off the debugging of player sightings
//#define DEBUG_SIGHT

#ifdef DEBUG_ALLOCATIONS

void* operator new(size_t s);
void* operator new[](size_t s);

void operator delete(void*, size_t);
void operator delete[](void*, size_t);

#endif

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

#ifdef PROTOCOL_WARNINGS
#define IF_PROTOCOL_WARNINGS(x) x
#else
#define IF_PROTOCOL_WARNINGS(x)
#endif

#ifdef NETWORK_TRAFFIC_DEBUG
#define IF_NETWORK_TRAFFIC_DEBUG(x) x
#else
#define IF_NETWORK_TRAFFIC_DEBUG(x)
#endif

#ifdef LOG_MESSAGE_BUFFER
#define IF_LOG_MESSAGE_BUFFER(x) x
#else
#define IF_LOG_MESSAGE_BUFFER(x)
#endif

#ifdef DEBUG_SIGHT
#define IF_DEBUG_SIGHT(x) x
#else
#define IF_DEBUG_SIGHT(x)
#endif