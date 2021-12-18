
#include "window.h"

namespace window
{
	unsigned getID(type windowType)
	{
		indexerType.emplace_back(windowType);
		return indexer++;
	}

	type getWindowType(unsigned index)
	{
		return indexerType[index - 1];
	}
}