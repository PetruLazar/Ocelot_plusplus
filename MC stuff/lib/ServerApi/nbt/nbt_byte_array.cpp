#include "nbt_byte_array.h"

nbt_Byte_array::nbt_Byte_array(const std::string& name, char* v, int s) : nbt(tag::ByteArray, name), values(v), _size(s) { }
nbt_Byte_array::~nbt_Byte_array()
{
	delete values;
	values = nullptr;
	_size = 0;
}

void nbt_Byte_array::write(std::ostream& os, bool iNT) const
{
	if (iNT)
	{
		os.write((char*)&type, sizeof(type));

		writeName(os);
	}

	_size.write(os);

	os.write(values, _size);
}
void nbt_Byte_array::read(std::istream& is, const std::string& name)
{
	std::streampos begin = is.tellg();

	if (values)
	{
		delete[] values;
	}

	if (name == "") readName(is);
	else nbt::name = name;

	_size.read(is);

	values = new char[_size];
	is.read(values, _size);
}
void nbt_Byte_array::write(char*& buffer, bool iNT) const
{
	if (iNT)
	{
		*(buffer++) = static_cast<char>(type);

		writeName(buffer);
	}

	_size.write(buffer);

	for (uint i = 0; i < _size; i++) *(buffer++) = values[i];
}
void nbt_Byte_array::read(char*& end, const std::string& name)
{
	char* buffer = end;

	if (values) delete[] values;

	if (name == "") readName(end);
	else nbt::name = name;

	_size.read(end);

	values = new char[_size];
	for (uint i = 0; i < _size; i++) values[i] = *(end++);
}
std::string nbt_Byte_array::getStringValue() const
{
	std::string ret = "[";

	if (!_size)
	{
		ret += "]b";
		return ret;
	}

	ret += std::to_string(values[0]);
	for (uint i = 1; i < _size; i++) ret += ',' + std::to_string(values[i]);
	ret += "]b";
	return ret;
}
/*std::string nbt_Byte_array::to_string()
{

}*/
char& nbt_Byte_array::vByte(uint i)
{
	if (i >= _size) throw outOfBoundsError;
	return values[i];
}
char& nbt_Byte_array::operator [] (uint i)
{
	if (i >= _size) throw outOfBoundsError;
	return values[i];
}
uint nbt_Byte_array::getSize() const { return _size; }
void nbt_Byte_array::resize(uint newSize)
{
	char* newValues = new char[newSize];

	uint minSize = newSize < _size ? newSize : (uint)_size;
	for (uint i = 0; i < minSize; i++) newValues[i] = values[i];

	delete[] values;
	values = newValues;
	_size = newSize;
}
void nbt_Byte_array::operator=(const nbt& that)
{
	if (that.type != type) throw typeError;
	operator=((const nbt_Byte_array&)that);
}
void nbt_Byte_array::operator=(const nbt_Byte_array& that)
{
	name = that.name;

	_size = that._size;
	values = new char[_size];
	for (uint i = 0; i < _size; i++) 
		values[i] = that.values[i];
}