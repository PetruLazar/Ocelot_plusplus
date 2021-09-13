#include "console.h"

#include <Windows.h>
#include <stdio.h>

bool ServerConsole::hasConsole = false;

void ServerConsole::AllocCosole()
{
	if (!AllocConsole()) return;
	SetConsoleTitleA("Minecraft Server");
	freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
	hasConsole = true;
}
bool ServerConsole::HasConsole() { return hasConsole; }
void ServerConsole::FreeConsole()
{
	if (!hasConsole) return;
	fclose(stdout);
	fclose(stdin);
	fclose(stderr);
	::FreeConsole();
	hasConsole = false;
}