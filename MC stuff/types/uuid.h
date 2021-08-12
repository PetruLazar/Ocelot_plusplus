#pragma once
#include "endian.h"

class mcUUID
{
	static uint playerId;
	static uint entityId;

	// bbbiibbb bbbiibbb bbbiibbb bbbiibbb - bbbiibbb bbbiibbb - vvvvbiib bbbiibbb - bbbiibbb bbbiibbb - bbbiibbb bbbiibbb bbbiibbb bbbiibbb bbbiibbb bbbiibbb
	uint data1;
	ush data2;
	ush data3; //contains the version: most significant 4 bytes
	ull data4;


public:

	//players: v3, entities, v2
	enum type
	{
		player,
		entity
	};
	
	mcUUID(type uuidtype);
	mcUUID(uint data1, ush data2, ush data3, ull data4);

	void write(std::fstream&) const;
	void write(char*&) const;
};

