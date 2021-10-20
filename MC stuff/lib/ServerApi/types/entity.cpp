#include "entity.h"
#include "../server/log.h"

#include "chat.h"
#include "slot.h"
#include "../nbt.h"
#include "particle.h"

void entity::Modifier::write(char*& buffer) const {
	uuid.write(buffer);
	amount.write(buffer);
	*(buffer++) = operation;
}

void entity::Property::write(char*& buffer) const {
	key.write(buffer);
	value.write(buffer);
	nOfModifiers.write(buffer);
	for(int i = 0; i < nOfModifiers; i++)
		modifiers[i].write(buffer);
}

void entity::Metadata::write(char*& buffer) const {
	*(buffer++) = index;

	if (index != 0xff) {
		type.write(buffer);

		bool optional;
		switch (type) {
		case 0:
			*(buffer++) = *static_cast<Byte*>(value);
			break;
		case 1:
			static_cast<varInt*>(value)->write(buffer);
			break;
		case 2:
			static_cast<bfloat*>(value)->write(buffer);
			break;
		case 3:
			static_cast<mcString*>(value)->write(buffer);
			break;
		case 4:
			static_cast<Chat*>(value)->write(buffer);
			break;
		case 5:
			optional = static_cast<bool*>(value);
			*(buffer++) = optional;
			if (optional)
				static_cast<Chat*>(value)->write(buffer);
			break;
		case 6:
			static_cast<Slot*>(value)->write(buffer);
			break;
		case 7:
			*(buffer++) = *static_cast<bool*>(value);
			break;
		case 8:
			static_cast<bfloat*>(value)->write(buffer);
			static_cast<bfloat*>(value)->write(buffer);
			static_cast<bfloat*>(value)->write(buffer);
			break;
		case 9:
			static_cast<Position*>(value)->write(buffer);
			break;
		case 10:
			optional = static_cast<bool*>(value);
			*(buffer++) = optional;
			if (optional) {
				static_cast<bfloat*>(value)->write(buffer);
				static_cast<bfloat*>(value)->write(buffer);
				static_cast<bfloat*>(value)->write(buffer);
			}
			break;
		case 11:
			static_cast<varInt*>(value)->write(buffer);
			break;
		case 12:
			optional = static_cast<bool*>(value);
			*(buffer++) = optional;
			if (optional)
				static_cast<mcUUID*>(value)->write(buffer);
			break;
		case 13:
			static_cast<varInt*>(value)->write(buffer);
			break;
		case 14:
			static_cast<nbt*>(value)->write(buffer);
			break;
		case 15:
			static_cast<Particle*>(value)->write(buffer);
			break;
		case 16:
			static_cast<varInt*>(value)->write(buffer);
			static_cast<varInt*>(value)->write(buffer);
			static_cast<varInt*>(value)->write(buffer);
			break;
		case 17:
			static_cast<varInt*>(value)->write(buffer);
			break;
		case 18:
			static_cast<varInt*>(value)->write(buffer);
			break;
		default:
			break;
		}
	}
}

namespace eidDispenser
{
	General::Iterator::Iterator(int eid, Iterator* next) : eid(eid), next(next) { }

	General::General(type dispenserType) : first(new Iterator((int)dispenserType)) { }
	General::~General() {
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

	Player::Player() : General(type::player) { }
	Entity::Entity() : General(type::entity) { }
}