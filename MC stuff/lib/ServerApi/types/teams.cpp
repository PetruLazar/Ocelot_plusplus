
#include "teams.h"

void teamsUpdate::createTeam::write(char*& buffer) const {
	teamDisplayName.write(buffer);
	*(buffer++) = friendlyFlags;
	nameTagVisibility.write(buffer);
	collisionRule.write(buffer);
	teamColor.write(buffer);
	teamPrefix.write(buffer);
	teamSufix.write(buffer);
	entityCount.write(buffer);

	for (int i = 0; i < entityCount; i++)
		entities[i].write(buffer);
}

void teamsUpdate::removeTeam::write(char*& buffer) const {

}

void teamsUpdate::updateTeamInfo::write(char*& buffer) const {
	teamDisplayName.write(buffer);
	*(buffer++) = friendlyFlags;
	nameTagVisibility.write(buffer);
	collisionRule.write(buffer);
	teamColor.write(buffer);
	teamPrefix.write(buffer);
	teamSufix.write(buffer);
}

void teamsUpdate::addEntitiesToTeam::write(char*& buffer) const {
	entityCount.write(buffer);

	for (int i = 0; i < entityCount; i++)
		entities[i].write(buffer);
}

void teamsUpdate::removeEntitiesFromTeam::write(char*& buffer) const {
	entityCount.write(buffer);

	for (int i = 0; i < entityCount; i++)
		entities[i].write(buffer);
}