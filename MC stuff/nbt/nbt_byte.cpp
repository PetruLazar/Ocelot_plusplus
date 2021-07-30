#include "nbt_byte.h"
nbt_byte::nbt_byte(std::string name) : nbt(Byte, name) { }
nbt_byte::nbt_byte(char v, std::string name) : nbt(Byte, name), value(v) { }
//nbt_byte::~nbt_byte() { }
void nbt_byte::write(std::fstream& os, bool iNT)
{
	if (iNT)
	{
		os.write((char*)&type, sizeof(type));

		writeName(os);
	}

	os.write(&value, sizeof(value));
}
void nbt_byte::read(std::fstream& is, std::string name)
{
	std::streampos begin = is.tellg(), end;

	if (name == "") readName(is);
	else nbt::name = name;

	is.read(&value, 1);
}
void nbt_byte::write(char*& buffer, bool iNT)
{
	if (iNT)
	{
		*(buffer++) = type;

		writeName(buffer);
	}

	*(buffer++) = value;
}
void nbt_byte::read(char*& end, std::string name)
{
	char* buffer = end;
	if (name == "") readName(end);
	else nbt::name = name;

	value = *(end++);
}
std::string nbt_byte::getStringValue()
{
	return std::to_string(value) + 'b';
}
/*std::string nbt_byte::to_string()
{

}*/
char& nbt_byte::vByte() { return value; }
nbt_byte::operator char& () { return value; }