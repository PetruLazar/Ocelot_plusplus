
#include "trades.h"

void trade::write(char*& buffer) const {
	inputFirst.write(buffer);
	outputItem.write(buffer);
	*(buffer++) = hasSecondItem;
	if(hasSecondItem)
		inputSecond.write(buffer);
	*(buffer++) = tradeDisabled;
	nOfTradeUses.write(buffer);
	maxOfTradeUses.write(buffer);
	xp.write(buffer);
	specialPrice.write(buffer);
	priceMultiplier.write(buffer);
	demand.write(buffer);
}