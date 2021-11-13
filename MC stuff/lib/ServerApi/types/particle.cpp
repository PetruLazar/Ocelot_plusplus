
#include "particle.h"

void particle::ParticleEnumed::write(char*& buffer, bool withID) const {
	theType.write(buffer);
}

void particle::block::write(char*& buffer, bool withID) const {
	if (withID)
		varInt(4).write(buffer);

	blockState.write(buffer);
}

void particle::dust::write(char*& buffer, bool withID) const {
	if (withID)
		varInt(15).write(buffer);

	red.write(buffer);
	green.write(buffer);
	blue.write(buffer);
	scale.write(buffer);
}

void particle::dustColorTransition::write(char*& buffer, bool withID) const {
	if (withID)
		varInt(16).write(buffer);
	
	fromRed.write(buffer);
	fromGreen.write(buffer);
	fromBlue.write(buffer);
	scale.write(buffer);
	toRed.write(buffer);
	toGreen.write(buffer);
	toBlue.write(buffer);
}

void particle::fallingDust::write(char*& buffer, bool withID) const {
	if (withID)
		varInt(25).write(buffer);
	
	blockState.write(buffer);
}

void particle::item::write(char*& buffer, bool withID) const {
	if (withID)
		varInt(36).write(buffer);
	
	slotItem->write(buffer);
}

void particle::vibration::write(char*& buffer, bool withID) const {
	if (withID)
		varInt(37).write(buffer);

	originX.write(buffer);
	originY.write(buffer);
	originZ.write(buffer);
	destX.write(buffer);
	destY.write(buffer);
	destZ.write(buffer);
	ticks.write(buffer);
}