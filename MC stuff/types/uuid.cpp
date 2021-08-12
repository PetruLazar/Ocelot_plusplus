#include "uuid.h"

uint mcUUID::playerId = 0;
uint mcUUID::entityId = 0;

//big endian
// bbbiibbb bbbiibbb bbbiibbb bbbiibbb - bbbiibbb bbbiibbb - vvvvbiib bbbiibbb -
// bbbiibbb bbbiibbb - bbbiibbb bbbiibbb bbbiibbb bbbiibbb bbbiibbb bbbiibbb

//little endian
// bbbiibbb vvvvbiib bbbiibbb bbbiibbb bbbiibbb bbbiibbb bbbiibbb bbbiibbb
// bbbiibbb bbbiibbb bbbiibbb bbbiibbb bbbiibbb bbbiibbb bbbiibbb bbbiibbb

mcUUID::mcUUID(type uuidtype)
{
	//ull half1, half2;
	for (int i = 0; i < 16; i++) *(char*)this = byte(rand() & 0xff);
	data3 &= 0xff0f;
	data3 |= 0x30;
	/*half1 &= 0xe709e7e7e7e7e7e7;
	half2 &= 0xe7e7e7e7e7e7e7e7;
	ull id;
	if (uuidtype == player)
	{
		half1 |= 3ui64 << 52;
		id = playerId++;
	}
	else
	{
		half1 |= 2ui64 << 52;
		id = entityId++;
	}

	half1 |= id & 0x3 << 49*/

}
mcUUID::mcUUID(uint data1, ush data2, ush data3, ull data4)
{

}

void mcUUID::write(char*& buffer) const
{
	*(((uint*&)buffer)++) = data1;
	*(((ush*&)buffer)++) = data2;
	*(((ush*&)buffer)++) = data3;
	*(((ull*&)buffer)++) = data4;
}

void mcUUID::write(std::fstream& is) const
{
	throw "WIP";
}