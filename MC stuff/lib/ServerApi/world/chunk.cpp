#include "../debug/mcexceptions.h"
#include "../types/utils.h"
#include "../world.h"

using namespace std;

Chunk::Chunk()
{
	heightmaps = nullptr;

	skyLightMask = nullptr;
	blockLightMask = nullptr;
	emptySkyLightMask = nullptr;
	emptyBlockLightMask = nullptr;

	sectionMask = nullptr;
}
Chunk::Chunk(ull worldHeight)
{
	//heightmap initialization
	heightmaps = new BitArray(256, bitCount(worldHeight));

	ull sectionCount = worldHeight >> 4;

	//section initialization
	sections.resize(sectionCount);

	//light section initialization
	lightSections.resize((ull)sectionCount + 2);
	skyLightMask = new BitArray(sectionCount + 2, 1);
	blockLightMask = new BitArray(sectionCount + 2, 1);
	emptySkyLightMask = new BitArray(sectionCount + 2, 1);
	emptyBlockLightMask = new BitArray(sectionCount + 2, 1);

	//section mask initialization
	sectionMask = new BitArray(sectionCount, 1);
}
Chunk::~Chunk()
{
	//chunk data
	delete heightmaps;
	delete sectionMask;

	//light data
	delete skyLightMask;
	delete blockLightMask;
	delete emptySkyLightMask;
	delete emptyBlockLightMask;
}

//reading a chunk that already contains data causes a memory leak!
void Chunk::read(istream& file)
{
	//chunk data

	//read sectionMask and heightmap
	sectionMask->read(file);
	heightmaps->read(file);

	for (Section& sec : sections)
	{
		sec.biomes.read(file);
		sec.blockStates.read(file);

		//read blockCount
		file.read((char*)(&sec.blockCount), 2);
	}

	//light data

	//read light masks
	skyLightMask->read(file);
	blockLightMask->read(file);
	emptySkyLightMask->read(file);
	emptyBlockLightMask->read(file);

	//read light data for each section
	for (uint i = 0; i < lightSections.size(); i++)
	{
		LightSection& sec = lightSections[i];
		sec.skyLight = new BitArray(4096, 4);
		if (skyLightMask->getElement(i))
		{
			//bit set in skyLightMask
			sec.skyLight->read(file);
		}
		else
		{
			//bit not set in skyLightMask
			if (emptySkyLightMask->getElement(i))
			{
				//bit set in emptySkyLightMask
				//section light has all zeros
			}
			else
			{
				//bit not set in emptySkyLightMask
				//should calculate light
			}
		}

		sec.blockLight = new BitArray(4096, 4);
		if (blockLightMask->getElement(i))
		{
			//bit set in blockLightMask
			sec.blockLight->read(file);
		}
		else
		{
			//bit not set in skyLightMask
			if (emptyBlockLightMask->getElement(i))
			{
				//bit set in emptyBlockLightMask
				//section light has all zeros
			}
			else
			{
				//bit not set in emptyBlockLightMask
				//should calculate light
			}
		}
	}

	//read block entities
	uint blockEntityCount;
	file.read((char*)&blockEntityCount, sizeof(blockEntityCount));
	blockEntities.resize(blockEntityCount);
	for (auto& entity : blockEntities)
	{
		entity = new BlockEntity();
		entity->read(file);
	}
	
	//read entities - reserved for now, must be none
	uint entityCount;
	file.read((char*)&entityCount, sizeof(entityCount));
	if (entityCount)
		Log::warn() << "Found chunk with a non-zero entity count" << Log::flush;
}
void Chunk::write(ostream& file)
{
	//chunk data

	//write sectionMask and heightmap
	sectionMask->write(file);
	heightmaps->write(file);

	//write each section
	for (Section& sec : sections)
	{
		sec.biomes.write(file);
		sec.blockStates.write(file);

		//write blockCount
		file.write((char*)(&sec.blockCount), 2);
	}

	//light data

	//write light masks
	skyLightMask->write(file);
	blockLightMask->write(file);
	emptySkyLightMask->write(file);
	emptyBlockLightMask->write(file);

	//write light data for each section
	for (uint i = 0; i < lightSections.size(); i++)
	{
		LightSection& sec = lightSections[i];

		if (skyLightMask->getElement(i))
		{
			sec.skyLight->write(file);
		}

		if (blockLightMask->getElement(i))
		{
			sec.blockLight->write(file);
		}
	}

	//write block entities
	uint blockEntityCount = blockEntities.size();
	file.write((char*)&blockEntityCount, sizeof(blockEntityCount));
	for (auto entity : blockEntities)
	{
		entity->write(file);
	}
	
	//write entities - reserved for now, must be none
	uint entityCount = 0;
	file.write((char*)&entityCount, sizeof(entityCount));
}

nbt_compound* Chunk::getNbt(int relX, int relY, int relZ)
{
	//auto entity = getBlockEntity(relX, relY, relZ);
	//if (entity) return entity->getNbt();
	return nullptr;
}
int Chunk::getBlockEntityIndex(int relX, int relY, int relZ)
{
	Byte packedXZ = relX << 4 | relZ;
	for (uint i = blockEntities.size() - 1; i != (uint)-1; i--)
	{
		auto entity = blockEntities[i];
		if (entity->y == relY && entity->packedXZ == packedXZ)
			return (int)i;
	}
	return -1;
}
bool Chunk::setBlock(int relX, int relY, int relZ, int blockid, nbt_compound* nbt_data)
{
	Section& section = sections[relY >> 4];
	bool hadBlocks = section.blockCount;
	bool ret = section.setBlock(relX, relY & 0xf, relZ, blockid);
	int blockEntityIndex = getBlockEntityIndex(relX, relY, relZ);
	//if there already is a block entity at this block, replace old nbt with new nbt, if there needs to be a new nbt
	//if there is no new nbt, delete the old entity 
	if (blockEntityIndex == -1)
	{
		// there is no nbt yet
		if (nbt_data)
		{
			varInt type = Registry::getId(Registry::blockEntityRegistry, (*nbt_data)["id"].vString());
			BlockEntity* blEntity = new BlockEntity(relX << 4 | relZ, relY, type, nbt_data);
			blockEntities.emplace_back(blEntity);
			Log::debug() << "Nbt built: " << nbt_data->getStringValue() << Log::endl;
		}
	}
	else
	{
		//there already is nbt
		if (nbt_data)
		{
			//replace nbt
			BlockEntity* entity = blockEntities[blockEntityIndex];
			delete entity->tags;
			entity->tags = nbt_data;
			entity->type = Registry::getId(Registry::blockEntityRegistry, (*nbt_data)["id"].vString());
		}
		else
		{
			//delete block entity
			delete blockEntities[blockEntityIndex];
			blockEntities.erase(blockEntities.begin() + blockEntityIndex);
		}
	}

	if ((bool)section.blockCount != hadBlocks)
	{
		//section mask modified
		sectionMask->setElement(relY >> 4, (bool)section.blockCount);
	}

	//change heightmaps? (once it is needed)

	//updatelight

	return ret;
}

void Chunk::setSkyLight(int relX, int relY, int relZ, Byte value)
{
	LightSection& sec = lightSections[((ull)relY >> 4) + 1];
	sec.setSkyLight(relX, relY & 0xf, relZ, value);
	//change light masks
}
void Chunk::setBlockLight(int relX, int relY, int relZ, Byte value)
{
	LightSection& sec = lightSections[((ull)relY >> 4) + 1];
	sec.setBlockLight(relX, relY & 0xf, relZ, value);
	//change light masks
}

void Chunk::writeSectionData(char*&)
{
	throw runtimeError("Chunk::writeSectionData not implemented yet");
}

void Chunk::tick(World* wld, int cX, int cZ, int randomTickSpeed)
{
	int reg_y = 0;
	cX <<= 4;
	cZ <<= 4;
	for (Section& sec : sections)
	{
		if (sec.blockCount == 0)
		{
			reg_y += 16;
			continue;
		}

		//random ticks
		for (int randomTickCount = 0; randomTickCount < randomTickSpeed; randomTickCount++)
		{
			int relX = rand() & 0xff,
				relY = rand() & 0xf,
				relZ = relX >> 4;
			relX &= 0xf;
			int id = sec.getBlock(relX, relY, relZ);
			auto block = BlockState::globalPalette[id];
			if (block)
				block->randomTick(wld, cX | relX, reg_y | relY, cZ | relZ);
		}

		//ticks
		//...

		reg_y += 16;
	}
}