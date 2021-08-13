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