#include "nbt_int.h"
nbt_int::nbt_int(std::string name) : nbt(Int, name) { }
nbt_int::nbt_int(int v, std::string name) : nbt(Int, name), value(v) { }
//nbt_int::~nbt_int() { }
void nbt_int::write(std::fstream& os, bool iNT)
{
	if (iNT)
	{
		os.write((char*)&type, sizeof(type));

		writeName(os);
	}

	value.write(os);
}
void nbt_int::read(std::fstream& is, std::string name)
{
	std::streampos begin = is.tellg();

	if (name == "") readName(is);
	else nbt::name = name;

	value.read(is);
}
void nbt_int::write(char*& buffer, bool iNT)
{
	if (iNT)
	{
		*(buffer++) = type;

		writeName(buffer);
	}

	value.write(buffer);
}
void nbt_int::read(char*& end, std::string name)
{
	char* buffer = end;

	if (name == "") readName(end);
	else nbt::name = name;

	value.read(end);
}
std::string nbt_int::getStringValue()
{
	return std::to_string(value);
}
/*std::string nbt_int::to_string()
{

}*/
int& nbt_int::vInt() { return value; }
nbt_int::operator int& () { return value; }