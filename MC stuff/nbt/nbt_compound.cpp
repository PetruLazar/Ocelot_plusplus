#include "nbt_compound.h"
nbt_compound::nbt_compound(std::string name) : nbt(Compound, name) { }
nbt_compound::nbt_compound(std::vector <nbt*> v, std::string name) : nbt(Compound, name), values(v) { }
nbt_compound::~nbt_compound()
{
	for (nbt* e : values)
		//possible error: do NOT use the same vector<> to construct multiple compound tags
		delete e;
}
void nbt_compound::write(std::fstream& os, bool iNT)
{
	if (iNT)
	{
		os.write((char*)&type, sizeof(type));

		writeName(os);
	}

	for (nbt* e : values) e->write(os);

	os.write("\0", 1);
}
void nbt_compound::read(std::fstream& is, std::string name)
{
	if (!values.empty())
	{
		for (nbt* e : values)
			//possible error: do NOT use the same vector<> to construct multiple compound tags
			delete e;

		values.clear();
	}

	std::streampos begin = is.tellg();

	if (name == "") readName(is);
	else nbt::name = name;

	tag t = End;
	while (true)
	{
		is.read((char*)&t, sizeof(t));
		if (t == End) break;
		nbt* v = getTagP(t);
		v->read(is);
		values.push_back(v);
	}
}
void nbt_compound::write(char*& buffer, bool iNT)
{
	if (iNT)
	{
		*(buffer++) = type;

		writeName(buffer);
	}

	for (nbt* e : values) e->write(buffer);

	*(buffer++) = 0;
}
void nbt_compound::read(char*& end, std::string name)
{
	if (!values.empty())
	{
		for (nbt* e : values) delete e;
		values.clear();
	}

	char* buffer = end;

	if (name == "") readName(end);
	else nbt::name = name;

	tag t;
	while (true)
	{
		t = (tag) * (end++);
		if (t == End) break;
		nbt* v = getTagP(t);
		v->read(end);
		values.push_back(v);
	}
}
std::string nbt_compound::getStringValue()
{
	std::string ret = "{";

	for (nbt* e : values) ret += e->to_string() + ',';
	ret.pop_back();
	ret += '}';
	return ret;
}
/*std::string nbt_compound::to_string()
{

}*/
nbt& nbt_compound::vTag(uint i)
{
	if (i >= values.size()) throw outOfBoundsError;
	return *values[i];
}
nbt& nbt_compound::vTag(std::string n)
{
	for (nbt* e : values) if (n == e->getName()) return *e;
	throw searchFailedError;
}
nbt& nbt_compound::operator[](uint i)
{
	return vTag(i);
}
nbt& nbt_compound::operator[](std::string n)
{
	return vTag(n);
}
uint nbt_compound::getSize() { return (uint)values.size(); }
/*void nbt_compound::resize(uint newSize)
{
	uint s = values.size();
}*/
void nbt_compound::add(nbt* newElem)
{
	values.push_back(newElem);
}
void nbt_compound::remove(nbt* elem)
{
	uint s = (uint)values.size();
	for (uint i = 0; i < s; i++) if (values[i] == elem)
	{
		delete values[i];
		for (uint j = i + 1; j < s; j++) values[(size_t)j - 1] = values[j];
		values.pop_back();
		break;
	}
}