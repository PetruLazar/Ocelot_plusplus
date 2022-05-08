#pragma once
#include "../types/apidef.h"

#define __FILENAME__ strrchr("\\" __FILE__, '\\') + 1

//set to true/false to switch between custom new operator and usual new operator
#define DEBUG_ALLOCATIONS false

//set to true/false to turn on or off chunk debug
#define CHUNK_DEBUG false

//set to true/false to turn on or off region debug
#define REGION_DEBUG false

//set to true/false to turn on or off the "unhandled packet" and "partially handled" warnings
#define PROTOCOL_WARNINGS true

//set to true/false to turn on or off the monitoring and logging of the amounts of data sent/received over the network
#define NETWORK_TRAFFIC_DEBUG false

//set to true/false to turn on or off the logging of the outgoing message buffering
#define LOG_PACKETS false

//set to true/false to turn on or off the debugging of player sightings
#define DEBUG_SIGHT false

//set to true/false to enable world loading debug
#define WORLD_LOAD_DEBUG false

//set to true/false to log all conections or not
#define LOG_ALL_CONNECTIONS true

//set to true/false to enable/disable server status request logigng
#define LOG_STATUS_REQ false

#if DEBUG_ALLOCATIONS
void* operator new(size_t s);
void* operator new[](size_t s);

void operator delete(void*, size_t);
void operator delete[](void*, size_t);
#endif

#if CHUNK_DEBUG
#define IF_CHUNK_DEBUG(x) x
#else
#define IF_CHUNK_DEBUG(x)
#endif

#if REGION_DEBUG
#define IF_REGION_DEBUG(x) x
#else
#define IF_REGION_DEBUG(x)
#endif

#if PROTOCOL_WARNINGS
#define IF_PROTOCOL_WARNINGS(x) x
#else
#define IF_PROTOCOL_WARNINGS(x)
#endif

#if NETWORK_TRAFFIC_DEBUG
#define IF_NETWORK_TRAFFIC_DEBUG(x) x
#else
#define IF_NETWORK_TRAFFIC_DEBUG(x)
#endif

#if DEBUG_SIGHT
#define IF_DEBUG_SIGHT(x) x
#else
#define IF_DEBUG_SIGHT(x)
#endif

#if WORLD_LOAD_DEBUG
#define IF_WORLD_LOAD_DEBUG(x) x
#else
#define IF_WORLD_LOAD_DEBUG(x)
#endif

#if LOG_ALL_CONNECTIONS
#define IF_LOG_ALL_CONNECTIONS(x) x
#else 
#define IF_LOG_ALL_CONNECTIONS(x)
#endif

#if LOG_STATUS_REQ
#define IF_LOG_STATUS_REQ(x) x
#else
#define IF_LOG_STATUS_REQ(x)
#endif