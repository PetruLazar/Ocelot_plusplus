#pragma once
#include "endian.h"
#include "chat.h"

namespace map {
	enum class iconType {
		whiteArrow = 0,
		greenArrow,
		redArrow,
		blueArrow,
		whiteCross,
		redPointer,
		whiteCircle,
		smallWhiteCircle,
		mansion,
		temple,
		whiteBanner,
		orangeBanner,
		magentaBanner,
		lightBlueBanner,
		yellowBanner,
		limeBanner,
		pinkBanner,
		grayBanner,
		lightGrayBanner,
		cyanBanner,
		purpleBanner,
		blueBanner,
		brownBanner,
		greenBanner,
		redBanner,
		blackBanner,
		treasureMarker
	};

	class icon
	{
	private:
		iconType type;
		Byte x, z, direction;
		bool hasDisplayName = false;
		Chat displayName;

	public:
		icon(iconType type, Byte x, Byte z, Byte direction) : type(type), x(x), z(z), direction(direction) {}

		icon(iconType type, Byte x, Byte z, Byte direction, bool hasDisplayName, Chat displayName) 
			: type(type), x(x), z(z), direction(direction), hasDisplayName(hasDisplayName), displayName(displayName) {}

		void write(char*& buffer) const;
	};
}