#include "nbt_int.h"

nbt_int::nbt_int(const std::string& name, int v) : nbt(Int, name), value(v) { }

void nbt_int::write(std::fstream& os, bool iNT) const
{
	if (iNT)
	{
		os.write((char*)&type, sizeof(type));

		writeName(os);
	}

	value.write(os);
}
void nbt_int::read(std::fstream& is, const std::string& name)
{
	std::streampos begin = is.tellg();

	if (name == "") readName(is);
	else nbt::name = name;

	value.read(is);
}
void nbt_int::write(char*& buffer, bool iNT) const
{
	if (iNT)
	{
		*(buffer++) = type;

		writeName(buffer);
	}

	value.write(buffer);
}
void nbt_int::read(char*& end, const std::string& name)
{
	char* buffer = end;

	if (name == "") readName(end);
	else nbt::name = name;

	value.read(end);
}
std::string nbt_int::getStringValue() const
{
	return std::to_string(value);
}
int& nbt_int::vInt() { return value; }
nbt_int::operator int& () { return value; }
void nbt_int::operator= (int v) { value = v; }