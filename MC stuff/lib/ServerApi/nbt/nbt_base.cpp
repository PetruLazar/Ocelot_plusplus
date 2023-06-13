#include "nbt_base.h"
#include "../nbt.h"

nbt* nbt::getTagP(tag t)
{
	switch (t)
	{
	case tag::End:
		return nullptr;
	case tag::Byte:
		return new nbt_Byte;
	case tag::Short:
		return new nbt_short;
	case tag::Int:
		return new nbt_int;
	case tag::Long:
		return new nbt_long;
	case tag::Float:
		return new nbt_float;
	case tag::Double:
		return new nbt_double;
	case tag::ByteArray:
		return new nbt_Byte_array;
	case tag::String:
		return new nbt_string;
	case tag::List:
		return new nbt_list;
	case tag::Compound:
		return new nbt_compound("", nullptr);
	case tag::IntArray:
		return new nbt_int_array;
	case tag::LongArray:
		return new nbt_long_array;
	}
	throw typeError;
}
/*nbt** nbt::getTagArray(tag t, int s)
{
	switch (t)
	{
	case End:
		return nullptr;
	case Byte:
		return new nbt_Byte * [s];
	case Short:
		return new nbt_short*[s];
	case Int:
		return new nbt_int*[s];
	case Long:
		return new nbt_long*[s];
	case Float:
		return new nbt_float*[s];
	case Double:
		return new nbt_double*[s];
	case ByteArray:
		return new nbt_Byte_array*[s];
	case String:
		return new nbt_string*[s];
	case List:
		return new nbt_list*[s];
	case Compound:
		return new nbt_compound*[s];
	case IntArray:
		return new nbt_int_array*[s];
	case LongArray:
		return new nbt_long_array*[s];
	}
	return nullptr;
}*/
bool nbt::checkTag(std::fstream& is, tag t)
{
	tag ft = tag::End;
	is.read((char*)&ft, sizeof(ft));
	return ft == t;
}
bool nbt::checkTag(char*& buffer, tag t)
{
	tag ft;
	ft = (tag) * (buffer++);
	return ft == t;
}

nbt::nbt(tag t, std::string name) : type(t), name(name) { }
nbt::~nbt() { }
std::string nbt::getName() const { return name; }
nbt::tag nbt::getType() const { return type; }
void nbt::readName(std::istream& is)
{
	bigEndian<ush> nameSize;
	nameSize.read(is);

	char* cname = new char[nameSize];
	is.read(cname, nameSize);
	name.assign(cname, nameSize);
	delete[] cname;
}
void nbt::writeName(std::ostream& os) const
{
	bigEndian<ush> nameSize = (ush)name.length();
	nameSize.write(os);

	os.write(name.c_str(), nameSize);
}
void nbt::readName(char*& buffer)
{
	bigEndian<ush> nameSize;
	nameSize.read(buffer);

	name.assign(buffer, nameSize);

	buffer += nameSize;
}
void nbt::writeName(char*& buffer) const
{
	bigEndian<ush> nameSize = (ush)name.length();
	nameSize.write(buffer);

	for (ush i = 0; i < nameSize; i++) *(buffer++) = name[i];
}
char& nbt::vByte() { throw typeError; }
short& nbt::vShort() { throw typeError; }
int& nbt::vInt() { throw typeError; }
int64& nbt::vLong() { throw typeError; }
float& nbt::vFloat() { throw typeError; }
double& nbt::vDouble() { throw typeError; }
char& nbt::vByte(uint) { throw typeError; }
std::string& nbt::vString() { throw typeError; }
nbt& nbt::vTag(uint) { throw typeError; }
nbt& nbt::vTag(const std::string&) { throw typeError; }
int& nbt::vInt(uint) { throw typeError; }
int64& nbt::vLong(uint) { throw typeError; }
uint nbt::getSize() const { throw typeError; }
void nbt::resize(uint) { throw typeError; }
void nbt::remove(nbt*) { throw typeError; }
void nbt::remove(const std::string&) { throw typeError; }
void nbt::add(nbt*) { throw typeError; }
nbt::tag nbt::getElemType() const { throw typeError; }
void nbt::changeType(tag) { throw typeError; }

std::string nbt::to_string() const
{
	if (!name.empty()) return name + ':' + getStringValue();
	return getStringValue();
}
void nbt::operator=(const nbt&) { throw typeError; }
void nbt::operator=(char) { throw typeError; }
void nbt::operator=(short) { throw typeError; }
void nbt::operator=(int) { throw typeError; }
void nbt::operator=(int64) { throw typeError; }
void nbt::operator=(float) { throw typeError; }
void nbt::operator=(double) { throw typeError; }
void nbt::operator=(const std::string&) { throw typeError; }
void nbt::operator=(const char*) { throw typeError; }

nbt& nbt::operator[](const std::string&) { throw typeError; }