#include "nbt_float.h"

nbt_float::nbt_float(const std::string& name, float v) : nbt(tag::Float, name), value(v) { }

void nbt_float::write(std::ostream& os, bool iNT) const
{
	if (iNT)
	{
		os.write((char*)&type, sizeof(type));

		writeName(os);
	}

	value.write(os);
}
void nbt_float::read(std::istream& is, const std::string& name)
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
		*(buffer++) = static_cast<char>(type);

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
void nbt_float::operator=(const nbt& that)
{
	if (that.type != type) throw typeError;
	operator=((const nbt_float&)that);
}
void nbt_float::operator=(const nbt_float& that)
{
	name = that.name;
	value = that.value;
}
void nbt_float::operator=(float v) { value = v; }