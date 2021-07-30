#include "nbt_double.h"
nbt_double::nbt_double(std::string name) : nbt(Double, name) { }
nbt_double::nbt_double(double v, std::string name) : nbt(Double, name), value(v) { }
//nbt_double::~nbt_double() { }
void nbt_double::write(std::fstream& os, bool iNT)
{
	if (iNT)
	{
		os.write((char*)&type, sizeof(type));

		writeName(os);
	}

	value.write(os);
}
void nbt_double::read(std::fstream& is, std::string name)
{
	std::streampos begin = is.tellg();

	if (name == "") readName(is);
	else nbt::name = name;

	value.read(is);
}
void nbt_double::write(char*& buffer, bool iNT)
{
	if (iNT)
	{
		*(buffer++) = type;

		writeName(buffer);
	}

	value.write(buffer);
}
void nbt_double::read(char*& end, std::string name)
{
	char* buffer = end;

	if (name == "") readName(end);
	else nbt::name = name;

	value.read(end);
}
std::string nbt_double::getStringValue()
{
	return std::to_string(value);
}
/*std::string nbt_double::to_string()
{

}*/
double& nbt_double::vDouble() { return value; }
nbt_double::operator double& () { return value; }