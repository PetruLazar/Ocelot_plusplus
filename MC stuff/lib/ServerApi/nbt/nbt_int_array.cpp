#include "nbt_int_array.h"

nbt_int_array::nbt_int_array(const std::string& name, int* v, int s) : nbt(tag::IntArray, name), values((bint*)v), _size(s) { }
nbt_int_array::~nbt_int_array()
{
	delete[] values;
	values = nullptr;
	_size = 0;
}

void nbt_int_array::write(std::ostream& os, bool iNT) const
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
void nbt_int_array::read(std::istream& is, const std::string& name)
{
	if (values)
	{
		delete[] values;
	}

	std::streampos begin = is.tellg();

	if (name == "") readName(is);
	else nbt::name = name;

	_size.read(is);

	values = new bint[_size];
	for (uint i = 0; i < _size; i++) values[i].read(is);
}
void nbt_int_array::write(char*& buffer, bool iNT) const
{
	if (iNT)
	{
		*(buffer++) = static_cast<char>(type);

		writeName(buffer);
	}

	_size.write(buffer);

	for (uint i = 0; i < _size; i++) values[i].write(buffer);
}
void nbt_int_array::read(char*& end, const std::string& name)
{
	if (values) delete[] values;

	char* buffer = end;

	if (name == "") readName(end);
	else nbt::name = name;

	_size.read(end);

	values = new bint[_size];
	for (uint i = 0; i < _size; i++) values[i].read(end);
}
std::string nbt_int_array::getStringValue() const
{
	std::string ret = "[";

	if (!_size)
	{
		ret += "]i";
		return ret;
	}

	ret += std::to_string(values[0]);
	for (uint i = 1; i < _size; i++) ret += ',' + std::to_string(values[i]);
	ret += "]i";
	return ret;
}
int& nbt_int_array::vInt(uint i)
{
	if (i >= _size) throw outOfBoundsError;
	return values[i];
}
int& nbt_int_array::operator[](uint i)
{
	if (i >= _size) throw outOfBoundsError;
	return values[i];
}
uint nbt_int_array::getSize() const { return _size; }
void nbt_int_array::resize(uint newSize)
{
	bint* newValues = new bint[newSize];

	uint minSize = newSize < _size ? newSize : (uint)_size;
	for (uint i = 0; i < minSize; i++) newValues[i] = values[i];

	delete[] values;
	values = newValues;
	_size = newSize;
}
void nbt_int_array::operator=(const nbt& that)
{
	if (that.type != type) throw typeError;
	operator=((const nbt_int_array&)that);
}
void nbt_int_array::operator=(const nbt_int_array& that)
{
	name = that.name;

	_size = that._size;
	values = new bint[_size];
	for (uint i = 0; i < _size; i++) 
		values[i] = that.values[i];
}