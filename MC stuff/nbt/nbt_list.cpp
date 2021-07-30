#include "nbt_list.h"
nbt_list::nbt_list(std::string name) : nbt(List, name), values(nullptr), _size(0) { }
nbt_list::nbt_list(nbt** v, int s, std::string name) : nbt(List, name), values(v), _size(s)
{
	childType = values[0]->getType();
	for (uint i = 1; i < _size; i++) if (values[i]->getType() != childType) throw "Not all elements of the list are of the same type";
}
nbt_list::~nbt_list()
{
	for (uint i = 0; i < _size; i++)
	{
		//possible error:	do NOT use the same tags multiple times
		//					and do not use local variables (due to variable lifetime)
		delete values[i];
	}
	delete[] values;
	values = nullptr;
	_size = 0;
}
void nbt_list::write(std::fstream& os, bool iNT)
{
	if (iNT)
	{
		os.write((char*)&type, sizeof(type));

		writeName(os);
	}

	os.write((char*)&childType, sizeof(childType));

	_size.write(os);

	for (uint i = 0; i < _size; i++)
	{
		values[i]->write(os, false);
	}
}
void nbt_list::read(std::fstream& is, std::string name)
{
	if (values)
	{
		for (uint i = 0; i < _size; i++)
		{
			//possible error:	do NOT use the same tags multiple times
			//					and do not use local variables (due to variable lifetime)
			delete values[i];
		}
		delete[] values;
	}

	std::streampos begin = is.tellg();

	if (name == "") readName(is);
	else nbt::name = name;

	is.read((char*)&childType, sizeof(childType));

	_size.read(is);

	values = new nbt * [_size];
	for (uint i = 0; i < _size; i++)
	{
		values[i] = getTagP(childType);
		values[i]->read(is, std::to_string(i));
	}
}
void nbt_list::write(char*& buffer, bool iNT)
{
	if (iNT)
	{
		*(buffer++) = type;

		writeName(buffer);
	}

	*(buffer++) = childType;

	_size.write(buffer);

	for (uint i = 0; i < _size; i++) values[i]->write(buffer, false);
}
void nbt_list::read(char*& end, std::string name)
{
	if (values)
	{
		for (uint i = 0; i < _size; i++)
		{
			//possible error:	do NOT use the same tags multiple times
			//					and do not use local variables (due to variable lifetim
			delete values[i];
		}
		delete[] values;
	}

	char* buffer = end;

	if (name == "") readName(end);
	else nbt::name = name;

	childType = (tag) * (end++);

	_size.read(end);

	values = new nbt * [_size];
	for (uint i = 0; i < _size; i++)
	{
		values[i] = getTagP(childType);
		values[i]->read(end, std::to_string(i));
	}
}
std::string nbt_list::getStringValue()
{
	std::string ret = "[";

	for (uint i = 0; i < _size; i++) ret += values[i]->getStringValue() + ',';
	ret.pop_back();
	ret += ']';
	return ret;
}
/*std::string nbt_list::to_string()
{

}*/
nbt& nbt_list::vTag(uint i)
{
	if (i >= _size) throw outOfBoundsError;
	return *values[i];
}
nbt& nbt_list::operator[](uint i)
{
	if (i >= _size) throw outOfBoundsError;
	return *values[i];
}
uint nbt_list::getSize() { return _size; }
nbt::tag nbt_list::getElemType()
{
	if (!_size) throw outOfBoundsError;
	return values[0]->getType();
}
void nbt_list::resize(uint newSize)
{
	nbt** newValues = new nbt * [newSize];

	uint minSize = newSize < _size ? newSize : (uint)_size;
	for (uint i = 0; i < minSize; i++) newValues[i] = values[i];
	for (uint i = minSize; i < _size; i++) delete values[i];
	for (uint i = minSize; i < newSize; i++) values[i] = getTagP(childType);

	delete[] values;
	values = newValues;
	_size = newSize;
}