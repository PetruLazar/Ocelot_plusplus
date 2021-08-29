#pragma once

#include "../common.h"

#ifdef NBTLIB_EXPORTS
#define NBT_API _declspec(dllexport)
#else
#define NBT_API _declspec(dllimport)
#endif