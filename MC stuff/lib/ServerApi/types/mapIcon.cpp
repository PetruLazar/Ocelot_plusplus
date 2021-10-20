
#include "mapIcon.h"
#include "varData.h"

void map::icon::write(char*& buffer) const {
	varInt((int)type).write(buffer);
	*(buffer++) = x;
	*(buffer++) = z;
	*(buffer++) = direction;
	*(buffer++) = hasDisplayName;
	if (hasDisplayName)
		displayName.write(buffer);
}