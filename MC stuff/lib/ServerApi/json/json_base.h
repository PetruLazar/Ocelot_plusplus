#pragma once
#include "../types/apidef.h"
#include "../types/typedefs.h"
#include <string>

class json
{
public:
	enum class type : Byte
	{
		none,
		integer,
		string,
		array,
		compound,
		boolean,
		decimal
	};

	SERVER_API json(json::type, const std::string& name = "");
	SERVER_API virtual ~json() = 0;

	//virtual void parse(const std::string) = 0;
	SERVER_API static json* parse(std::fstream&, bool canHaveName = true);

	//get the name of a json object
	SERVER_API std::string getName();
	//get the type of a json object
	SERVER_API json::type getType();
	//get the type of json objects stored by the array
	SERVER_API json::type getChildType();
	//change the type that of the json object stored by an array
	virtual void SERVER_API changeType(json::type); //remove

	//get the value of an int json object
	SERVER_API virtual int& iValue();
	//get the value of a decimal json object
	SERVER_API virtual double& dValue();
	//get the value of a bool json object
	SERVER_API virtual bool& bValue();
	//get the value of a string json object
	SERVER_API virtual std::string& value();
	//get a value from an array json object
	SERVER_API virtual json& value(int);
	//get a value from a compound json object
	SERVER_API virtual json& value(const std::string&);
	virtual void SERVER_API operator=(int);
	virtual void SERVER_API operator=(const std::string&);
	SERVER_API virtual json& operator[](int);
	SERVER_API virtual json& operator[](const std::string&);

	bool virtual has(const std::string&);

	//get the value of any json object as a string
	SERVER_API virtual std::string stringValue() const = 0;
	//convert a json object to a string (name + value)
	SERVER_API virtual std::string to_string() const;

	//get the size of an array or compound json object
	SERVER_API virtual ull getSize();
	//resize an array json object
	virtual void SERVER_API resize(ull); //remove?
	//add a new json object to a compound json object
	virtual void SERVER_API add(json*);
	//remove a json object from a compound json object
	virtual void SERVER_API remove(json*);
	virtual void SERVER_API remove(const std::string&);

	SERVER_API static json* allocate(json::type);

protected:
	json::type t;
	std::string name;
};

