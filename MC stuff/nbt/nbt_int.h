#pragma once
#include "nbt_base.h"

class nbt_int : public nbt
{
	bint value;

public:
	nbt_int(std::string name = "");
	nbt_int(int, std::string name = "");
	//~nbt_int();

	//write tag to file
	void write(std::fstream&, bool includeNameAndType = true);
	//read tag to file
	void read(std::fstream&, std::string name = "");
	//write tag to buffer
	void write(char*&, bool includeNameAndType = true);
	//read tag from buffer
	void read(char*&, std::string name = "");
	//get the value of the tag as string
	std::string getStringValue();

	//get the value of an int tag
	int& vInt();
	operator int& ();
};