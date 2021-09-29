#include "block.h"

std::string PaletteEntry::Block::getState(const std::string& stateName)
{
	json& stateProp = (*state)["properties"][stateName];
	return stateProp.getType() == json::string ? stateProp.value() : stateProp.stringValue();
}
void PaletteEntry::Block::setState(const std::string& stateName, const std::string& value)
{
	json& stateProps = (*state)["properties"];
	ull propCount = stateProps.getSize();
	BlockProperty* desiredProps = new BlockProperty[propCount];
	for (ull i = 0; i < propCount; i++)
	{
		json& prop = stateProps[i];
		BlockProperty& desiredProp = desiredProps[i];
		desiredProp.name = prop.getName();
		if (desiredProp.name == stateName) desiredProp.value = value;
		else desiredProp.value = prop.getType() == json::string ? prop.value() : prop.stringValue();
	}
	state = &Registry::getBlockState(Registry::getBlock(id));
	id = (*state)["id"].iValue();
}

void PaletteEntry::Block::set(int blockId)
{
	state = &Registry::getBlockState(blockId);
	id = (*state)["id"].iValue();
}
void PaletteEntry::Block::set(const std::string& blockName)
{
	state = &Registry::getBlockState(blockName);
	id = (*state)["id"].iValue();
}
void PaletteEntry::Block::set(const std::string& blockName, BlockProperty* properties)
{
	state = &Registry::getBlockState(blockName, properties);
	id = (*state)["id"].iValue();
}