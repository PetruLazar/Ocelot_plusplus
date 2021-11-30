#include "chat.h"
#include "../types/utils.h"

using namespace std;

Chat::optionalComponent::optionalComponent() {}
Chat::optionalComponent::optionalComponent(json* comp) : comps({ comp }) {}
Chat::optionalComponent& Chat::optionalComponent::operator|(const optionalComponent& other)
{
	comps.emplace_back(other.comps[0]);
	return *this;
}
void Chat::optionalComponent::addCompsToChat(Chat& chat) const
{
	for (json* comp : comps) chat.add(comp);
}

const Chat::optionalComponent Chat::optionalComponent::empty;

Chat::color::color(const string& rawText) : optionalComponent(new json_string("color", rawText)) {}
Chat::color::color(Byte red, Byte green, Byte blue) : optionalComponent(new json_string("color", '#' + to_hexstring(red) + to_hexstring(green) + to_hexstring(blue))) {}
Chat::color::color(const std::string& red, const std::string& green, const std::string& blue) : optionalComponent(new json_string("color", '#' + red + green + blue)) {}

Chat::color::black::black() : optionalComponent(new json_string("color", "black")) {}
Chat::color::dark_blue::dark_blue() : optionalComponent(new json_string("color", "dark_blue")) {}
Chat::color::dark_green::dark_green() : optionalComponent(new json_string("color", "dark_green")) {}
Chat::color::dark_aqua::dark_aqua() : optionalComponent(new json_string("color", "dark_aqua")) {}
Chat::color::dark_red::dark_red() : optionalComponent(new json_string("color", "dark_red")) {}
Chat::color::dark_purple::dark_purple() : optionalComponent(new json_string("color", "dark_purple")) {}
Chat::color::gold::gold() : optionalComponent(new json_string("color", "gold")) {}
Chat::color::gray::gray() : optionalComponent(new json_string("color", "gray")) {}
Chat::color::dark_gray::dark_gray() : optionalComponent(new json_string("color", "dark_gray")) {}
Chat::color::blue::blue() : optionalComponent(new json_string("color", "blue")) {}
Chat::color::green::green() : optionalComponent(new json_string("color", "green")) {}
Chat::color::aqua::aqua() : optionalComponent(new json_string("color", "aqua")) {}
Chat::color::red::red() : optionalComponent(new json_string("color", "red")) {}
Chat::color::light_purple::light_purple() : optionalComponent(new json_string("color", "light_purple")) {}
Chat::color::yellow::yellow() : optionalComponent(new json_string("color", "yellow")) {}
Chat::color::white::white() : optionalComponent(new json_string("color", "white")) {}
Chat::color::reset::reset() : optionalComponent(new json_string("color", "reset")) {}

Chat::style::obfuscated::obfuscated(bool b) : optionalComponent(new json_boolean("obfuscated", b)) {}
Chat::style::bold::bold(bool b) : optionalComponent(new json_boolean("bold", b)) {}
Chat::style::strikethrough::strikethrough(bool b) : optionalComponent(new json_boolean("strikethrough", b)) {}
Chat::style::underlined::underlined(bool b) : optionalComponent(new json_boolean("underlined", b)) {}
Chat::style::italic::italic(bool b) : optionalComponent(new json_boolean("italic", b)) {}

Chat::Chat() :
	json_compound("", new json*[1]{
			new json_string("text", "")
		}, 1)
{}

Chat::Chat(const char* text, optionalComponent opt) :
	json_compound("", new json*[1]{
			new json_string("text",text)
		}, 1)
{
	opt.addCompsToChat(*this);
}

/*Chat::Chat(const char* text, color cl, style st) :
	json_compound("", new json*[2]{
			new json_string("text",text),
			new json_string("color", cl.text)
		}, 2)
{}*/