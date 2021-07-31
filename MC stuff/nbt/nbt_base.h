#pragma once
#include "../basic.h"
#include "../endian.h"

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

	void readName(std::fstream&);
	void writeName(std::fstream&);
	void readName(char*&);
	void writeName(char*&);

public:
	nbt(tag, std::string name = "");
	virtual ~nbt() = 0;

	virtual void write(std::fstream&, bool includeNameAndType = true) = 0;
	//make sure to either use checkTag() or skip a byte manually
	virtual void read(std::fstream&, std::string name = "") = 0;
	virtual void write(char*&, bool includeNameAndType = true) = 0;
	//make sure to either use checkTag() or skip a byte manually
	virtual void read(char*&, std::string name = "") = 0;
	std::string getName();
	tag getType();

	//get the value of any tag as a string
	virtual std::string getStringValue() = 0;
	//convert the tag to string (name + value)
	std::string to_string();
	//get the value of a byte tag
	virtual char& vByte();
	//get the value of a short tag
	virtual short& vShort();
	//get the value of an int tag
	virtual int& vInt();
	//get the value of a long tag
	virtual int64& vLong();
	//get the value of a float tag
	virtual float& vFloat();
	//get the value of a double tag
	virtual double& vDouble();
	//get a value of a byte array tag
	virtual char& vByte(uint);
	//get the value of a string tag
	virtual std::string& vString();
	//get a sub-tag of a compound or list tag
	virtual nbt& vTag(uint);
	//get a sub-tag from a compound tag
	virtual nbt& vTag(std::string);
	//get a value from an int array tag
	virtual int& vInt(uint);
	//get a value from a long array tag
	virtual int64& vLong(uint);
	//get the size of an array tag
	virtual uint getSize();
	//resize a list, an array or a compound tag
	virtual void resize(uint);
	//remove a value from a compound tag
	virtual void remove(nbt*);
	//add a new element to a compound tag
	virtual void add(nbt*);

	//get the type of the elements of a list tag
	virtual tag getElemType();

	//check if an nbt file has the specified nbt tag
	static bool checkTag(std::fstream&, tag = Compound);
	//check if an nbt in memory contains the specified tag
	static bool checkTag(char*&, tag = Compound);
	//allocate a pointer of the specified type
	static nbt* getTagP(tag);
};