#include "nbt_long.h"
nbt_long::nbt_long(std::string name) : nbt(Long, name) { }
nbt_long::nbt_long(int64 v, std::string name) : nbt(Long, name), value(v) { }
//nbt_long::~nbt_long() { }
void nbt_long::write(std::fstream& os, bool iNT)
{
	if (iNT)
	{
		os.write((char*)&type, sizeof(type));

		writeName(os);
	}

	value.write(os);
}
void nbt_long::read(std::fstream& is, std::string name)
{
	std::streampos begin = is.tellg();

	if (name == "") readName(is);
	else nbt::name = name;

	value.read(is);
}
void nbt_long::write(char*& buffer, bool iNT)
{
	if (iNT)
	{
		*(buffer++) = type;

		writeName(buffer);
	}

	value.write(buffer);
}
void nbt_long::read(char*& end, std::string name)
{
	char* buffer = end;

	if (name == "") readName(end);
	else nbt::name = name;

	value.read(end);
}
std::string nbt_long::getStringValue()
{
	return std::to_string(value) + 'l';
}
/*std::string nbt_long::to_string()
{

}*/
int64& nbt_long::vLong() { return value; }
nbt_long::operator int64& () { return value; }