
#include "bossBar.h"

void bossBar::add::write(char*& buffer) const {
	title.write(buffer);
	health.write(buffer);
	varInt((int)barColor).write(buffer);
	varInt((int)barDivisions).write(buffer);
	*(buffer++) = flags;
}

void bossBar::remove::write(char*& buffer) const {

}

void bossBar::updateHealth::write(char*& buffer) const {
	health.write(buffer);
}

void bossBar::updateTitle::write(char*& buffer) const {
	title.write(buffer);
}

void bossBar::updateStyle::write(char*& buffer) const {
	varInt((int)barColor).write(buffer);
	varInt((int)barDivisions).write(buffer);
}

void bossBar::updateFlags::write(char*& buffer) const {
	*(buffer++) = flags;
}