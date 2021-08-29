#pragma once
#include "apidef.h"

class protocolError
{
public:
	const char* msg;

	BASICDEFS_API protocolError(const char* msg);
};

class protocolWarning
{
public:
	const char* msg;

	BASICDEFS_API protocolWarning(const char* msg);
};

class runtimeError
{
public:
	const char* msg;

	BASICDEFS_API runtimeError(const char* msg);
};

class runtimeWarning
{
public:
	const char* msg;

	BASICDEFS_API runtimeWarning(const char* msg);
};