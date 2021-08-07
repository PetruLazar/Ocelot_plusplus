#include "uuid.h"

void mcUUID::write(char*& buffer) const
{
	//123e4567-e89b-12d3-a456-426614174000
	*(((ull*&)buffer)++) = 0x123e4567e89b32d3;
	*(((ull*&)buffer)++) = 0xa456426614174000;
}