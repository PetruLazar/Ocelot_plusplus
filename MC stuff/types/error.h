#pragma once

class protocolError
{
public:
	const char* msg;

	protocolError(const char* msg);
};

class protocolWarning
{
public:
	const char* msg;

	protocolWarning(const char* msg);
};

class runtimeError
{
public:
	const char* msg;

	runtimeError(const char* msg);
};

class runtimeWarning
{
public:
	const char* msg;

	runtimeWarning(const char* msg);
};