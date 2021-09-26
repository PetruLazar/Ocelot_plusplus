#include "customGen.h"

Chunk* generate(World* wld, int chunkX, int chunkZ)
{
	int wldHeight = wld->height;

	Chunk* ch = new Chunk(wldHeight);

	ull sectionCount = wldHeight >> 4;

	//light section below ground - completly dark
	{
		//mark light section as empty
		ch->emptyBlockLightMask->setElement(0, 1);
		ch->emptySkyLightMask->setElement(0, 1);
	}
	//light section above ground - completly dark
	{
		//mark light section as empty
		ch->emptyBlockLightMask->setElement(sectionCount, 1);
		ch->emptySkyLightMask->setElement(sectionCount, 1);
	}

	for (ull i = 0; i < sectionCount; i++)
	{
		Section& section = ch->sections[i];
		LightSection& lightSection = ch->lightData[i + 1];

		//SECTION DATA - this is a section full of air

		//set corresponding bit in chunk's section mask
		//ch->sectionMask->setElement(i, 1);

		//block data
		section.blockCount = 0;
		section.bitsPerBlock = 4;
		section.useGlobalPallete = false;
		section.pallete.push_back(0); // 0 is air
		section.blockStates = new BitArray(4096, section.bitsPerBlock);

		//biome data
		for (ull i = 0; i < sizeof(section.biomes) / sizeof(ull); i++) ((ull*)section.biomes)[i] = 0;

		//LIGHTSECTION DATA - this is a completly dark section
		//lightSection.skyLight = new BitArray(4096, 4);
		//lightSection.blockLight = new BitArray(4096, 4);

		//mark light sections as empty
		ch->emptyBlockLightMask->setElement(i + 1, 1);
		ch->emptySkyLightMask->setElement(i + 1, 1);
	}

	return ch;
}