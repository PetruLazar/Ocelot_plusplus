#include "entity.h"
#include "../server/log.h"

#include "chat.h"
#include "slot.h"
#include "../nbt.h"
#include "particle.h"

using namespace entity;

void Rotation3f::write(char*& buffer) const {
	x.write(buffer);
	y.write(buffer);
	z.write(buffer);
}

namespace entity 
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
			modifiers[i].write(buffer);
	}

	void Metadata::write(char*& buffer) const {
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

namespace entity //all entity classes functions
{
	entity::entity(eidDispenser::Entity& eidDispenser, Byte attributes, varInt airTicks, Chat customName, bool isCustomNameVisible, bool isSilent, bool hasGravity, pose thePose, varInt ticksFrozen)
		: attributes(attributes), airTicks(airTicks), customName(customName), isCustomNameVisible(isCustomNameVisible), isSilent(isSilent), hasGravity(hasGravity), thePose(thePose), ticksFrozen(ticksFrozen), euuid(mcUUID::entity), eidDispenser(eidDispenser), eid(eidDispenser.Alloc()) {}

	entity::entity(const entity& e)
		: attributes(e.attributes), airTicks(e.airTicks), customName(e.customName), isCustomNameVisible(e.isCustomNameVisible), isSilent(e.isSilent), hasGravity(e.hasGravity), thePose(e.thePose), ticksFrozen(e.ticksFrozen), euuid(mcUUID::entity), eidDispenser(eidDispenser), eid(eidDispenser.Alloc()){}

	entity::~entity() {
		eidDispenser.Free(eid);
	}

	mcUUID entity::getUuid() { return euuid; }
	varInt entity::getEid() { return eid; }

	void entity::write(char*& buffer) const {
		*(buffer++) = attributes;
		airTicks.write(buffer);
		customName.write(buffer);
		*(buffer++) = isCustomNameVisible;
		*(buffer++) = isSilent;
		*(buffer++) = hasGravity;
		varInt((int)thePose).write(buffer);
		ticksFrozen.write(buffer);
	}

	void thrownEgg::write(char*& buffer) const {
		entity::write(buffer);
		item.write(buffer);
	}
	void thrownEnderPearl::write(char*& buffer) const {
		entity::write(buffer);
		item.write(buffer);
	}
	void thrownExperienceBottle::write(char*& buffer) const {
		entity::write(buffer);
		item.write(buffer);
	}
	void thrownPotion::write(char*& buffer) const {
		entity::write(buffer);
		item.write(buffer);
	}
	void snowball::write(char*& buffer) const {
		entity::write(buffer);
		item.write(buffer);
	}
	void eyeOfEnder::write(char*& buffer) const {
		entity::write(buffer);
		item.write(buffer);
	}
	void fallingBlock::write(char*& buffer) const {
		entity::write(buffer);
		position.write(buffer);
	}
	void areaEffectCloud::write(char*& buffer) const {
		entity::write(buffer);
		radius.write(buffer);
		color.write(buffer);
		*(buffer++) = ignoreRadius;
		theParticle->write(buffer);
	}
	void fishingHook::write(char*& buffer) const {
		entity::write(buffer);
		hookedEntity.write(buffer);
		*(buffer++) = isCatchable;
	}
	void AbstractArrow::write(char*& buffer) const {
		entity::write(buffer);
		*(buffer++) = attributes;
		*(buffer++) = piercingLevel;
	}
	void arrow::write(char*& buffer) const {
		AbstractArrow::write(buffer);
		color.write(buffer);
	}
	void thrownTrident::write(char*& buffer) const {
		AbstractArrow::write(buffer);
		loyaltyLevel.write(buffer);
		*(buffer++) = hasEnchantmentGlint;
	}
	void boat::write(char*& buffer) const {
		entity::write(buffer);
		timeSinceLastHit.write(buffer);
		forwardDirection.write(buffer);
		damageTaken.write(buffer);
		type.write(buffer);
		*(buffer++) = isLeftPaddleTurning;
		*(buffer++) = isRightPaddleTurning;
		splashTimer.write(buffer);
	}
	void endCrystal::write(char*& buffer) const {
		entity::write(buffer);
		beamTarget.write(buffer);
		*(buffer++) = showBottom;
	}
	void smallFireball::write(char*& buffer) const {
		entity::write(buffer);
		item.write(buffer);
	}
	void fireball::write(char*& buffer) const {
		entity::write(buffer);
		item.write(buffer);
	}
	void witherSkull::write(char*& buffer) const {
		entity::write(buffer);
		*(buffer++) = isInvulnerable;
	}
	void firework::write(char*& buffer) const {
		entity::write(buffer);
		fireworkInfo.write(buffer);
		entityID.write(buffer);
		*(buffer++) = isShotAtAngle;
	}
	void itemFrame::write(char*& buffer) const {
		entity::write(buffer);
		theItem.write(buffer);
		rotation.write(buffer);
	}
	void item::write(char*& buffer) const {
		entity::write(buffer);
		theItem.write(buffer);
	}
	void LivingEntity::write(char*& buffer) const {
		entity::write(buffer);
		*(buffer++) = attributes;
		health.write(buffer);
		potionEffectColor.write(buffer);
		*(buffer++) = isPotionEffectAmbient;
		nOfArrowsInEntity.write(buffer);
		nOfBeeStingersInEntity.write(buffer);
		locationOfBed.write(buffer);
	}
	void player::write(char*& buffer) const {
		LivingEntity::write(buffer);
		additionalHearts.write(buffer);
		score.write(buffer);
		*(buffer++) = attributes;
		*(buffer++) = mainHand;
		leftShoulderEntityData->write(buffer);
		rightShoulderEntityData->write(buffer);
	}
	void armorStand::write(char*& buffer) const {
		LivingEntity::write(buffer);
		*(buffer++) = attributes;
		head.write(buffer);
		body.write(buffer);
		leftArm.write(buffer);
		rightArm.write(buffer);
		leftLeg.write(buffer);
		rightLeg.write(buffer);
	}
	void Mob::write(char*& buffer) const {
		LivingEntity::write(buffer);
		*(buffer++) = attributes;
	}
	void bat::write(char*& buffer) const {
		AmbientCreature::write(buffer);
		*(buffer++) = attributes;
	}
	void dolphin::write(char*& buffer) const {
		WaterAnimal::write(buffer);
		treasurePosition.write(buffer);
		*(buffer++) = canFindTreasure;
		*(buffer++) = hasFish;
	}
	void AbstractFish::write(char*& buffer) const {
		WaterAnimal::write(buffer);
		*(buffer++) = fromBucket;
	}
	void pufferFish::write(char*& buffer) const {
		WaterAnimal::write(buffer);
		puffState.write(buffer);
	}
	void tropicalFish::write(char*& buffer) const {
		WaterAnimal::write(buffer);
		variant.write(buffer);
	}
	void AgeableMob::write(char*& buffer) const {
		PathfinderMob::write(buffer);
		*(buffer++) = isBaby;
	}
	void AbstractHorse::write(char*& buffer) const {
		Animal::write(buffer);
		*(buffer++) = attributes;
		owner.write(buffer);
	}
	void horse::write(char*& buffer) const {
		AbstractHorse::write(buffer);
		variant.write(buffer);
	}
	void chestedHorse::write(char*& buffer) const {
		AbstractHorse::write(buffer);
		*(buffer++) = hasChest;
	}
	void llama::write(char*& buffer) const {
		chestedHorse::write(buffer);
		strength.write(buffer);
		carpetColor.write(buffer);
		variant.write(buffer);
	}
	void bee::write(char*& buffer) const {
		Animal::write(buffer);
		*(buffer++) = attributes;
		angerTimeTicks.write(buffer);
	}
	void fox::write(char*& buffer) const {
		Animal::write(buffer);
		type.write(buffer);
		*(buffer++) = attributes;
		firstUUID.write(buffer);
		secondUUID.write(buffer);
	}
	void ocelot::write(char*& buffer) const {
		Animal::write(buffer);
		*(buffer++) = attributes;
	}
	void panda::write(char*& buffer) const {
		Animal::write(buffer);
		breedTimer.write(buffer);
		sneezeTimer.write(buffer);
		eatTimer.write(buffer);
		*(buffer++) = mainGene;
		*(buffer++) = hiddenGene;
		*(buffer++) = attributes;
	}
	void pig::write(char*& buffer) const {
		Animal::write(buffer);
		*(buffer++) = hasSaddle;
		boostCarrotTime.write(buffer);
	}
	void rabbit::write(char*& buffer) const {
		Animal::write(buffer);
		type.write(buffer);
	}
	void turtle::write(char*& buffer) const {
		Animal::write(buffer);
		homePosition.write(buffer);
		*(buffer++) = hasEggs;
		*(buffer++) = isLayingEggs;
		travelPosition.write(buffer);
		*(buffer++) = isGoingHome;
		*(buffer++) = isTravelling;
	}
	void polarBear::write(char*& buffer) const {
		Animal::write(buffer);
		*(buffer++) = isStandingUp;
	}
	void hoglin::write(char*& buffer) const {
		Animal::write(buffer);
		*(buffer++) = isImmune;
	}
	void mooshroom::write(char*& buffer) const {
		cow::write(buffer);
		variant.write(buffer);
	}
	void sheep::write(char*& buffer) const {
		Animal::write(buffer);
		*(buffer++) = attributes;
	}
	void strider::write(char*& buffer) const {
		Animal::write(buffer);
		boostTime.write(buffer);
		*(buffer++) = isShaking;
		*(buffer++) = hasSaddle;
	}
	void TameableAnimal::write(char*& buffer) const {
		Animal::write(buffer);
		*(buffer++) = attributes;
		ownerUUID.write(buffer);
	}
	void cat::write(char*& buffer) const {
		TameableAnimal::write(buffer);
		type.write(buffer);
		*(buffer++) = isLying;
		*(buffer++) = isRelaxed;
		collarColor.write(buffer);
	}
	void wolf::write(char*& buffer) const {
		TameableAnimal::write(buffer);
		*(buffer++) = isBegging;
		collarColor.write(buffer);
		angerTime.write(buffer);
	}
	void parrot::write(char*& buffer) const {
		TameableAnimal::write(buffer);
		variant.write(buffer);
	}
	void AbstractVillager::write(char*& buffer) const {
		AgeableMob::write(buffer);
		headShakeTime.write(buffer);
	}
	void villager::write(char*& buffer) const {
		AbstractVillager::write(buffer);
		villagerTyper.write(buffer);
		villagerProfession.write(buffer);
		level.write(buffer);
	}
	void ironGolem::write(char*& buffer) const {
		AbstractGolem::write(buffer);
		*(buffer++) = attributes;
	}
	void snowGolem::write(char*& buffer) const {
		AbstractGolem::write(buffer);
		*(buffer++) = attributes;
	}
	void shulker::write(char*& buffer) const {
		AbstractGolem::write(buffer);
		varInt((int)theDirection).write(buffer);
		attachmentPosition.write(buffer);
		*(buffer++) = shieldHeight;
		*(buffer++) = color;
	}
	void BasePiglin::write(char*& buffer) const {
		Monster::write(buffer);
		*(buffer++) = isImmune;
	}
	void piglin::write(char*& buffer) const {
		BasePiglin::write(buffer);
		*(buffer++) = isBaby;
		*(buffer++) = isChargingCrossbow;
		*(buffer++) = isDancing;
	}
	void blaze::write(char*& buffer) const {
		Monster::write(buffer);
		*(buffer++) = attributes;
	}
	void creeper::write(char*& buffer) const {
		Monster::write(buffer);
		state.write(buffer);
		*(buffer++) = isCharged;
		*(buffer++) = isIgnited;
	}
	void guardian::write(char*& buffer) const {
		Monster::write(buffer);
		*(buffer++) = isRetractingSpikes;
		targetEid.write(buffer);
	}
	void raider::write(char*& buffer) const {
		Monster::write(buffer);
		*(buffer++) = isCelebrating;
	}
	void pillager::write(char*& buffer) const {
		AbstractIllager::write(buffer);
		*(buffer++) = isCharging;
	}
	void spellcasterIllager::write(char*& buffer) const {
		AbstractIllager::write(buffer);
		*(buffer++) = spell;
	}
	void witch::write(char*& buffer) const {
		raider::write(buffer);
		*(buffer++) = isDrinking;
	}
	void vex::write(char*& buffer) const {
		Monster::write(buffer);
		*(buffer++) = attributes;
	}
	void spider::write(char*& buffer) const {
		Monster::write(buffer);
		*(buffer++) = attributes;
	}
	void wither::write(char*& buffer) const {
		Monster::write(buffer);
		centerTargetEid.write(buffer);
		leftTargetEid.write(buffer);
		rightTargetEid.write(buffer);
		invulnerableTime.write(buffer);
	}
	void zoglin::write(char*& buffer) const {
		Monster::write(buffer);
		*(buffer++) = isBaby;
	}
	void zombie::write(char*& buffer) const {
		Monster::write(buffer);
		*(buffer++) = isBaby;
		unused.write(buffer);
		*(buffer++) = isBecomingDrowned;
	}
	void zombieVillager::write(char*& buffer) const {
		Monster::write(buffer);
		*(buffer++) = isConverting;
		villagerTyper.write(buffer);
		villagerProfession.write(buffer);
		level.write(buffer);
	}
	void enderman::write(char*& buffer) const {
		Monster::write(buffer);
		blockId.write(buffer);
		*(buffer++) = isScreaming;
		*(buffer++) = isStaring;
	}
	void enderDragon::write(char*& buffer) const {
		Mob::write(buffer);
		phase.write(buffer);
	}
	void ghast::write(char*& buffer) const {
		Flying::write(buffer);
		*(buffer++) = isAttacking;
	}
	void phantom::write(char*& buffer) const {
		Flying::write(buffer);
		size.write(buffer);
	}
	void slime::write(char*& buffer) const {
		Mob::write(buffer);
		size.write(buffer);
	}
	void AbstractMinecart::write(char*& buffer) const {
		entity::write(buffer);
		shakingPower.write(buffer);
		shakingDirection.write(buffer);
		shakingMultiplier.write(buffer);
		customBlockId.write(buffer);
		customBlockY.write(buffer);
		*(buffer++) = showCustomBlock;
	}
	void minecartFurnace::write(char*& buffer) const {
		AbstractMinecart::write(buffer);
		*(buffer++) = hasFuel;
	}
	void minecartCommandBlock::write(char*& buffer) const {
		AbstractMinecart::write(buffer);
		command.write(buffer);
		lastOutput.write(buffer);
	}
	void primedTNT::write(char*& buffer) const {
		entity::write(buffer);
		fuseTime.write(buffer);
	}
}
