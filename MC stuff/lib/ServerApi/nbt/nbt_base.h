#pragma once

#include "../types/endian.h"
#include <string>
#include "../types/apidef.h"

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

	SERVER_API void readName(std::fstream&);
	SERVER_API void writeName(std::fstream&) const;
	SERVER_API void readName(char*&);
	SERVER_API void writeName(char*&) const;

public:
	SERVER_API nbt(tag, std::string name = "");
	SERVER_API virtual ~nbt() = 0;

	SERVER_API virtual void write(std::fstream&, bool includeNameAndType = true) const = 0;
	//make sure to either use checkTag() or skip a byte manually
	SERVER_API virtual void read(std::fstream&, const std::string& name = "") = 0;
	SERVER_API virtual void write(char*&, bool includeNameAndType = true) const = 0;
	//make sure to either use checkTag() or skip a byte manually
	SERVER_API virtual void read(char*&, const std::string& name = "") = 0;
	SERVER_API std::string getName();
	SERVER_API tag getType();

	//get the value of any tag as a string
	SERVER_API virtual std::string getStringValue() const = 0;
	//convert the tag to string (name + value)
	SERVER_API std::string to_string() const;
	//get the value of a byte tag
	SERVER_API virtual char& vByte();
	//get the value of a short tag
	SERVER_API virtual short& vShort();
	//get the value of an int tag
	SERVER_API virtual int& vInt();
	//get the value of a long tag
	SERVER_API virtual int64& vLong();
	//get the value of a float tag
	SERVER_API virtual float& vFloat();
	//get the value of a double tag
	SERVER_API virtual double& vDouble();
	//get a value of a byte array tag
	SERVER_API virtual char& vByte(uint);
	//get the value of a string tag
	SERVER_API virtual std::string& vString();
	//get a sub-tag of a compound or list tag
	SERVER_API virtual nbt& vTag(uint);
	//get a sub-tag from a compound tag
	SERVER_API virtual nbt& vTag(const std::string&);
	//get a value from an int array tag
	SERVER_API virtual int& vInt(uint);
	//get a value from a long array tag
	SERVER_API virtual int64& vLong(uint);
	//get the size of an array tag
	SERVER_API virtual uint getSize() const;
	//resize a list, an array or a compound tag
	SERVER_API virtual void resize(uint);
	//remove a value from a compound tag
	SERVER_API virtual void remove(nbt*);
	//remove a value from a compound tag by name
	SERVER_API virtual void remove(const std::string&);
	//add a new element to a compound tag
	SERVER_API virtual void add(nbt*);

	//get the type of the elements of a list tag
	SERVER_API virtual tag getElemType() const;
	//change the type of the elements of a list tag
	SERVER_API virtual void changeType(tag);

	//check if an nbt file has the specified nbt tag
	SERVER_API static bool checkTag(std::fstream&, tag = Compound);
	//check if an nbt in memory contains the specified tag
	SERVER_API static bool checkTag(char*&, tag = Compound);
	//allocate a pointer of the specified type
	SERVER_API static nbt* getTagP(tag);

	SERVER_API virtual void operator=(char);
	SERVER_API virtual void operator=(short);
	SERVER_API virtual void operator=(int);
	SERVER_API virtual void operator=(int64);
	SERVER_API virtual void operator=(float);
	SERVER_API virtual void operator=(double);
	SERVER_API virtual void operator=(const std::string&);
	SERVER_API virtual void operator=(const char*);

	SERVER_API virtual nbt& operator[](const std::string&);
};