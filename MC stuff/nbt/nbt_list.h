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

	//get a tag from a tag list
	nbt& vTag(uint);
	nbt& operator[](uint);
	//get the size of an array tag
	uint getSize();
	//resize a list, an array or a compound tag
	void resize(uint);

	tag getElemType();
};