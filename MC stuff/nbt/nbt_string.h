#pragma once
#include "nbt_base.h"

class nbt_string : public nbt
{
	std::string value;

public:
	nbt_string(std::string name = "");
	nbt_string(std::string, std::string name = "");
	//~nbt_string();

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

	//get the value of a string tag
	std::string& vString();
	operator std::string& ();
};