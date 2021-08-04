#pragma once
#include "../basic.h"

class json
{
public:
	enum type : byte
	{
		none,
		integer,
		string,
		array,
		compound
	};

	json(type);
	json(type, const std::string&);
	virtual ~json() = 0;

	//get the name of a json object
	std::string getName();
	//get the type of a json object
	type getType();
	//get the type of json objects stored by the array
	type getChildType();
	//change the type that of the json object stored by an array
	virtual void changeType(type);

	//get the value of an int json object
	virtual int& iValue();
	//get the value of a string json object
	virtual std::string& value();
	//get a value from an array json object
	virtual json& value(int);
	//get a value from a compound json object
	virtual json& value(const std::string&);
	virtual void operator=(int);
	virtual void operator=(const std::string&);
	virtual json& operator[](int);
	virtual json& operator[](const std::string&);

	//get the value of any json object as a string
	virtual std::string stringValue() = 0;
	//convert a json objec to a string (name + value)
	virtual std::string to_string();

	//get the size of an arrayor compound json object
	virtual ull getSize();
	//resize an array json object
	virtual void resize(ull);
	//add a new json object to a compound json object
	virtual void add(json*);
	//remove a json object from a compound json object
	virtual void remove(json*);
	virtual void remove(const std::string&);

	static json* allocate(type);

protected:
	type t;
	std::string name;
};

