#include "nbt_short.h"
nbt_short::nbt_short(const std::string& name) : nbt(Short, name) { }
nbt_short::nbt_short(short v, const std::string& name) : nbt(Short, name), value(v) { }
//nbt_short::~nbt_short() { }
void nbt_short::write(std::fstream& os, bool iNT) const
{
	if (iNT)
	{
		os.write((char*)&type, sizeof(type));

		writeName(os);
	}

	value.write(os);
}
void nbt_short::read(std::fstream& is, const std::string& name)
{
	std::streampos begin = is.tellg();

	if (name == "") readName(is);
	else nbt::name = name;

	value.read(is);
}
void nbt_short::write(char*& buffer, bool iNT) const
{
	if (iNT)
	{
		*(buffer++) = type;

		writeName(buffer);
	}

	value.write(buffer);
}
void nbt_short::read(char*& end, const std::string& name)
{
	char* buffer = end;

	if (name == "") readName(end);
	else nbt::name = name;

	value.read(end);
}
std::string nbt_short::getStringValue() const
{
	return std::to_string(value) + 's';
}
short& nbt_short::vShort() { return value; }
nbt_short::operator short& () { return value; }
void nbt_short::operator=(short v) { value = v; }