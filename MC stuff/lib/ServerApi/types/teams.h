#pragma once
#include "chat.h"
#include "mcString.h"
#include "varData.h"

enum class teamsUpdateMode { createTeam = 0, removeTeam, updateTeamInfo, addEntitiesToTeam, removeEntitiesFromTeam };

namespace teamsUpdate
{
	class mode
	{
	private:

	public:
		virtual void write(char*& buffer) const = 0;
	};

	class createTeam : mode
	{
	private:
		Chat teamDisplayName, teamPrefix, teamSufix;
		Byte friendlyFlags;
		mcString nameTagVisibility, collisionRule;
		varInt teamColor, entityCount;
		mcString* entities;

	public:
		createTeam(Chat teamDisplayName, Chat teamPrefix, Chat teamSufix, Byte friendlyFlags, mcString nameTagVisibility, mcString collisionRule, varInt teamColor, varInt entityCount, mcString* entities)
			: teamDisplayName(teamDisplayName), teamPrefix(teamPrefix), teamSufix(teamSufix), friendlyFlags(friendlyFlags), nameTagVisibility(nameTagVisibility), collisionRule(collisionRule), teamColor(teamColor), entityCount(entityCount), entities(entities) {};

		void write(char*& buffer) const;
	};

	class removeTeam : mode
	{
	private:

	public:
		void write(char*& buffer) const;
	};

	class updateTeamInfo : mode
	{
	private:
		Chat teamDisplayName, teamPrefix, teamSufix;
		Byte friendlyFlags;
		mcString nameTagVisibility, collisionRule;
		varInt teamColor;

	public:
		updateTeamInfo(Chat teamDisplayName, Chat teamPrefix, Chat teamSufix, Byte friendlyFlags, mcString nameTagVisibility, mcString collisionRule, varInt teamColor)
			: teamDisplayName(teamDisplayName), teamPrefix(teamPrefix), teamSufix(teamSufix), friendlyFlags(friendlyFlags), nameTagVisibility(nameTagVisibility), collisionRule(collisionRule), teamColor(teamColor) {};

		void write(char*& buffer) const;
	};

	class addEntitiesToTeam : mode
	{
	private:
		varInt entityCount;
		mcString* entities;

	public:
		addEntitiesToTeam(varInt entityCount, mcString* entities)
			: entityCount(entityCount), entities(entities) {};

		void write(char*& buffer) const;
	};

	class removeEntitiesFromTeam : mode
	{
	private:
		varInt entityCount;
		mcString* entities;

	public:
		removeEntitiesFromTeam(varInt entityCount, mcString* entities)
			: entityCount(entityCount), entities(entities) {};

		void write(char*& buffer) const;
	};
}