#pragma once

//unused
//WIP

#include "json/json_base.h"
#include "json/json_string.h"
#include "json/json_int.h"
#include "json/json_array.h"
#include "json/json_compound.h"

//example of forming a json object in memory and editing it
/*int main()
{
	json_compound test("", new json * [3]{
		new json_string("text","This is a "),
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
	}, 3);
	std::cout << test.stringValue() << '\n';
	test["text"] = "This is not a ";
	test["extra"][0]["color"] = "dark_purple";
	test["extra"][1]["text"] = " server!";
	test["extra"][1].remove("color");
	test["clickEvent"]["value"] = "/kill @s";
	std::cout << test.stringValue();

	return 0;
}*/