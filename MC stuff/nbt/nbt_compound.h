#pragma once
#include "nbt_base.h"

class nbt_compound : public nbt
{
	std::vector<nbt*> values;

public:
	nbt_compound(const std::string& name = "", const std::vector<nbt*>& values = std::vector<nbt*>());
	nbt_compound(const std::string& name = "", nbt** values = nullptr, uint size = 0);
	~nbt_compound();

	void write(std::fstream&, bool includeNameAndType = true) const;
	void read(std::fstream&, const std::string& name = "");
	void write(char*&, bool includeNameAndType = true) const;
	void read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	std::string getStringValue() const;

	//get a tag from the compound tag
	nbt& vTag(const std::string&);
	nbt& operator[](const std::string&);
	//get the size of the compound tag
	uint getSize() const;
	//remove a tag from the compound tag
	virtual void remove(nbt*);
	//remove a tag from the compound tag by name
	virtual void remove(const std::string&);
	//add a new tag to the compound tag
	virtual void add(nbt*);
};