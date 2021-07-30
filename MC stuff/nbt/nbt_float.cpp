#include "nbt_float.h"
nbt_float::nbt_float(std::string name) : nbt(Float, name) { }
nbt_float::nbt_float(float v, std::string name) : nbt(Float, name), value(v) { }
//nbt_float::~nbt_float() { }
void nbt_float::write(std::fstream& os, bool iNT)
{
	if (iNT)
	{
		os.write((char*)&type, sizeof(type));

		writeName(os);
	}

	value.write(os);
}
void nbt_float::read(std::fstream& is, std::string name)
{
	std::streampos begin = is.tellg();

	if (name == "") readName(is);
	else nbt::name = name;

	value.read(is);
}
void nbt_float::write(char*& buffer, bool iNT)
{
	if (iNT)
	{
		*(buffer++) = type;

		writeName(buffer);
	}

	value.write(buffer);
}
void nbt_float::read(char*& end, std::string name)
{
	char* buffer = end;

	if (name == "") readName(end);
	else nbt::name = name;

	value.read(end);
}
std::string nbt_float::getStringValue()
{
	return std::to_string(value) + 'f';
}
/*std::string nbt_float::to_string()
{

}*/
float& nbt_float::vFloat() { return value; }
nbt_float::operator float& () { return value; }