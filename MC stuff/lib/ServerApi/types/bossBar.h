#pragma once
#include "varData.h"
#include "chat.h"
#include "endian.h"

namespace bossBar {
	enum class action { add = 0, remove, updateHealth, updateTitle, updateStyle, updateFlags};

	enum class color { pink = 0, blue, red, green, yellow, purple, white };
	enum class divisions { none = 0, six, ten, twelve, twenty };

	class mode
	{
	public:
		virtual void write(char*& buffer) const = 0;
	};

	class add : mode
	{
	private:
		Chat title;
		bfloat health;
		color barColor;
		divisions barDivisions;
		Byte flags; //wiki says unsigned byte

	public:
		add(Chat title, bfloat health, color barColor, divisions barDivisions, Byte flags)
			: title(title), health(health), barColor(barColor), barDivisions(barDivisions), flags(flags) {}

		void write(char*& buffer) const;
	};

	class remove : mode
	{
	private:

	public:
		void write(char*& buffer) const;
	};

	class updateHealth : mode
	{
	private:
		bfloat health;

	public:
		updateHealth(bfloat health) : health(health) {}

		void write(char*& buffer) const;
	};

	class updateTitle : mode
	{
	private:
		Chat title;

	public:
		updateTitle(Chat title) : title(title) {}

		void write(char*& buffer) const;
	};

	class updateStyle : mode
	{
	private:
		color barColor;
		divisions barDivisions;

	public:
		updateStyle(color barColor, divisions barDivisions) : barColor(barColor), barDivisions(barDivisions) {}

		void write(char*& buffer) const;
	};

	class updateFlags : mode
	{
	private:
		Byte flags; //wiki says unsigned byte

	public:
		updateFlags(Byte flags) : flags(flags) {}

		void write(char*& buffer) const;
	};
}