#pragma once
#include "nbt_base.h"

class nbt_list : public nbt
{
	nbt** values;
	buint _size;

	tag childType;

public:
	SERVER_API nbt_list(const std::string& name = "", nbt** values = nullptr, int size = 0);
	SERVER_API ~nbt_list();

	SERVER_API void write(std::fstream&, bool includeNameAndType = true)const;
	SERVER_API void read(std::fstream&, const std::string& name = "");
	SERVER_API void write(char*&, bool includeNameAndType = true)const;
	SERVER_API void read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	SERVER_API std::string getStringValue()const;

	//get a tag from the list
	SERVER_API nbt& vTag(uint);
	SERVER_API nbt& operator[](uint);
	//get the size of the list
	SERVER_API uint getSize()const;
	//resize the list
	SERVER_API void resize(uint);

	SERVER_API tag getElemType()const;
	SERVER_API void changeType(tag);
};