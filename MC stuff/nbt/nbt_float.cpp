#include "nbt_float.h"
nbt_float::nbt_float(const std::string& name) : nbt(Float, name) { }
nbt_float::nbt_float(float v, const std::string& name) : nbt(Float, name), value(v) { }
//nbt_float::~nbt_float() { }
void nbt_float::write(std::fstream& os, bool iNT) const
{
	if (iNT)
	{
		os.write((char*)&type, sizeof(type));

		writeName(os);
	}

	value.write(os);
}
void nbt_float::read(std::fstream& is, const std::string& name)
{
	std::streampos begin = is.tellg();

	if (name == "") readName(is);
	else nbt::name = name;

	value.read(is);
}
void nbt_float::write(char*& buffer, bool iNT) const
{
	if (iNT)
	{
		*(buffer++) = type;

		writeName(buffer);
	}

	value.write(buffer);
}
void nbt_float::read(char*& end, const std::string& name)
{
	char* buffer = end;

	if (name == "") readName(end);
	else nbt::name = name;

	value.read(end);
}
std::string nbt_float::getStringValue() const
{
	return std::to_string(value) + 'f';
}
float& nbt_float::vFloat() { return value; }
nbt_float::operator float& () { return value; }
void nbt_float::operator=(float v) { value = v; }