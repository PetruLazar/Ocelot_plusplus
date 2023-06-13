#include "nbt_list.h"

nbt_list::nbt_list(const std::string& name, nbt** v, int s) : nbt(tag::List, name), values(v), _size(s)
{
	if (_size) childType = values[0]->getType();
	for (uint i = 1; i < _size; i++) if (values[i]->getType() != childType) throw "Not all elements of the list are of the same type";
}
nbt_list::~nbt_list()
{
	if (childType != tag::End) for (uint i = 0; i < _size; i++) if (values[i])
	{
		//possible error:	do NOT use the same tags multiple times
		//					and do not use local variables (due to variable lifetime)
		delete values[i];
	}
	if (values) delete[] values;
	values = nullptr;
	_size = 0;
}

void nbt_list::write(std::ostream& os, bool iNT) const
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
void nbt_list::read(std::istream& is, const std::string& name)
{
	if (values)
	{
		if (childType != tag::End) for (uint i = 0; i < _size; i++)
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
void nbt_list::write(char*& buffer, bool iNT) const
{
	if (iNT)
	{
		*(buffer++) = static_cast<char>(type);

		writeName(buffer);
	}

	*(buffer++) = static_cast<char>(childType);

	_size.write(buffer);

	for (uint i = 0; i < _size; i++) values[i]->write(buffer, false);
}
void nbt_list::read(char*& end, const std::string& name)
{
	if (values)
	{
		if (childType != tag::End) for (uint i = 0; i < _size; i++)
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
std::string nbt_list::getStringValue() const
{
	std::string ret = "[";

	for (uint i = 0; i < _size; i++) ret += values[i]->getStringValue() + ',';
	ret.pop_back();
	ret += ']';
	return ret;
}
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
uint nbt_list::getSize() const { return _size; }
nbt::tag nbt_list::getElemType() const
{
	if (!_size) throw outOfBoundsError;
	return values[0]->getType();
}
void nbt_list::changeType(tag newtag)
{
	if (childType == newtag) return;
	for (uint i = 0; i < _size; i++)
	{
		if (childType != tag::End) delete values[i];
		values[i] = getTagP(newtag);
	}
	childType = newtag;
}
void nbt_list::resize(uint newSize)
{
	nbt** newValues = new nbt * [newSize];

	uint minSize = newSize < _size ? newSize : (uint)_size;
	for (uint i = 0; i < minSize; i++) newValues[i] = values[i];
	if (childType != tag::End) for (uint i = minSize; i < _size; i++) delete values[i];
	for (uint i = minSize; i < newSize; i++) values[i] = getTagP(childType);

	delete[] values;
	values = newValues;
	_size = newSize;
}

void nbt_list::operator=(const nbt& that)
{
	if (that.type != type) throw typeError;
	operator=((const nbt_list&)that);
}
void nbt_list::operator=(const nbt_list& that)
{
	name = that.name;

	childType = that.childType;

	_size = that._size;
	values = new nbt * [_size];
	for (uint i = 0; i < _size; i++) {
		values[i] = nbt::getTagP(that.values[i]->type);
		*(values[i]) = *(that.values[i]);
	}
}