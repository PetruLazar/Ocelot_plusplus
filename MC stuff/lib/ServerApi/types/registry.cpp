#include "registry.h"
#include <fstream>
#include "error.h"

json* Registry::registries, * Registry::globalPalette;

int Registry::getId(const std::string& category, const std::string& item)
{
	return (*registries)[category]["entries"][item]["protocol_id"].iValue();
}
std::string Registry::getName(const std::string& category, int id)
{
	//registry entries might not always be in order, watch out
	return (*registries)[category]["entries"][id].getName();
}

void Registry::loadRegistriesAndPalette()
{
	std::fstream registryFile("data/registries.json", std::ios::in);
	if (!registryFile.is_open())
		throw runtimeError("Could not load registries");

	registries = json::parse(registryFile);
	registryFile.close();
}