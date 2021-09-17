#pragma once

#include "../types/apidef.h"

class ServerConsole
{
	static bool hasConsole;
public:
	SERVER_API static void AllocCosole();
	SERVER_API static bool HasConsole();
	SERVER_API static void FreeConsole();
};