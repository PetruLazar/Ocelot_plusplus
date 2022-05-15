#pragma once

#include "../types/apidef.h"
#include "../../ThreadPool/ThreadPool.h"

#define versionName "v0.1.0"

class Server
{
	static bool hasConsole;
public:
	static ThreadPool threadPool;
	SERVER_API static bool keepServerOpen;
	SERVER_API static bool restartOnClose;

	SERVER_API static void AllocConsole();
	SERVER_API static bool HasConsole();
	SERVER_API static void FreeConsole();
};