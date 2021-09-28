#include "registry.h"
#include <fstream>
#include "error.h"

json* Registry::registries, * Registry::globalPalette;

BlockProperty::BlockProperty(const std::string& name, const std::string& val) : name(name), value(val) { }

int Registry::getId(const std::string& category, const std::string& item)
{
	return (*registries)[category]["entries"][item]["protocol_id"].iValue();
}
std::string Registry::getName(const std::string& category, int id)
{
	//registry entries might not always be in order, watch out
	return (*registries)[category]["entries"][id].getName();
}
int Registry::getBlockId(const std::string& blockName)
{
	json& blockStates = (*globalPalette)[blockName]["states"];
	ull stateCount = blockStates.getSize();
	for (ull i = 0; i < stateCount; i++)
	{
		json& state = blockStates[(int)i];
		//check if the default field is present and if it is true
		try
		{
			if (state["default"].bValue() == true)
			{
				return state["id"].iValue();
			}
		}
		catch (...) {}
	}
	throw runtimeError("no default state found");
}
int Registry::getBlockId(const std::string& blockName, BlockProperty* states)
{
	json& blockEntry = (*globalPalette)[blockName];
	ull propertyCount = blockEntry["properties"].getSize();
	json& blockStates = blockEntry["states"];
	ull stateCount = blockStates.getSize();
	for (ull i = 0; i < stateCount; i++)
	{
		json& state = blockStates[(int)i],
			& properties = state["properties"];
		//if any of the properties is different, skip this state
		bool cont = false;
		for (ull j = 0; j < propertyCount; j++)
		{
			json& propertyField = properties[states[j].name];
			if ((propertyField.getType() == json::string && propertyField.value() != states[j].value) || (propertyField.getType() == json::boolean && propertyField.stringValue() != states[j].value))
			{
				cont = true;
				break;
			}
		}
		if (cont) continue;
		//no different states found, return the id
		return state["id"].iValue();
	}
	throw runtimeError("desired state not found");
}

void Registry::loadRegistriesAndPalette()
{
	//load registries
	std::fstream jsonFile("data/registries.json", std::ios::in);
	if (!jsonFile.is_open()) throw runtimeError("Could not load registries");
	registries = json::parse(jsonFile);
	jsonFile.close();

	//load global palette
	jsonFile.open("data/blocks.json", std::ios::in);
	if (!jsonFile.is_open()) throw runtimeError("Could not load global palette");
	globalPalette = json::parse(jsonFile);
	jsonFile.close();
}