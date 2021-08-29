#pragma once

//#include "../typedefs.h"
#include "../endian.h"
#include <string>
#include "apidef.h"

extern const char* typeError;
extern const char* outOfBoundsError;
extern const char* searchFailedError;

class nbt
{
public:
	enum tag : byte
	{
		End,
		Byte,
		Short,
		Int,
		Long,
		Float,
		Double,
		ByteArray,
		String,
		List,
		Compound,
		IntArray,
		LongArray
	};
protected:
	std::string name;

	tag type;

	NBT_API void readName(std::fstream&);
	NBT_API void writeName(std::fstream&) const;
	NBT_API void readName(char*&);
	NBT_API void writeName(char*&) const;

public:
	NBT_API nbt(tag, std::string name = "");
	NBT_API virtual ~nbt() = 0;

	NBT_API virtual void write(std::fstream&, bool includeNameAndType = true) const = 0;
	//make sure to either use checkTag() or skip a byte manually
	NBT_API virtual void read(std::fstream&, const std::string& name = "") = 0;
	NBT_API virtual void write(char*&, bool includeNameAndType = true) const = 0;
	//make sure to either use checkTag() or skip a byte manually
	NBT_API virtual void read(char*&, const std::string& name = "") = 0;
	NBT_API std::string getName();
	NBT_API tag getType();

	//get the value of any tag as a string
	NBT_API virtual std::string getStringValue() const = 0;
	//convert the tag to string (name + value)
	NBT_API std::string to_string() const;
	//get the value of a byte tag
	NBT_API virtual char& vByte();
	//get the value of a short tag
	NBT_API virtual short& vShort();
	//get the value of an int tag
	NBT_API virtual int& vInt();
	//get the value of a long tag
	NBT_API virtual int64& vLong();
	//get the value of a float tag
	NBT_API virtual float& vFloat();
	//get the value of a double tag
	NBT_API virtual double& vDouble();
	//get a value of a byte array tag
	NBT_API virtual char& vByte(uint);
	//get the value of a string tag
	NBT_API virtual std::string& vString();
	//get a sub-tag of a compound or list tag
	NBT_API virtual nbt& vTag(uint);
	//get a sub-tag from a compound tag
	NBT_API virtual nbt& vTag(const std::string&);
	//get a value from an int array tag
	NBT_API virtual int& vInt(uint);
	//get a value from a long array tag
	NBT_API virtual int64& vLong(uint);
	//get the size of an array tag
	NBT_API virtual uint getSize() const;
	//resize a list, an array or a compound tag
	NBT_API virtual void resize(uint);
	//remove a value from a compound tag
	NBT_API virtual void remove(nbt*);
	//remove a value from a compound tag by name
	NBT_API virtual void remove(const std::string&);
	//add a new element to a compound tag
	NBT_API virtual void add(nbt*);

	//get the type of the elements of a list tag
	NBT_API virtual tag getElemType() const;
	//change the type of the elements of a list tag
	NBT_API virtual void changeType(tag);

	//check if an nbt file has the specified nbt tag
	NBT_API static bool checkTag(std::fstream&, tag = Compound);
	//check if an nbt in memory contains the specified tag
	NBT_API static bool checkTag(char*&, tag = Compound);
	//allocate a pointer of the specified type
	NBT_API static nbt* getTagP(tag);

	NBT_API virtual void operator=(char);
	NBT_API virtual void operator=(short);
	NBT_API virtual void operator=(int);
	NBT_API virtual void operator=(int64);
	NBT_API virtual void operator=(float);
	NBT_API virtual void operator=(double);
	NBT_API virtual void operator=(const std::string&);
	NBT_API virtual void operator=(const char*);

	NBT_API virtual nbt& operator[](const std::string&);
};