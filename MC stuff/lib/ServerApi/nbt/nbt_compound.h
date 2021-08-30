#pragma once
#include "nbt_base.h"
#include <vector>

class nbt_compound : public nbt
{
	std::vector<nbt*> values;

public:
	SERVER_API nbt_compound(const std::string& name, const std::vector<nbt*>& values = std::vector<nbt*>());
	SERVER_API nbt_compound(const std::string& name = "", nbt** values = nullptr, uint size = 0);
	SERVER_API ~nbt_compound();

	void SERVER_API write(std::fstream&, bool includeNameAndType = true) const;
	void SERVER_API read(std::fstream&, const std::string& name = "");
	void SERVER_API write(char*&, bool includeNameAndType = true) const;
	void _fastcall read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	SERVER_API std::string getStringValue() const;

	//get a tag from the compound tag
	SERVER_API nbt& vTag(const std::string&);
	SERVER_API nbt& operator[](const std::string&);
	//get the size of the compound tag
	SERVER_API uint getSize() const;
	//remove a tag from the compound tag
	virtual void SERVER_API remove(nbt*);
	//remove a tag from the compound tag by name
	virtual void SERVER_API remove(const std::string&);
	//add a new tag to the compound tag
	virtual void SERVER_API add(nbt*);
};