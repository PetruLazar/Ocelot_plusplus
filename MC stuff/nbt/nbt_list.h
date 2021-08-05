#pragma once
#include "nbt_base.h"

class nbt_list : public nbt
{
	nbt** values;
	buint _size;

	tag childType;

public:
	nbt_list(const std::string& name = "", nbt** values = nullptr, int size = 0);
	~nbt_list();

	void write(std::fstream&, bool includeNameAndType = true)const;
	void read(std::fstream&, const std::string& name = "");
	void write(char*&, bool includeNameAndType = true)const;
	void read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	std::string getStringValue()const;

	//get a tag from the list
	nbt& vTag(uint);
	nbt& operator[](uint);
	//get the size of the list
	uint getSize()const;
	//resize the list
	void resize(uint);

	tag getElemType()const;
	void changeType(tag);
};