#include "registry.h"
#include <fstream>
#include "error.h"

json* Registry::registry;

int Registry::getId(const std::string& category, const std::string& item)
{
	return (*registry)[category]["entries"][item]["protocol_id"].iValue();
}
std::string Registry::getName(const std::string& category, int id)
{
	//registry entries might not always be in order, watch out
	return (*registry)[category]["entries"][id].getName();
}

void Registry::loadRegistry()
{
	std::fstream registryFile("data/registries.json", std::ios::in);
	if (!registryFile.is_open()) throw runtimeError("Could not load registries");
	registry = json::parse(registryFile);
	registryFile.close();
}