#include "slot.h"

void Slot::write(char*& buffer) const {
	*(buffer++) = present;

	if (present != 0) {
		itemId.write(buffer);
		*(buffer++) = count;
		nbt_data->write(buffer);
	}
}
