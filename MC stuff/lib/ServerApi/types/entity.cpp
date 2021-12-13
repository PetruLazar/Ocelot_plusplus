#include "entity.h"
#include "../server/log.h"

#include "chat.h"
#include "slot.h"
#include "../nbt.h"
#include "particle.h"

#include <type_traits>

void Rotation3f::write(char*& buffer) const {
	x.write(buffer);
	y.write(buffer);
	z.write(buffer);
}

void VillagerData::write(char*& buffer) const {
	type.write(buffer);
	profession.write(buffer);
	level.write(buffer);
}

namespace Entity 
{
	void Modifier::write(char*& buffer) const {
		uuid.write(buffer);
		amount.write(buffer);
		*(buffer++) = operation;
	}

	void Property::write(char*& buffer) const {
		key.write(buffer);
		value.write(buffer);
		nOfModifiers.write(buffer);
		for (int i = 0; i < nOfModifiers; i++)
			modifiers[i]->write(buffer);
	}

	void Metadata::write(char*& buffer) const {
		*(buffer++) = index;

		if (index != 0xff) {
			varInt(dataType).write(buffer);

			switch (dataType) {
			case type::_Byte:
				*(buffer++) = *static_cast<Byte*>(value);
				break;
			case type::_varInt:
				static_cast<varInt*>(value)->write(buffer);
				break;
			case type::_Float:
				static_cast<bfloat*>(value)->write(buffer);
				break;
			case type::_String:
				static_cast<mcString*>(value)->write(buffer);
				break;
			case type::_Chat:
				static_cast<Chat*>(value)->write(buffer);
				break;
			case type::_optChat:
				static_cast<OptVar<Chat>*>(value)->write(buffer);
				break;
			case type::_Slot:
				static_cast<Slot*>(value)->write(buffer);
				break;
			case type::_Boolean:
				*(buffer++) = *static_cast<bool*>(value);
				break;
			case type::_Rotation:
				static_cast<Rotation3f*>(value)->write(buffer);
				break;
			case type::_Position:
				static_cast<Position*>(value)->write(buffer);
				break;
			case type::_OptPosition:
				static_cast<OptVar<Position>*>(value)->write(buffer);
				break;
			case type::_Direction:
				static_cast<varInt*>(value)->write(buffer);
				break;
			case type::_OptUUID:
				static_cast<OptVar<mcUUID>*>(value)->write(buffer);
				break;
			case type::_OptBlockID:
				static_cast<varInt*>(value)->write(buffer);
				break;
			case type::_NBT:
				static_cast<nbt*>(value)->write(buffer);
				break;
			case type::_Particle:
				static_cast<particle::Particle*>(value)->write(buffer, true);
				break;
			case type::_VillagerData:
				static_cast<VillagerData*>(value)->write(buffer);
				break;
			case type::_optVarInt:
				static_cast<varInt*>(value)->write(buffer);
				break;
			case type::_Pose:
				static_cast<varInt*>(value)->write(buffer);
				break;
			default:
				break;
			}
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

namespace Entity //all entity classes functions
{
	entity::entity(eidDispenser::General *eidDispenser, Byte attributes, varInt airTicks, Chat *customName, bool isCustomNameVisible, bool isSilent, bool hasGravity, pose thePose, varInt ticksFrozen)
		: attributes(attributes), airTicks(airTicks), customName(customName), isCustomNameVisible(isCustomNameVisible), isSilent(isSilent), hasGravity(hasGravity), thePose(thePose), ticksFrozen(ticksFrozen), euuid(new mcUUID(mcUUID::entity)), eidDispenser(eidDispenser), eid(eidDispenser->Alloc()) {}

	entity::entity(const entity& e)
		: attributes(e.attributes), airTicks(e.airTicks), customName(e.customName), isCustomNameVisible(e.isCustomNameVisible), isSilent(e.isSilent), hasGravity(e.hasGravity), thePose(e.thePose), ticksFrozen(e.ticksFrozen), euuid(new mcUUID(mcUUID::entity)), eidDispenser(e.eidDispenser), eid(e.eidDispenser->Alloc()) {}

	entity::~entity() {
		eidDispenser->Free(eid);
		delete euuid;
	}

	varInt entity::getEid() { return eid; }
}
