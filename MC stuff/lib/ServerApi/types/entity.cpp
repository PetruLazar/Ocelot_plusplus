#include "entity.h"
#include "../server/log.h"

EntityProperty::EntityProperty(const mcString& key, double value) : key(key), value(value) { }

namespace EidDispenser
{
	General::Iterator::Iterator(int eid, Iterator* next) : eid(eid), next(next) { }

	General::General(type dispenserType) : first(new Iterator(dispenserType)) { }
	General::~General()
	{
		clear(first);
	}

	void General::clear(Iterator* it)
	{
		if (it->next) clear(it->next);
		delete it;
	}
	int General::Alloc()
	{
		Iterator* i;
		for (i = first; i->next && i->eid == i->next->eid - 2; i = i->next);
		i->next = new Iterator(i->eid + 2, i->next);
		return i->next->eid;
	}
	void General::Free(int eid)
	{
		for (Iterator* i = first; i->next; i = i->next) if (i->next->eid == eid)
		{
			Iterator* i2 = i->next;
			i->next = i->next->next;
			delete i2;
			return;
		}
	}

	Player::Player() : General(player) { }
	Entity::Entity() : General(entity) { }
}