#pragma once
#include "nbt_base.h"

class nbt_list : public nbt
{
	nbt** values;
	buint _size;

	tag childType;

public:
	NBT_API nbt_list(const std::string& name = "", nbt** values = nullptr, int size = 0);
	NBT_API ~nbt_list();

	NBT_API void write(std::fstream&, bool includeNameAndType = true)const;
	NBT_API void read(std::fstream&, const std::string& name = "");
	NBT_API void write(char*&, bool includeNameAndType = true)const;
	NBT_API void read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	NBT_API std::string getStringValue()const;

	//get a tag from the list
	NBT_API nbt& vTag(uint);
	NBT_API nbt& operator[](uint);
	//get the size of the list
	NBT_API uint getSize()const;
	//resize the list
	NBT_API void resize(uint);

	NBT_API tag getElemType()const;
	NBT_API void changeType(tag);
};