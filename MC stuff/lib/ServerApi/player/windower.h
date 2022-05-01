#pragma once

#include "../types/window.h"

#include <queue>

namespace mcp {
	class windower {
	private:
		std::queue<std::pair<window::type, unsigned>> que;
		unsigned indexer = 1;

	public:
		unsigned open(window::type theWindow);
		void close(unsigned ID);

		window::type getLatest(unsigned ID);
	};
}
