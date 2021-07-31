#pragma once
#include "nbt_base.h"

class nbt_list : public nbt
{
	nbt** values;
	buint _size;

	tag childType;

public:
	nbt_list(std::string name = "");
	nbt_list(nbt**, int, std::string name = "");
	~nbt_list();

	void write(std::fstream&, bool includeNameAndType = true);
	void read(std::fstream&, std::string name = "");
	void write(char*&, bool includeNameAndType = true);
	void read(char*&, std::string name = "");
	//get the value of the tag as a string
	std::string getStringValue();

	//get a tag from the list
	nbt& vTag(uint);
	nbt& operator[](uint);
	//get the size of the list
	uint getSize();
	//resize the list
	void resize(uint);

	tag getElemType();
};