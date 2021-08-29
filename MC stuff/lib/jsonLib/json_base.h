#pragma once
#include "apidef.h"
#include "../typedefs.h"
#include <string>

class json
{
public:
	enum type : byte
	{
		none,
		integer,
		string,
		array,
		compound,
		boolean
	};

	JSON_API json(type, const std::string& name = "");
	JSON_API virtual ~json() = 0;

	//virtual void parse(const std::string) = 0;
	JSON_API static json* parse(std::fstream&, bool canHaveName = true);

	//get the name of a json object
	JSON_API std::string getName();
	//get the type of a json object
	JSON_API type getType();
	//get the type of json objects stored by the array
	JSON_API type getChildType();
	//change the type that of the json object stored by an array
	virtual void JSON_API changeType(type);

	//get the value of an int json object
	JSON_API virtual int& iValue();
	//get the value of a bool json object
	JSON_API virtual bool& bValue();
	//get the value of a string json object
	JSON_API virtual std::string& value();
	//get a value from an array json object
	JSON_API virtual json& value(int);
	//get a value from a compound json object
	JSON_API virtual json& value(const std::string&);
	virtual void JSON_API operator=(int);
	virtual void JSON_API operator=(const std::string&);
	JSON_API virtual json& operator[](int);
	JSON_API virtual json& operator[](const std::string&);

	//get the value of any json object as a string
	JSON_API virtual std::string stringValue() const = 0;
	//convert a json object to a string (name + value)
	JSON_API virtual std::string to_string() const;

	//get the size of an array or compound json object
	JSON_API virtual ull getSize();
	//resize an array json object
	virtual void JSON_API resize(ull);
	//add a new json object to a compound json object
	virtual void JSON_API add(json*);
	//remove a json object from a compound json object
	virtual void JSON_API remove(json*);
	virtual void JSON_API remove(const std::string&);

	JSON_API static json* allocate(type);

protected:
	type t;
	std::string name;
};

