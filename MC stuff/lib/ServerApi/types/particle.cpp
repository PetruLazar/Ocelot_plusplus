
#include "particle.h"

void particle_class::block::write(char*& buffer) const {
	blockState.write(buffer);
}

void particle_class::dust::write(char*& buffer) const {
	red.write(buffer);
	green.write(buffer);
	blue.write(buffer);
	scale.write(buffer);
}

void particle_class::dustColorTransition::write(char*& buffer) const {
	fromRed.write(buffer);
	fromGreen.write(buffer);
	fromBlue.write(buffer);
	scale.write(buffer);
	toRed.write(buffer);
	toGreen.write(buffer);
	toBlue.write(buffer);
}

void particle_class::fallingDust::write(char*& buffer) const {
	blockState.write(buffer);
}

void particle_class::item::write(char*& buffer) const {
	slotItem->write(buffer);
}

void particle_class::vibration::write(char*& buffer) const {
	originX.write(buffer);
	originY.write(buffer);
	originZ.write(buffer);
	destX.write(buffer);
	destY.write(buffer);
	destZ.write(buffer);
	ticks.write(buffer);
}