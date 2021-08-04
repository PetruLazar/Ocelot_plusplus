#pragma once

//unused
//WIP

#include "json_base.h"
#include "json_string.h"
#include "json_int.h"
#include "json_array.h"
#include "json_compound.h"

//example of forming a json object in memory and editing it
/*int main()
{
	json_compound test("", new json * [2]{
		new json_string("text","This is a "),
		new json_array("extra",new json * [2]{
			new json_compound("",new json * [2]{
				new json_string("text","Minecraft"),
				new json_string("color","gold")},2),
			new json_compound("",new json * [2]{
				new json_string("text"," server."),
				new json_string("color","white")},2)},2) }, 2);
	std::cout << test.stringValue() << '\n';
	test["text"] = "This is not a ";
	test["extra"][0]["color"] = "dark_purple";
	test["extra"][1]["text"] = " server!";
	test["extra"][1].remove("color");
	std::cout << test.stringValue();

	return 0;
}*/