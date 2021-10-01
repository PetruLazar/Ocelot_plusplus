#include "nbt_compound.h"

nbt_compound::nbt_compound(const std::string& name, const std::vector <nbt*>& v) : nbt(tag::Compound, name), values(v) { }
nbt_compound::nbt_compound(const std::string& name, nbt** v, uint s) : nbt(tag::Compound, name)
{
	for (uint i = 0; i < s; i++) values.push_back(v[i]);
	delete[] v;
}
nbt_compound::~nbt_compound()
{
	for (nbt* e : values)
		//possible error: do NOT use the same vector<> or array to construct multiple compound tags
		delete e;
}

void nbt_compound::write(std::fstream& os, bool iNT) const
{
	if (values.empty()) os.write("", 1);

	if (iNT)
	{
		os.write((char*)&type, sizeof(type));

		writeName(os);
	}

	for (nbt* e : values) e->write(os);

	os.write("", 1);
}
void nbt_compound::read(std::fstream& is, const std::string& name)
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

	tag t = tag::End;
	while (true)
	{
		is.read((char*)&t, sizeof(t));
		if (t == tag::End) break;
		nbt* v = getTagP(t);
		v->read(is);
		values.push_back(v);
	}
}
void nbt_compound::write(char*& buffer, bool iNT) const
{
	if (values.empty())
	{
		*(buffer++) = 0;
		return;
	}

	if (iNT)
	{
		*(buffer++) = static_cast<char>(type);

		writeName(buffer);
	}

	for (nbt* e : values) e->write(buffer);

	*(buffer++) = 0;
}
void nbt_compound::read(char*& end, const std::string& name)
{
	if (!values.empty())
	{
		for (nbt* e : values) delete e;
		values.clear();
	}

	char* buffer = end;

	if (name == "") readName(end);
	else nbt::name = name;

	nbt::tag t;
	while (true)
	{
		t = (nbt::tag) * (end++);
		if (t == nbt::tag::End) break;
		nbt* v = getTagP(t);
		v->read(end);
		values.push_back(v);
	}
}
std::string nbt_compound::getStringValue() const
{
	if (values.empty()) return "{}";
	std::string ret = "{";

	for (nbt* e : values)
		ret += e->to_string() + ',';
	
	if (values.size() != 0)
		ret.pop_back();

	ret += '}';
	return ret;
}
nbt& nbt_compound::vTag(const std::string& n)
{
	return operator[](n);
}
nbt& nbt_compound::operator[](const std::string& n)
{
	for (nbt* e : values)
		if (n == e->getName())
			return *e;
	throw searchFailedError;
}
uint nbt_compound::getSize() const { return (uint)values.size(); }
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
void nbt_compound::remove(const std::string& elem)
{
	uint s = (uint)values.size();
	for (uint i = 0; i < s; i++) if (values[i]->getName() == elem)
	{
		delete values[i];
		for (uint j = i + 1; j < s; j++) values[(size_t)j - 1] = values[j];
		values.pop_back();
		return;
	}
	throw searchFailedError;
}