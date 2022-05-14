#include "server.h"

#include <Windows.h>
#include <stdio.h>
#include <conio.h>

#include <thread>

bool Server::hasConsole = false;
bool Server::keepServerOpen = true;
bool Server::restartOnClose = false;

ThreadPool Server::threadPool(std::thread::hardware_concurrency());

void Server::AllocConsole()
{
	if (!::AllocConsole()) return;
	SetConsoleTitleA("Minecraft Server");
	freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
	hasConsole = true;
}
bool Server::HasConsole() { return hasConsole; }
void Server::FreeConsole()
{
	if (!hasConsole) return;
	fclose(stdout);
	fclose(stdin);
	fclose(stderr);
	::FreeConsole();
	_kbhit();
	hasConsole = false;
}
