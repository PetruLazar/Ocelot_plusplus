#include "nbt_long_array.h"

nbt_long_array::nbt_long_array(const std::string& name, int64* v, int s) : nbt(tag::LongArray, name), values((blong*)v), _size(s) { }
nbt_long_array::nbt_long_array(const std::string& name, const BitArray& bitarray) :
	nbt(tag::LongArray, name),
	_size((uint)bitarray.getCompactedSize())
{
	values = new blong[_size];
	blong* arrayValues = bitarray.getCompactedValues();
	for (uint i = 0; i < _size; i++) values[i] = arrayValues[i];
}
nbt_long_array::~nbt_long_array()
{
	delete[] values;
	values = nullptr;
	_size = 0;
}

void nbt_long_array::write(std::fstream& os, bool iNT) const
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
void nbt_long_array::read(std::fstream& is, const std::string& name)
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
void nbt_long_array::write(char*& buffer, bool iNT) const
{
	if (iNT)
	{
		*(buffer++) = static_cast<char>(type);

		writeName(buffer);
	}

	_size.write(buffer);
	for (uint i = 0; i < _size; i++) values[i].write(buffer);
}
void nbt_long_array::read(char*& end, const std::string& name)
{
	if (values) delete[] values;

	char* buffer = end;

	if (name == "") readName(end);
	else nbt::name = name;

	_size.read(end);

	values = new blong[_size];
	for (uint i = 0; i < _size; i++) values[i].read(end);
}
std::string nbt_long_array::getStringValue() const
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
uint nbt_long_array::getSize() const { return _size; }
void nbt_long_array::resize(uint newSize)
{
	blong* newValues = new blong[newSize];

	uint minSize = newSize < _size ? newSize : (uint)_size;
	for (uint i = 0; i < minSize; i++) newValues[i] = values[i];

	delete[] values;
	values = newValues;
	_size = newSize;
}
void nbt_long_array::operator=(const nbt& that)
{
	if (that.type != type) throw typeError;
	operator=((const nbt_long_array&)that);
}
void nbt_long_array::operator=(const nbt_long_array& that)
{
	_size = that._size;
	values = new blong[_size];
	for (uint i = 0; i < _size; i++) values[i] = that.values[i];
}