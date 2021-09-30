#include "block.h"

PaletteEntry::Block::Block(int id) : id(id), state(&Registry::getBlockState(id)) { }
PaletteEntry::Block::Block(json* blockState) : state(blockState), id((*blockState)["id"].iValue()) { }
PaletteEntry::Block::Block(json& blockState) : state(&blockState), id(blockState["id"].iValue()) { }
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
		json& prop = stateProps[(int)i];
		BlockProperty& desiredProp = desiredProps[i];
		desiredProp.name = prop.getName();
		if (desiredProp.name == stateName) desiredProp.value = value;
		else desiredProp.value = prop.getType() == json::string ? prop.value() : prop.stringValue();
	}
	state = &Registry::getBlockState(Registry::getBlock(id), desiredProps);
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

bool PaletteEntry::Block::operator==(const Block& that)
{
	return (int&)id == (int&)that.id;
}
void PaletteEntry::Block::operator=(const Block& that)
{
	id = that.id;
	state = that.state;
}