#include "slot.h"

void Slot::write(char*& buffer) const {
	*(buffer++) = present;

	if (present) {
		itemId.write(buffer);
		*(buffer++) = count;
		nbt_data->write(buffer);
	}
}

void Slot::read(char*& buffer) {
	present = *(buffer++);

	if (present) {
		itemId.read(buffer);
		count = *(buffer++);
		nbt_data->read(buffer);
	}
}
