
#include "advancements.h"

void advancement::write(char*& buffer) const {
	*(buffer++) = hasParent;
	if (hasParent)
		parentId.write(buffer);

	*(buffer++) = hasDisplay;
	if (hasDisplay)
		displayData.write(buffer);

	nOfCriteria.write(buffer);
	for (int i = 0; i < (int)(&nOfCriteria); i++) {
		criteriaIdentifiers[i].write(buffer);
		//(void) criteriaValue.write(buffer) ?
	}

	arraySetLength.write(buffer);
	for (int i = 0; i < (int)(&arraySetLength); i++) {
		for (int j = 0; j < (int)(&(arrayLength[i])); j++)
			criteriaRequirements[i][j].write(buffer);
	}
}

void advancementDisplay::write(char*& buffer) const {
	title.write(buffer);
	description.write(buffer);
	icon.write(buffer);
	*(buffer++) = (int)frame;
	flags.write(buffer);
	if(flags == 0x01)
		backgroundTexture.write(buffer);
	x.write(buffer);
	y.write(buffer);
}

void criterionProgress::write(char*& buffer) const {
	*(buffer++) = achieved;
	if (achieved)
		dateOfAchieving.write(buffer);
}

void advancementProgress::write(char*& buffer) const {
	size.write(buffer);
	for (int i = 0; i < (int)(&size); i++) {
		identifiers[i].write(buffer);
		criterions[i].write(buffer);
	}
}