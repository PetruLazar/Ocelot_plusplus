#include "nbt_double.h"

nbt_double::nbt_double(const std::string& name, double v) : nbt(tag::Double, name), value(v) { }

void nbt_double::write(std::fstream& os, bool iNT) const
{
	if (iNT)
	{
		os.write((char*)&type, sizeof(type));

		writeName(os);
	}

	value.write(os);
}
void nbt_double::read(std::fstream& is, const std::string& name)
{
	std::streampos begin = is.tellg();

	if (name == "") readName(is);
	else nbt::name = name;

	value.read(is);
}
void nbt_double::write(char*& buffer, bool iNT) const
{
	if (iNT)
	{
		*(buffer++) = static_cast<char>(type);

		writeName(buffer);
	}

	value.write(buffer);
}
void nbt_double::read(char*& end, const std::string& name)
{
	char* buffer = end;

	if (name == "") readName(end);
	else nbt::name = name;

	value.read(end);
}
std::string nbt_double::getStringValue() const
{
	return std::to_string(value);
}
double& nbt_double::vDouble() { return value; }
nbt_double::operator double& () { return value; }
void nbt_double::operator=(double v) { value = v; }