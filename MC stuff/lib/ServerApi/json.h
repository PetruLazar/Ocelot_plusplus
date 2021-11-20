#pragma once

//unused
//WIP

#include "json/json_base.h"
#include "json/json_string.h"
#include "json/json_int.h"
#include "json/json_decimal.h"
#include "json/json_array.h"
#include "json/json_compound.h"
#include "json/json_boolean.h"

//example of forming a json object in memory and editing it
/*int main()
{
	json_compound test("", new json * [5]{
		new json_string("text","This is a "),
		new json_boolean("bold",true),
		new json_int("integer", 9),
		new json_compound("clickEvent",new json * [2]{
			new json_string("action","run_command"),
			new json_string("value","/tellraw @s {\\\"text\\\":\\\"Yesss!\\\",\\\"color\\\":\\\"green\\\"}")
			},2),
		new json_array("extra",new json * [2]{
			new json_compound("",new json * [2]{
				new json_string("text","Minecraft"),
				new json_string("color","gold")
			},2),
			new json_compound("",new json * [2]{
				new json_string("text"," server."),
				new json_string("color","white")
			},2)
		},2)
		}, 5);
	if (test["integer"].iValue() == 8) cout << '\n';
	std::cout << test.stringValue() << "\n\n";
	test["text"] = "This is not a ";
	test["extra"][0]["color"] = "dark_purple";
	test["extra"][1]["text"] = " server!";
	test["extra"][1].remove("color");
	test["clickEvent"]["value"] = "/kill @s";
	test["bold"] = true;
	test["integer"] = 19;

	if (test["integer"].iValue() == 9) cout << '\n';
	std::cout << test.stringValue();

	return 0;
}*/