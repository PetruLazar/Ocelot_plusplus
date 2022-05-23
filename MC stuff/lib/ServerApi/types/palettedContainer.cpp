#include "palettedContainer.h"
#include "utils.h"
#include "../world/block.h"

struct BasicPaletteEntry
{
	int id;
	int refCount;

	BasicPaletteEntry(int id, int refCount) : id(id), refCount(refCount) {}
};

template<Byte minBitsPerEntry, Byte maxBitsPerEntry, Byte globalPaletteBitsPerEntry>
class PalettedContainer
{
	//Byte bitsPerEntry; //? 0 or min-max or max
	std::vector<BasicPaletteEntry> palette;
	BitArray* values; //values
	const uint size;

	/*void incBitsPerEntry();
	void decBitsPerEntry();
	void incPaletteSize();
	void decPaletteSize();*/

	int getPaletteEntryIndex(int id) const
	{
		for (int i = (int)palette.size() - 1; i >= 0; i--) if (palette[i].id == id) return i;
		return -1;
	}

public:
	PalettedContainer(uint size,
		int single_value) :
		size(size),
		values(nullptr)
	{
		palette.emplace_back(single_value, size);
	};
	~PalettedContainer() { if (values) delete values; }

	void set(int index, int value)
	{
		if (!values)
		{
			//0 bits per block
			if (palette[0].id == value) return;
			//create the values
			palette[0].refCount--;
			palette.emplace_back(value, 1);

			values = new BitArray(size, minBitsPerEntry);
			values->setElement(index, 1);
			return;
		}
		//1+ bits per entry
		Byte currentBitsPerEntry = values->getBitsPerEntry();
		if (currentBitsPerEntry <= maxBitsPerEntry)
		{
			//local palette
			int currentIndex = (int)values->getElement(index);
			BasicPaletteEntry& current = palette[currentIndex];
			if (current.id == value) return;
			int toPlaceIndex = getPaletteEntryIndex(value);
			if (current.refCount == 1)
			{
				//this is the last of the current value
				if (toPlaceIndex == -1) //first of the new value
					current.id = value;
				else
				{
					//not the first of the new value, delete the old palette entry
					palette[toPlaceIndex].refCount++;
					//erase the entry from the palette
					palette.erase(palette.begin() + currentIndex);
					//check if bitsPerEntry need to change
					size_t newPaletteSize = palette.size();
					if (newPaletteSize == 1)
					{
						//only 1 palette entry left, go to single value mode
						delete values;
						values = nullptr;
						return;
					}
					values->setElement(index, toPlaceIndex);

					//decrease values bigger than the deleted index, so that all the values continue to point to a valid index
					for (uint i = 0; i < size; i++)
					{
						ull val = values->getElement(i);
						if (val > currentIndex) values->setElement(i, --val);
					}

					Byte newBitsPerEntry = bitCount(newPaletteSize - 1);
					if (newBitsPerEntry != currentBitsPerEntry && newBitsPerEntry <= minBitsPerEntry)
						values->changeBitsPerEntry(newBitsPerEntry);

					//erasing from the palette when bitsPerEntry did not change only introduces the overhead of decrementing all the entries that point to a palette entry that has moved due to deletion
					//maybe optimise so that palette entries stay in the palette until the bits per entry change?
				}
			}
			else
			{
				//there are more of the current value
				current.refCount--;
				if (toPlaceIndex == -1)
				{
					ull oldPaletteSize = palette.size();
					//first of the new values, increase the palette size
					palette.emplace_back(value, 1);
					//check the bitsPerEntry
					Byte newBitsPerEntry = bitCount(oldPaletteSize);

					if (newBitsPerEntry == currentBitsPerEntry) //bits per entry did not change
						return;

					if (newBitsPerEntry > maxBitsPerEntry)
					{
						//go to global palette
						values->changeBitsPerEntry(globalPaletteBitsPerEntry);
						values->setElement(index, oldPaletteSize);
						for (uint i = 0; i < size; i++) values->setElement(i, palette[values->getElement(i)].id);
					}
					else
					{
						if (newBitsPerEntry != currentBitsPerEntry) //bits per entry are different
						{
							values->changeBitsPerEntry(newBitsPerEntry);
						}
						values->setElement(index, oldPaletteSize);
					}
				}
				else
				{
					//not the first of the desired value
					palette[toPlaceIndex].refCount++;
					values->setElement(index, toPlaceIndex);
				}
			}

			return;
		}

		//global Palette
		int currentValue = (int)values->getElement(index);
		if (currentValue == value) return;
		int currentIndex = getPaletteEntryIndex(currentValue);
		BasicPaletteEntry& current = palette[currentIndex];
		int toPlaceIndex = getPaletteEntryIndex(value);
		values->setElement(index, value);
		if (current.refCount == 1)
		{
			//last block of current value
			if (toPlaceIndex == -1) //first toPlace value
				current.id = value;
			else
			{
				//not the first toPlace value, remove the current value from palette
				palette[toPlaceIndex].refCount++;
				palette.erase(palette.begin() + currentIndex);

				//check if it is possible to go to local palette
				Byte newBitsPerBlock = bitCount(palette.size() - 1);
				if (newBitsPerBlock <= maxBitsPerEntry)
				{
					//go to local palette (expensive)
					for (uint i = 0; i < size; i++)
						values->setElement(i, getPaletteEntryIndex((int)values->getElement(i)));
					values->changeBitsPerEntry(newBitsPerBlock);
				}
			}
		}
		else
		{
			current.refCount--;
			//not last block of current value
			if (toPlaceIndex == -1)
				palette.emplace_back(value, 1);
			else
				palette[toPlaceIndex].refCount++;
		}
	}

	int get(int index) const
	{
		if (!values) return palette[0].id;
		return values->getBitsPerEntry() == globalPaletteBitsPerEntry ? (int)values->getElement(index) : palette[values->getElement(index)].id;
	}

	void write(char*&) const;
	void read(char*&);
};

using blockTemplate = PalettedContainer<4, 8, bitCount(20342)>;
using biomeTemplate = PalettedContainer<1, 3, 6>; //6?

inline blockTemplate* asBlock(void* p) { return (blockTemplate*)p; }
inline biomeTemplate* asBiome(void* p) { return (biomeTemplate*)p; }

BlockStatesContainer::BlockStatesContainer() : container(new blockTemplate(16 * 16 * 16, 0)) {}
BlockStatesContainer::~BlockStatesContainer() { delete container; }

int BlockStatesContainer::get(int index) const { return asBlock(container)->get(index); }
void BlockStatesContainer::set(int index, int value) { asBlock(container)->set(index, value); }

BiomesContainer::BiomesContainer() : container(new biomeTemplate(4 * 4 * 4, 0)) {}
BiomesContainer::~BiomesContainer() { delete container; }

int BiomesContainer::get(int index) const { return asBiome(container)->get(index); }
void BiomesContainer::set(int index, int value) { asBiome(container)->set(index, value); }