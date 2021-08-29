#pragma once
#include <fstream>
#include "../typedefs.h"
#include "apidef.h"

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
	
	BASICDEFS_API mcUUID(type uuidtype);
	BASICDEFS_API mcUUID(uint data1, ush data2, ush data3, ull data4);

	BASICDEFS_API void write(std::fstream&) const;
	BASICDEFS_API void write(char*&) const;
};

