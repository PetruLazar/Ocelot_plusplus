#include "nbt_string.h"
nbt_string::nbt_string(std::string name) : nbt(String, name) { }
nbt_string::nbt_string(std::string v, std::string name) : nbt(String), value(v) { }
//nbt_string::~nbt_string() { }
void nbt_string::write(std::fstream& os, bool iNT)
{
	if (iNT)
	{
		os.write((char*)&type, sizeof(type));

		writeName(os);
	}

	bigEndian<ush> size = (ush)value.length();

	size.write(os);
	os.write(value.c_str(), size);
}
void nbt_string::read(std::fstream& is, std::string name)
{
	std::streampos begin = is.tellg();

	if (name == "") readName(is);
	else nbt::name = name;

	bigEndian<ush> size;
	size.read(is);

	char* cstr = new char[size];
	is.read(cstr, size);
	value.assign(cstr, size);

	delete[] cstr;
}
void nbt_string::write(char*& buffer, bool iNT)
{
	if (iNT)
	{
		*(buffer++) = type;

		writeName(buffer);
	}
	bigEndian<ush> size = (ush)value.length();

	size.write(buffer);
	for (ush i = 0; i < size; i++) *(buffer++) = value[i];
}
void nbt_string::read(char*& end, std::string name)
{
	char* buffer = end;

	if (name == "") readName(end);
	else nbt::name = name;

	bigEndian<ush> size;
	size.read(end);

	value.assign(end, size);
	end += size;
}
std::string nbt_string::getStringValue()
{
	return "\"" + value + "\"";
}
/*std::string nbt_string::to_string()
{

}*/
std::string& nbt_string::vString() { return value; }
nbt_string::operator std::string& () { return value; }