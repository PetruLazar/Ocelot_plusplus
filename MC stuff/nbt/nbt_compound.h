#pragma once
#include "nbt_base.h"

class nbt_compound : public nbt
{
	std::vector<nbt*> values;

public:
	nbt_compound(std::string name = "");
	nbt_compound(std::vector<nbt*>, std::string name = "");
	~nbt_compound();

	void write(std::fstream&, bool includeNameAndType = true);
	void read(std::fstream&, std::string name = "");
	void write(char*&, bool includeNameAndType = true);
	void read(char*&, std::string name = "");
	//get the value of the tag as a string
	std::string getStringValue();

	//get a tag from the compound tag
	nbt& vTag(uint);
	//get a tag from the compound tag
	nbt& vTag(std::string);
	nbt& operator[](uint);
	nbt& operator[](std::string);
	//get the size of the compound tag
	uint getSize();
	//remove a tag from the compound tag
	virtual void remove(nbt*);
	//add a new tag to the compound tag
	virtual void add(nbt*);
};