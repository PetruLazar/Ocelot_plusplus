#pragma once

#include "../common.h"

#ifdef BASICDEFS_EXPORTS
#define BASICDEFS_API _declspec(dllexport)
#else
#define BASICDEFS_API _declspec(dllimport)
#endif