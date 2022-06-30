#include "customGen.h"

Chunk* generate(World* wld, int chunkX, int chunkZ)
{
	int wldHeight = wld->height;

	Chunk* ch = new Chunk(wldHeight);

	ull sectionCount = wldHeight >> 4;

	if (chunkX == 0 && chunkZ == 0)
	{
		Byte lightData[4096]{};
		for (int i = 0; i < 4096 / 8; i++) ((ull*)(lightData))[i] = 0xffffffffffffffff;

		for (int i = 0; i < 256; i++) ch->heightmaps->setElement(i, 2);

		//light section below ground - empty
		{
			//mark light section as empty
			ch->skyLightMask->setElement(0, 1);
			ch->blockLightMask->setElement(0, 1);
			ch->lightData[0].skyLight = new BitArray(4096, 4, (Byte*)lightData);
			ch->lightData[0].blockLight = new BitArray(4096, 4, (Byte*)lightData);
		}
		//light section above ground - empty
		{
			//mark light section as empty
			ch->blockLightMask->setElement(sectionCount, 1);
			ch->skyLightMask->setElement(sectionCount, 1);
			ch->lightData[sectionCount].skyLight = new BitArray(4096, 4, (Byte*)lightData);
			ch->lightData[sectionCount].blockLight = new BitArray(4096, 4, (Byte*)lightData);
		}

		for (ull i = 0; i < sectionCount; i++)
		{
			Section& section = ch->sections[i];
			LightSection& lightSection = ch->lightData[i + 1];

			//SECTION DATA - this is a section full of air

			//set corresponding bit in chunk's section mask
			ch->sectionMask->setElement(i, 1);

			//block data
			section.blockCount = 0x80;

			//section.palette.emplace_back(BlockState("minecraft:air"), 0xf80);
			//section.palette.emplace_back(BlockState("minecraft:stone"), 0x80); //128 stone blocks

			//			y   z   x
			//Byte blocks[16][16][16]{ 0 };

				//upper half
			for (int z = 1; z < 16; z += 4) for (int x = 0; x < 16; x++)
			{
				//blocks[0][z][x] = 1;
				section.blockStates.set(z << 4 | x, 1);
				//blocks[8][z][x] = 1;
			}

			//section.blockStates = new BitArray(4096, section.bitsPerBlock, (Byte*)blocks);

			//biome data
			//section.biomes = new BitArray(64, World::currentBiomeBitsPerEntry);

			//LIGHTSECTION DATA - this section is light empty

			//mark light sections as empty
			ch->blockLightMask->setElement(i + 1, 1);
			ch->skyLightMask->setElement(i + 1, 1);
			lightSection.skyLight = new BitArray(4096, 4, (Byte*)lightData);
			lightSection.blockLight = new BitArray(4096, 4, (Byte*)lightData);
		}

		for (int stateid = 0; stateid < 20342; stateid++)
		{
			int y = stateid / 128 * 8 + 1;
			int z = (stateid % 128) / 16 * 2;
			int x = stateid % 128 % 16;

			ch->setBlock(x, y, z, stateid);
		}
	}
	else
	{
		Byte lightData[4096]{};
		for (int i = 0; i < 4096 / 8; i++) ((ull*)(lightData))[i] = 0xffffffffffffffff;

		//light section below ground - empty
		{
			//mark light section as empty
			ch->skyLightMask->setElement(0, 1);
			ch->blockLightMask->setElement(0, 1);
			ch->lightData[0].skyLight = new BitArray(4096, 4, (Byte*)lightData);
			ch->lightData[0].blockLight = new BitArray(4096, 4, (Byte*)lightData);
		}
		//light section above ground - empty
		{
			//mark light section as empty
			ch->blockLightMask->setElement(sectionCount, 1);
			ch->skyLightMask->setElement(sectionCount, 1);
			ch->lightData[sectionCount].skyLight = new BitArray(4096, 4, (Byte*)lightData);
			ch->lightData[sectionCount].blockLight = new BitArray(4096, 4, (Byte*)lightData);
		}

		for (ull i = 0; i < sectionCount; i++)
		{
			Section& section = ch->sections[i];
			LightSection& lightSection = ch->lightData[i + 1];

			//SECTION DATA - this is a section full of air

			//set corresponding bit in chunk's section mask
			ch->sectionMask->setElement(i, 0);

			//block data
			section.blockCount = 0x0;
			//section.palette.emplace_back(BlockState("minecraft:air"), 0x1000);
			//section.blockStates = new BitArray(4096, section.bitsPerBlock);

			//biome data
			//section.biomes = new BitArray(64, World::currentBiomeBitsPerEntry);

			//LIGHTSECTION DATA - this section is light empty

			//mark light sections as empty
			ch->blockLightMask->setElement(i + 1, 1);
			ch->skyLightMask->setElement(i + 1, 1);
			lightSection.skyLight = new BitArray(4096, 4, (Byte*)lightData);
			lightSection.blockLight = new BitArray(4096, 4, (Byte*)lightData);
		}
	}

	return ch;
}