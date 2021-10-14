#pragma once
#include "mcString.h"
#include "varData.h"
#include "chat.h"
#include "slot.h"

class advancement
{
private:
	bool hasParent, hasDisplay;
	mcString parentId;
	advancementDisplay displayData;
	varInt nOfCriteria;
	mcString* criteriaIdentifiers;
	//void* criteriaValue; ?
	varInt arraySetLength;
	varInt* arrayLength;
	mcString** criteriaRequirements;

public:
	void write(char*& buffer) const;
};

class advancementDisplay
{
public:
	enum class frameType { task = 0, challenge, goal };

private:
	Chat title, description;
	Slot icon;
	frameType frame;
	bint flags;
	mcString backgroundTexture;
	bfloat x, y;

public:
	advancementDisplay(Chat title, Chat decription, Slot icon, frameType frame, bint flags, mcString backgroundTexture, bfloat x, bfloat y)
		: title(title), description(description), icon(icon), frame(frame), flags(flags), backgroundTexture(backgroundTexture), x(x), y(y) {}

	void write(char*& buffer) const;
};

class criterionProgress
{
private:
	bool achieved;
	blong dateOfAchieving;

public:
	criterionProgress(bool achieved, long dateOfAchieving = 0) : achieved(achieved), dateOfAchieving(dateOfAchieving) {}

	void write(char*& buffer) const;
};

class advancementProgress
{
private:
	varInt size;
	mcString* identifiers;
	criterionProgress* criterions;

public:
	advancementProgress(varInt size, mcString* identifiers, criterionProgress* criterions)
		: size(size), identifiers(identifiers), criterions(criterions) {}

	void write(char*& buffer) const;
};
