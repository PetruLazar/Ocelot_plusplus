#include "registry.h"
#include <fstream>
#include "error.h"

json* Registry::registries, * Registry::globalPalette;

BlockProperty::BlockProperty(const std::string& name, const std::string& val) : name(name), value(val) { }
BlockProperty::BlockProperty() { }

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
	return getBlockState(blockName)["id"].iValue();
}
int Registry::getBlockId(const std::string& blockName, BlockProperty* states)
{
	return getBlockState(blockName, states)["id"].iValue();
}

json& Registry::getBlockState(const std::string& blockName)
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
				return state;
			}
		}
		catch (...) {}
	}
	throw runtimeError("No default state found");
}
json& Registry::getBlockState(const std::string& blockName, BlockProperty* properties)
{
	json& blockEntry = (*globalPalette)[blockName];
	ull propertyCount = blockEntry["properties"].getSize();
	json& blockStates = blockEntry["states"];
	ull stateCount = blockStates.getSize();
	for (ull i = 0; i < stateCount; i++)
	{
		json& state = blockStates[(int)i],
			& stateProperties = state["properties"];
		//if any of the properties is different, skip this state
		bool cont = false;
		for (ull j = 0; j < propertyCount; j++)
		{
			json& propertyField = stateProperties[properties[j].name];
			if ((propertyField.getType() == json::type::string && propertyField.value() != properties[j].value) || (propertyField.getType() == json::type::boolean && propertyField.stringValue() != properties[j].value))
			{
				cont = true;
				break;
			}
		}
		if (cont) continue;
		//no different properties found, return the state
		delete[] properties;
		return state;
	}
	delete[] properties;
	throw runtimeError("Desired state not found");
}
json& Registry::getBlockState(int id)
{
	ull blockCount = globalPalette->getSize();
	for (ull i = 0; i < blockCount; i++)
	{
		json& blockStates = (*globalPalette)[(int)i]["states"];
		ull stateCount = blockStates.getSize() - 1;
		json& lastState = blockStates[(int)stateCount];
		int lastStateId = lastState["id"].iValue();
		if (lastStateId == id) return lastState;
		else if (lastStateId > id)
		{
			int diff = lastStateId - id;
			return blockStates[(int)stateCount - diff];
		}
	}
	throw runtimeError("No block found");
}

std::string Registry::getBlock(int id)
{
	ull blockCount = globalPalette->getSize();
	for (ull i = 0; i < blockCount; i++)
	{
		json& block = (*globalPalette)[(int)i],
			& blockStates = block["states"];
		if (blockStates[(int)blockStates.getSize() - 1]["id"].iValue() >= id) return block.getName();
	}
	throw runtimeError("No block found");
}
std::string Registry::getBlock(const json& blockState)
{
	ull blockCount = globalPalette->getSize();
	for (ull i = 0; i < blockCount; i++)
	{
		json& block = (*globalPalette)[(int)i],
			& blockStates = block["states"];
		ull stateCount = blockStates.getSize();

		for (ull j = 0; j < stateCount; j++) if (&blockStates[(int)j] == &blockState) return block.getName();
	}
	throw runtimeError("No block found");
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