#pragma once

#include "nbt_byte.h"
#include "nbt_short.h"
#include "nbt_int.h"
#include "nbt_long.h"
#include "nbt_float.h"
#include "nbt_double.h"
#include "nbt_list.h"
#include "nbt_compound.h"
#include "nbt_byte_array.h"
#include "nbt_int_array.h"
#include "nbt_long_array.h"
#include "nbt_string.h"

//example of constructing an nbt compound tag in memory and editing it
/*int main()
{
	nbt_compound c(new nbt * [12]{
			new nbt_byte(4,"Byte"),
			new nbt_short(-3,"Short"),
			new nbt_int(48,"Int"),
			new nbt_long(-849,"Long"),
			new nbt_string("salutari","String"),
			new nbt_double(15.75,"Double"),
			new nbt_float(6.25,"Float") ,
			new nbt_byte_array(new char[3]{45,16,-7},3,"ByteArray"),
			new nbt_int_array(new int[3]{
				0x4f9c645a,
				0x080fcd5a,
				0x9f57c62ai32
			},3,"IntArray"),
			new nbt_long_array(new int64[6]{
			0x4f9c645a080fcd5ai64,
			0x4f9c645a9f57c624i64,
			0x9f57c62080fcd5bai64,
			0x080fcd5a4f9c6457i64,
			0x9f57c624f9c645a3i64,
			0x080fcd5a9f57c628i64},6,"LongArray"),
			new nbt_list(new nbt * [3]{
				new nbt_float(7.4375f,"Float0"),
				new nbt_float(1.5625f,"Float1"),
				new nbt_float(-3.1875f,"Float2")
			},3,"List"),
			new nbt_compound(new nbt * [4]{new nbt_string("Petru","FirstName"),new nbt_string("Lazar","LastName"),new nbt_int(19,"Age"),new nbt_string("Laz","Location")},4,"Compound")
		}, 12);
	cout << c.to_string();

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

	cout << "\n\n" << c.to_string();

	return 0;
}*/