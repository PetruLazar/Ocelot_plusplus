#pragma once
#include "slot.h"
#include "endian.h"

class trade
{
private:
	Slot inputFirst, outputItem, inputSecond;
	bool hasSecondItem = false, tradeDisabled;
	bint nOfTradeUses, maxOfTradeUses, xp, specialPrice, demand;
	bfloat priceMultiplier;

public:
	trade(Slot inputFirst, Slot outputItem, bool tradeDisabled, bint nOfTradeUses, bint maxOfTradeUses, bint xp, bint specialPrice, bfloat priceMultiplier, bint demand)
		: inputFirst(inputFirst), outputItem(outputItem), tradeDisabled(tradeDisabled), nOfTradeUses(nOfTradeUses), xp(xp), specialPrice(specialPrice), priceMultiplier(priceMultiplier), demand(demand) {}

	trade(Slot inputFirst, Slot outputItem, bool hasSecondItem, Slot inputSecond, bool tradeDisabled, bint nOfTradeUses, bint maxOfTradeUses, bint xp, bint specialPrice, bfloat priceMultiplier, bint demand)
		: inputFirst(inputFirst), outputItem(outputItem), hasSecondItem(hasSecondItem), inputSecond(inputSecond), tradeDisabled(tradeDisabled), nOfTradeUses(nOfTradeUses), xp(xp), specialPrice(specialPrice), priceMultiplier(priceMultiplier), demand(demand) {}

	void write(char*& buffer) const;
};