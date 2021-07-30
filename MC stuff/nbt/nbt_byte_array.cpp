#include "nbt_byte_array.h"
nbt_byte_array::nbt_byte_array(std::string name) : nbt(ByteArray, name), values(nullptr), _size(0) { }
nbt_byte_array::nbt_byte_array(char* v, int s, std::string name) : nbt(ByteArray, name), values(v), _size(s) { }
nbt_byte_array::~nbt_byte_array()
{
	delete values;
	values = nullptr;
	_size = 0;
}
void nbt_byte_array::write(std::fstream& os, bool iNT)
{
	if (iNT)
	{
		os.write((char*)&type, sizeof(type));

		writeName(os);
	}

	_size.write(os);

	os.write(values, _size);
}
void nbt_byte_array::read(std::fstream& is, std::string name)
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
void nbt_byte_array::write(char*& buffer, bool iNT)
{
	if (iNT)
	{
		*(buffer++) = type;

		writeName(buffer);
	}

	_size.write(buffer);

	for (uint i = 0; i < _size; i++) *(buffer++) = values[i];
}
void nbt_byte_array::read(char*& end, std::string name)
{
	char* buffer = end;

	if (values) delete[] values;

	if (name == "") readName(end);
	else nbt::name = name;

	_size.read(end);

	values = new char[_size];
	for (uint i = 0; i < _size; i++) values[i] = *(end++);
}
std::string nbt_byte_array::getStringValue()
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
/*std::string nbt_byte_array::to_string()
{

}*/
char& nbt_byte_array::vByte(uint i)
{
	if (i >= _size) throw outOfBoundsError;
	return values[i];
}
char& nbt_byte_array::operator [] (uint i)
{
	if (i >= _size) throw outOfBoundsError;
	return values[i];
}
uint nbt_byte_array::getSize() { return _size; }
void nbt_byte_array::resize(uint newSize)
{
	char* newValues = new char[newSize];

	uint minSize = newSize < _size ? newSize : (uint)_size;
	for (uint i = 0; i < minSize; i++) newValues[i] = values[i];

	delete[] values;
	values = newValues;
	_size = newSize;
}