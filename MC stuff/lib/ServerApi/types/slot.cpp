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

		nbt::tag tagType;
		tagType = (nbt::tag) * (buffer++);
		if (tagType == nbt::tag::End) /*nbt_data is empty*/;
		else if (tagType == nbt::tag::Compound) nbt_data->read(buffer);
		else throw std::exception("Invalid slot nbt");
	}
}

Byte Slot::getStackableSize(Slot* slot) {
	return getStackableSize(*slot);
}
Byte Slot::getStackableSize(const Slot& slot) {
	if (slot.nbt_data->getSize() != 0)
		return 1;

	return items::getStackableSize(slot.getItemId());
}