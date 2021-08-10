#pragma once
#include "../json.h"

class Chat : public json_compound
{
public:

	enum color
	{
		black,
		dark_blue = 0xaa,
		dark_green = 0xaa00,
		dark_aqua = 0xaaaa,
		dark_red = 0xaa0000,
		dark_purple = 0xaa00aa,
		gold = 0xffaa00,
		gray = 0xaaaaaa,
		dark_gray = 0x555555,
		blue = 0x5555ff,
		green = 0x55ff55,
		aqua = 0x55ffff,
		red = 0xff5555,
		light_purple = 0xff55ff,
		yellow = 0xffff55,
		white = 0xffffff
	};

	Chat(const char* text);
	Chat(const char* text, color cl);
};

