#pragma once
#include "nbt_base.h"

class nbt_compound : public nbt
{
	std::vector<nbt*> values;

public:
	nbt_compound(std::string name = "");
	nbt_compound(std::vector<nbt*>, std::string name = "");
	~nbt_compound();

	//write tag to file
	void write(std::fstream&, bool includeNameAndType = true);
	//read tag from file
	void read(std::fstream&, std::string name = "");
	//write tag to buffer
	void write(char*&, bool includeNameAndType = true);
	//read tag from buffer
	void read(char*&, std::string name = "");
	//get the value of the tag as string
	std::string getStringValue();

	//get a tag from a compound tag
	nbt& vTag(uint);
	nbt& vTag(std::string);
	nbt& operator[](uint);
	nbt& operator[](std::string);
	//get the size of an array tag
	uint getSize();
	//remove a value from a list, an array, or a compound tag
	virtual void remove(nbt*);
	//add a new element to a list or a compound tag
	virtual void add(nbt*);
};