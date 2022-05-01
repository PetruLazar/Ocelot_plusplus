
#include "windower.h"

#include "../server/log.h"

unsigned mcp::windower::open(window::type theWindow)
{
	this->que.emplace(std::make_pair(theWindow, indexer));
	return indexer++;
}
void mcp::windower::close(unsigned theID)
{
	if (theID == this->que.front().second)
	{
		this->que.pop();
		indexer--;
	}
	else
		Log::warn() << "Player closed a not-last-opened window." << Log::endl;
}
window::type mcp::windower::getLatest(unsigned theID)
{
	if (theID == this->que.back().second)
		return que.back().first;

	Log::warn() << "Player got a wrong ID latest." << Log::endl;
	return que.back().first;
}