#pragma once

#ifdef SERVERAPI_EXPORTS
#define SERVER_API _declspec(dllexport)
#else
#define SERVER_API _declspec(dllimport)
#endif