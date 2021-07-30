#include "nbt_long_array.h"
nbt_long_array::nbt_long_array(std::string name) : nbt(LongArray, name), values(nullptr), _size(0) { }
nbt_long_array::nbt_long_array(int64* v, int s, std::string name) : nbt(LongArray, name), values((blong*)v), _size(s) { }
nbt_long_array::~nbt_long_array()
{
	delete values;
	values = nullptr;
	_size = 0;
}
void nbt_long_array::write(std::fstream& os, bool iNT)
{
	if (iNT)
	{
		os.write((char*)&type, sizeof(type));

		writeName(os);
	}

	_size.write(os);

	for (uint i = 0; i < _size; i++)
	{
		values[i].write(os);
	}
}
void nbt_long_array::read(std::fstream& is, std::string name)
{
	if (values)
	{
		delete[] values;
	}

	std::streampos begin = is.tellg();

	if (name == "") readName(is);
	else nbt::name = name;

	_size.read(is);

	values = new bigEndian<int64>[_size];
	for (uint i = 0; i < _size; i++) values[i].read(is);
}
void nbt_long_array::write(char*& buffer, bool iNT)
{
	if (iNT)
	{
		*(buffer++) = type;

		writeName(buffer);
	}

	_size.write(buffer);
	for (uint i = 0; i < _size; i++) values[i].write(buffer);
}
void nbt_long_array::read(char*& end, std::string name)
{
	if (values) delete[] values;

	char* buffer = end;

	if (name == "") readName(end);
	else nbt::name = name;

	_size.read(end);

	values = new blong[_size];
	for (uint i = 0; i < _size; i++) values[i].read(end);
}
std::string nbt_long_array::getStringValue()
{
	std::string ret = "[";

	if (!_size)
	{
		ret += "]l";
		return ret;
	}

	ret += std::to_string(values[0]);
	for (uint i = 1; i < _size; i++) ret += ',' + std::to_string(values[i]);
	ret += "]l";
	return ret;
}
/*std::string nbt_long_array::to_string()
{

}*/
int64& nbt_long_array::vLong(uint i)
{
	if (i >= _size) throw outOfBoundsError;
	return values[i];
}
int64& nbt_long_array::operator[](uint i)
{
	if (i >= _size) throw outOfBoundsError;
	return values[i];
}
uint nbt_long_array::getSize() { return _size; }
void nbt_long_array::resize(uint newSize)
{
	blong* newValues = new blong[newSize];

	uint minSize = newSize < _size ? newSize : (uint)_size;
	for (uint i = 0; i < minSize; i++) newValues[i] = values[i];

	delete[] values;
	values = newValues;
	_size = newSize;
}