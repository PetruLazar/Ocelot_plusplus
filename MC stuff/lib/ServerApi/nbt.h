#pragma once

#include "nbt/nbt_Byte.h"
#include "nbt/nbt_short.h"
#include "nbt/nbt_int.h"
#include "nbt/nbt_long.h"
#include "nbt/nbt_float.h"
#include "nbt/nbt_double.h"
#include "nbt/nbt_list.h"
#include "nbt/nbt_compound.h"
#include "nbt/nbt_Byte_array.h"
#include "nbt/nbt_int_array.h"
#include "nbt/nbt_long_array.h"
#include "nbt/nbt_string.h"

//example of constructing an nbt compound tag in memory and editing it
/*int main()
{
	nbt_compound c("", new nbt * [12]{
			new nbt_Byte("Byte",4),
			new nbt_short("Short",-3),
			new nbt_int("Int",48),
			new nbt_long("Long",-849),
			new nbt_string("String","salutari"),
			new nbt_double("Double",15.75),
			new nbt_float("Float",6.25) ,
			new nbt_Byte_array("ByteArray",new char[3]{45,16,-7},3),
			new nbt_int_array("IntArray",new int[3]{
				0x4f9c645a,
				0x080fcd5a,
				0x9f57c62ai32
			},3),
			new nbt_long_array("LongArray",new int64[6]{
			0x4f9c645a080fcd5ai64,
			0x4f9c645a9f57c624i64,
			0x9f57c62080fcd5bai64,
			0x080fcd5a4f9c6457i64,
			0x9f57c624f9c645a3i64,
			0x080fcd5a9f57c628i64},6),
			new nbt_list("List",new nbt * [3]{
				new nbt_float("Float0",7.4375f),
				new nbt_float("Float1",1.5625f),
				new nbt_float("Float2",-3.1875f)
			},3),
			new nbt_compound("Compound",new nbt * [4]{new nbt_string("FirstName","Petru"),new nbt_string("LastName","Lazar"),new nbt_int("Age",19),new nbt_string("Location","Laz")},4)
		}, 12);
	std::cout << c.to_string();

	c["Long"] = -100i64;
	c["Short"] = 0x8000i16;
	c["ByteArray"].resize(2);
	c["ByteArray"].vByte(1) = c["ByteArray"].vByte(0);
	c["Compound"]["Age"] = 20;
	c["Compound"].remove("LastName");
	c["LongArray"].resize(3);
	c["List"].changeType(nbt::Short);
	c["List"].resize(2);
	c["List"].vTag(0) = 512i16;
	c["List"].vTag(1) = 513i16;

	std::cout << "\n\n" << c.to_string();

	return 0;
}*/