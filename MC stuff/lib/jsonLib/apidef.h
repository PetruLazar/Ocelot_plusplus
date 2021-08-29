#pragma once

#ifdef JSONLIB_EXPORTS
#define JSON_API _declspec(dllexport)
#else
#define JSON_API _declspec(dllimport)
#endif