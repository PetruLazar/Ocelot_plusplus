#include "chat.h"
#include "../types/utils.h"

Chat::Chat(const char* text) : 
	json_compound("", new json* [1]{ 
			new json_string("text",text)
		}, 1) { }

Chat::Chat(const char* text, color cl) :
	json_compound("", new json* [2]{
			new json_string("text",text),
			new json_string("color","#" + to_hexstring(static_cast<int>(cl)))
		}, 2) { }