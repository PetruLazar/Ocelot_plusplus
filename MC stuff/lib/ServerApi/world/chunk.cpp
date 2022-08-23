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
	lightData.resize((ull)sectionCount + 2);
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
void Chunk::read(std::istream& file)
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
	for (uint i = 0; i < lightData.size(); i++)
	{
		LightSection& sec = lightData[i];
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
	for (uint i = 0; i < lightData.size(); i++)
	{
		LightSection& sec = lightData[i];

		if (skyLightMask->getElement(i))
		{
			sec.skyLight->write(file);
		}

		if (blockLightMask->getElement(i))
		{
			sec.blockLight->write(file);
		}
	}
}

int Chunk::getBlock(int relX, int relY, int relZ)
{
	return sections[relY >> 4].getBlock(relX, relY & 0xf, relZ);
}
void Chunk::setBlock(int relX, int relY, int relZ, int blockid, nbt_compound* nbt_data)
{
	Section& section = sections[relY >> 4];
	bool hadBlocks = section.blockCount;
	section.setBlock(relX, relY & 0xf, relZ, blockid);
	if (nbt_data)
	{
		varInt type = Registry::getId(Registry::blockEntityRegistry, (*nbt_data)["id"].vString());
		BlockEntity* blEntity = new BlockEntity(relX << 4 | relZ, relY, type, nbt_data);
		blockEntities.emplace_back(blEntity);
		Log::debug() << "Nbt built: " << nbt_data->getStringValue() << Log::endl;
	}
	if ((bool)section.blockCount != hadBlocks)
	{
		//section mask modified
		sectionMask->setElement(relY >> 4, (bool)section.blockCount);
	}

	//change heightmaps? (once it is needed)

	//updatelight
}

void Chunk::writeSectionData(char*&)
{
	throw runtimeError("Chunk::writeSectionData not implemented yet");
}

void Chunk::addPlayer(Player* p)
{
	players.emplace_front(p);
}
void Chunk::removePlayer(Player* p)
{
	players.remove(p);
}
void Chunk::addEntity(Entity::entity* en)
{
	entities.emplace_front(en);
}
void Chunk::removeEntity(Entity::entity* en)
{
	entities.remove(en);
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
			BlockState::globalPalette[id]->randomTick(wld, cX | relX, reg_y | relY, cZ | relZ);
		}

		//ticks
		//...

		reg_y += 16;
	}
}