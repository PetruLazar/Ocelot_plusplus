#include "nbt_byte.h"

nbt_Byte::nbt_Byte(const std::string& name, char v) : nbt(tag::Byte, name), value(v) { }

void nbt_Byte::write(std::fstream& os, bool iNT) const
{
	if (iNT)
	{
		os.write((char*)&type, sizeof(type));

		writeName(os);
	}

	os.write(&value, sizeof(value));
}
void nbt_Byte::read(std::fstream& is, const std::string& name)
{
	std::streampos begin = is.tellg(), end;

	if (name == "") readName(is);
	else nbt::name = name;

	is.read(&value, 1);
}
void nbt_Byte::write(char*& buffer, bool iNT) const
{
	if (iNT)
	{
		*(buffer++) = static_cast<char>(type);

		writeName(buffer);
	}

	*(buffer++) = value;
}
void nbt_Byte::read(char*& end, const std::string& name)
{
	char* buffer = end;
	if (name == "") readName(end);
	else nbt::name = name;

	value = *(end++);
}
std::string nbt_Byte::getStringValue() const
{
	return std::to_string(value) + 'b';
}
char& nbt_Byte::vByte() { return value; }
nbt_Byte::operator char& () { return value; }
void nbt_Byte::operator= (const nbt& that)
{
	if (that.type != type) throw typeError;
	operator=((const nbt_Byte&)that);
}
void nbt_Byte::operator=(const nbt_Byte& that)
{
	name = that.name;
	value = that.value;
}
void nbt_Byte::operator=(char v) { value = v; }