#include "nbt_long.h"

nbt_long::nbt_long(const std::string& name, int64 v) : nbt(tag::Long, name), value(v) { }

void nbt_long::write(std::fstream& os, bool iNT) const
{
	if (iNT)
	{
		os.write((char*)&type, sizeof(type));

		writeName(os);
	}

	value.write(os);
}
void nbt_long::read(std::fstream& is, const std::string& name)
{
	std::streampos begin = is.tellg();

	if (name == "") readName(is);
	else nbt::name = name;

	value.read(is);
}
void nbt_long::write(char*& buffer, bool iNT) const
{
	if (iNT)
	{
		*(buffer++) = static_cast<char>(type);

		writeName(buffer);
	}

	value.write(buffer);
}
void nbt_long::read(char*& end, const std::string& name)
{
	char* buffer = end;

	if (name == "") readName(end);
	else nbt::name = name;

	value.read(end);
}
std::string nbt_long::getStringValue() const
{
	return std::to_string(value) + 'l';
}
int64& nbt_long::vLong() { return value; }
nbt_long::operator int64& () { return value; }
void nbt_long::operator= (const nbt& that)
{
	if (that.type != type) throw typeError;
	operator=((const nbt_long&)that);
}
void nbt_long::operator=(const nbt_long& that)
{
	value = that.value;
}
void nbt_long::operator=(int64 v) { value = v; }