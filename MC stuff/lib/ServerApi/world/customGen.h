#pragma once
#include "../world.h"

extern "C" _declspec(dllexport) Chunk * generate(World * wld, int chunkX, int chunkZ);