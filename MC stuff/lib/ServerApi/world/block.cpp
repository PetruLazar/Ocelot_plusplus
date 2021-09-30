#include "block.h"

BlockState::BlockState(int id) : id(id), state(&Registry::getBlockState(id)) { }
BlockState::BlockState(json* blockState) : state(blockState), id((*blockState)["id"].iValue()) { }
BlockState::BlockState(json& blockState) : state(&blockState), id(blockState["id"].iValue()) { }
BlockState::BlockState(const std::string blockName) : state(&Registry::getBlockState(blockName))
{
	id = (*state)["id"].iValue();
}
BlockState::BlockState(const std::string blockName, BlockProperty* properties) : state(&Registry::getBlockState(blockName, properties))
{
	id = (*state)["id"].iValue();
}

std::string BlockState::getState(const std::string& stateName) const
{
	json& stateProp = (*state)["properties"][stateName];
	return stateProp.getType() == json::string ? stateProp.value() : stateProp.stringValue();
}
void BlockState::setState(const std::string& stateName, const std::string& value)
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

void BlockState::set(int blockId)
{
	state = &Registry::getBlockState(blockId);
	id = (*state)["id"].iValue();
}
void BlockState::set(const std::string& blockName)
{
	state = &Registry::getBlockState(blockName);
	id = (*state)["id"].iValue();
}
void BlockState::set(const std::string& blockName, BlockProperty* properties)
{
	state = &Registry::getBlockState(blockName, properties);
	id = (*state)["id"].iValue();
}

bool BlockState::operator==(const BlockState& that) const
{
	return (int&)id == (int&)that.id;
}
void BlockState::operator=(const BlockState& that)
{
	id = that.id;
	state = that.state;
}

PaletteEntry::PaletteEntry(const BlockState& bl, short refCount) : block(bl), referenceCount(refCount) { }