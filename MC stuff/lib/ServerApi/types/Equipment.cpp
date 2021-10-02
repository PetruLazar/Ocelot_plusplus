
#include "Equipment.h"

void Equipment::write(char*& buffer) const {
	*(buffer++) = slot;
	item->write(buffer);
}