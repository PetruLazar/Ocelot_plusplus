#pragma once
#include "mcString.h"
#include "varData.h"
#include "endian.h"
#include "position.h"
#include "uuid.h"
#include "chat.h"
#include "slot.h"
#include "position.h"
#include "particle.h"

class Rotation3f
{
public:
	bfloat x, y, z;

	Rotation3f(bfloat x, bfloat y, bfloat z) : x(x), y(y), z(z) {}

	void write(char*& buffer) const;
};

namespace eidDispenser
{
	enum class type
	{
		player = -2,
		entity = -1
	};

	class General
	{
		class Iterator
		{
		public:
			int eid;
			Iterator* next;

			Iterator(int eid, Iterator* next = 0);
		} *first;

		void clear(Iterator*);

	public:
		SERVER_API General(eidDispenser::type dispenserType);
		SERVER_API ~General();

		SERVER_API int Alloc();
		SERVER_API void Free(int);
	};

	class Player : public General
	{
	public:
		SERVER_API Player();
	};
	class Entity : public General
	{
	public:
		SERVER_API Entity();
	};
}

namespace Entity {
	enum class type
	{
		minecraft_area_effect_cloud = 0,
		minecraft_armor_stand = 1,
		minecraft_arrow = 2,
		minecraft_axolotl = 3,
		minecraft_bat = 4,
		minecraft_bee = 5,
		minecraft_blaze = 6,
		minecraft_boat = 7,
		minecraft_cat = 8,
		minecraft_cave_spider = 9,
		minecraft_chicken = 10,
		minecraft_cod = 11,
		minecraft_cow = 12,
		minecraft_creeper = 13,
		minecraft_dolphin = 14,
		minecraft_donkey = 15,
		minecraft_dragon_fireball = 16,
		minecraft_drowned = 17,
		minecraft_elder_guardian = 18,
		minecraft_end_crystal = 19,
		minecraft_ender_dragon = 20,
		minecraft_enderman = 21,
		minecraft_endermite = 22,
		minecraft_evoker = 23,
		minecraft_evoker_fangs = 24,
		minecraft_experience_orb = 25,
		minecraft_eye_of_ender = 26,
		minecraft_falling_block = 27,
		minecraft_firework_rocket = 28,
		minecraft_fox = 29,
		minecraft_ghast = 30,
		minecraft_giant = 31,
		minecraft_glow_item_frame = 32,
		minecraft_glow_squid = 33,
		minecraft_goat = 34,
		minecraft_guardian = 35,
		minecraft_hoglin = 36,
		minecraft_horse = 37,
		minecraft_husk = 38,
		minecraft_illusioner = 39,
		minecraft_iron_golem = 40,
		minecraft_item = 41,
		minecraft_item_frame = 42,
		minecraft_fireball = 43,
		minecraft_leash_knot = 44,
		minecraft_lightning_bolt = 45,
		minecraft_llama = 46,
		minecraft_llama_spit = 47,
		minecraft_magma_cube = 48,
		minecraft_marker = 49,
		minecraft_minecart = 50,
		minecraft_chest_minecart = 51,
		minecraft_command_block_minecart = 52,
		minecraft_furnace_minecart = 53,
		minecraft_hopper_minecart = 54,
		minecraft_spawner_minecart = 55,
		minecraft_tnt_minecart = 56,
		minecraft_mule = 57,
		minecraft_mooshroom = 58,
		minecraft_ocelot = 59,
		minecraft_painting = 60,
		minecraft_panda = 61,
		minecraft_parrot = 62,
		minecraft_phantom = 63,
		minecraft_pig = 64,
		minecraft_piglin = 65,
		minecraft_piglin_brute = 66,
		minecraft_pillager = 67,
		minecraft_polar_bear = 68,
		minecraft_tnt = 69,
		minecraft_pufferfish = 70,
		minecraft_rabbit = 71,
		minecraft_ravager = 72,
		minecraft_salmon = 73,
		minecraft_sheep = 74,
		minecraft_shulker = 75,
		minecraft_shulker_bullet = 76,
		minecraft_silverfish = 77,
		minecraft_skeleton = 78,
		minecraft_skeleton_horse = 79,
		minecraft_slime = 80,
		minecraft_small_fireball = 81,
		minecraft_snow_golem = 82,
		minecraft_snowball = 83,
		minecraft_spectral_arrow = 84,
		minecraft_spider = 85,
		minecraft_squid = 86,
		minecraft_stray = 87,
		minecraft_strider = 88,
		minecraft_egg = 89,
		minecraft_ender_pearl = 90,
		minecraft_experience_bottle = 91,
		minecraft_potion = 92,
		minecraft_trident = 93,
		minecraft_trader_llama = 94,
		minecraft_tropical_fish = 95,
		minecraft_turtle = 96,
		minecraft_vex = 97,
		minecraft_villager = 98,
		minecraft_vindicator = 99,
		minecraft_wandering_trader = 100,
		minecraft_witch = 101,
		minecraft_wither = 102,
		minecraft_wither_skeleton = 103,
		minecraft_wither_skull = 104,
		minecraft_wolf = 105,
		minecraft_zoglin = 106,
		minecraft_zombie = 107,
		minecraft_zombie_horse = 108,
		minecraft_zombie_villager = 109,
		minecraft_zombified_piglin = 110,
		minecraft_player = 111,
		minecraft_fishing_bobber = 112
	};

	enum class direction
	{
		south,
		west,
		north,
		east
	};

	enum class animation : Byte
	{
		swingMainArm,
		takeDamage,
		leaveBed,
		swingOffhand,
		criticalEffect,
		magicCriticalEffect
	};
	
	enum class pose
	{
		standing = 0,
		fall_flying, 
		sleeping, 
		swimming, 
		spin_attack, 
		sneaking, 
		long_jumping, 
		dying
	};

	namespace Painting
	{
		enum motive
		{
			kebab,
			aztec,
			alban,
			aztec2,
			bomb,
			plant,
			wasteland,
			pool,
			courbet,
			sea,
			sunset,
			creebet,
			wanderer,
			graham,
			match,
			bust,
			stage,
			void_,
			skull_and_roses,
			wither,
			fighters,
			pointer,
			pigscene,
			burning_skull,
			skeleton,
			donkey_kong
		};
	}

	namespace Sculk
	{
		enum destinationType
		{
			block,
			entity
		};
		union destination
		{
			varInt eid;
			Position position;
		};
	}

	class Modifier
	{
	public:
		mcUUID uuid;
		bdouble amount;
		Byte operation;

		Modifier(mcUUID uuid, bdouble amount, Byte operation) : uuid(uuid), amount(amount), operation(operation) {}

		void write(char*& buffer) const;
	};

	class Property
	{
	public:
		mcString key;
		bdouble value;
		varInt nOfModifiers = 0;
		Modifier* modifiers;

		SERVER_API Property(const mcString& key, double value, varInt nOfModifiers, Modifier* modifiers)
			: key(key), value(value), nOfModifiers(nOfModifiers), modifiers(modifiers) { }
		SERVER_API Property(const mcString& key, double value, Modifier modifier)
			: key(key), value(value), nOfModifiers(1), modifiers(&modifier) { }

		void write(char*& buffer) const;
	};

	class Metadata
	{
	public:
		Byte index;
		varInt type;
		void* value;
		Byte state = 2;

		Metadata(Byte index, varInt type, void* value) : index(index), type(type), value(value) {}
		Metadata(Byte index, varInt type, bool present, void* value = nullptr) : index(index), type(type), value(value), state(present) {}

		void write(char*& buffer) const;
	};

	struct entity
	{
	protected:
		varInt eid;

		eidDispenser::General *eidDispenser;

	public:
		mcUUID* euuid;

		Byte attributes;
		varInt airTicks;
		Chat *customName;
		bool isCustomNameVisible;
		bool isSilent;
		bool hasGravity;
		pose thePose;
		varInt ticksFrozen;

		entity(eidDispenser::General *eidDispenser, Byte attributes = 0, varInt airTicks = 300, Chat *customName = nullptr, bool isCustomNameVisible = false,
			bool isSilent = false, bool hasGravity = false, pose thePose = pose::standing, varInt ticksFrozen = 0);
		entity(const entity& e);

		~entity();

		varInt getEid();
	
		void write(char*& buffer) const;
	};

	struct thrownEgg : entity
	{
		Slot item;

		thrownEgg(entity theEntity, Slot item = Slot()) : entity(theEntity) {}
	};

	struct thrownEnderPearl : entity
	{
		Slot item;

		thrownEnderPearl(entity theEntity, Slot item = Slot()) : entity(theEntity) {}
	};

	struct thrownExperienceBottle : entity
	{
		Slot item;

		thrownExperienceBottle(entity theEntity, Slot item = Slot()) : entity(theEntity) {}
	};

	struct thrownPotion : entity
	{
		Slot item;

		thrownPotion(entity theEntity, Slot item) : entity(theEntity) {}
	};

	struct snowball : entity
	{
		Slot item;

		snowball(entity theEntity, Slot item = Slot()) : entity(theEntity) {}
	};

	struct eyeOfEnder : entity
	{
		Slot item;

		eyeOfEnder(entity theEntity, Slot item = Slot()) : entity(theEntity) {}
	};

	struct fallingBlock : entity
	{
		Position position;

		fallingBlock(entity theEntity, Position position = Position(0, 0, 0)) : position(position), entity(theEntity) {}
	};

	struct areaEffectCloud : entity
	{
		bfloat radius;
		varInt color;
		bool ignoreRadius;
		Particle* theParticle;

		areaEffectCloud(entity theEntity, bfloat radius = 0.5, varInt color = 0, bool ignoreRadius = false, Particle* theParticle = nullptr)
			: radius(radius), color(color), ignoreRadius(ignoreRadius), theParticle(theParticle), entity(theEntity) {}  //CHANGE PARTICLE FROM NULLPTR TO PARTICLE::EFFECT
	};

	struct fishingHook : entity
	{
		varInt hookedEntity;
		bool isCatchable;

		fishingHook(entity theEntity, varInt hookedEntity = 0, bool isCatchable = false)
			: hookedEntity(hookedEntity), isCatchable(isCatchable), entity(theEntity) {}
	};

	struct AbstractArrow : entity
	{
		Byte bitMask;
		Byte piercingLevel;

		AbstractArrow(entity theEntity, Byte bitMask = 0, Byte piercingLevel = 0)
			: bitMask(bitMask), piercingLevel(piercingLevel), entity(theEntity) {}
	};

	struct arrow : AbstractArrow
	{
		varInt color;

		arrow(AbstractArrow theAbstractArrow, varInt color = -1) : color(color), AbstractArrow(theAbstractArrow) {}
	};

	struct spectralArrow : arrow
	{
		spectralArrow(arrow theArrow) : arrow(theArrow) {}
	};

	struct thrownTrident : AbstractArrow
	{
		varInt loyaltyLevel;
		bool hasEnchantmentGlint;

		thrownTrident(AbstractArrow theAbstractArrow, varInt loyaltyLevel = 0, bool hasEnchantmentGlint = false) : loyaltyLevel(loyaltyLevel), hasEnchantmentGlint(hasEnchantmentGlint), AbstractArrow(theAbstractArrow) {}
	};

	struct boat : entity
	{
		varInt timeSinceLastHit, forwardDirection, type, splashTimer;
		bfloat damageTaken;
		bool isLeftPaddleTurning, isRightPaddleTurning;

		boat(entity theEntity, varInt timeSinceLastHit = 0, varInt forwardDirection = 1, bfloat damageTaken = 0, varInt type = 0, bool isLeftPaddleTurning = false, bool isRightPaddleTurning = false, varInt splashTimer = 0)
			: timeSinceLastHit(timeSinceLastHit), forwardDirection(forwardDirection), damageTaken(damageTaken), type(type), isLeftPaddleTurning(isLeftPaddleTurning), isRightPaddleTurning(isRightPaddleTurning), splashTimer(splashTimer), entity(theEntity) {}
	};

	struct endCrystal : entity
	{
		Position beamTarget;
		bool showBottom;

		endCrystal(entity theEntity, Position beamTarget = Position(), bool showBottom = true) : beamTarget(beamTarget), showBottom(showBottom), entity(theEntity) {}
	};

	struct dragonFireball : entity
	{
		dragonFireball(entity theEntity) : entity(theEntity) {}
	};

	struct smallFireball : entity
	{
		Slot item;

		smallFireball(entity theEntity, Slot item = Slot()) : item(item), entity(theEntity) {}
	};

	struct fireball : entity
	{
		Slot item;

		fireball(entity theEntity, Slot item = Slot()) : item(item), entity(theEntity) {}
	};

	struct witherSkull : entity
	{
		bool isInvulnerable;

		witherSkull(entity theEntity, bool isInvulnerable = false) : isInvulnerable(isInvulnerable), entity(theEntity) {}
	};

	struct firework : entity
	{
		Slot fireworkInfo;
		varInt entityID;
		bool isShotAtAngle;

		firework(entity theEntity, Slot fireworkInfo, varInt entityID, bool isShotAtAngle = false)
			: fireworkInfo(fireworkInfo), entityID(entityID), isShotAtAngle(isShotAtAngle), entity(theEntity) {}
	};

	struct itemFrame : entity
	{
		Slot theItem;
		varInt rotation;

		itemFrame(entity theEntity, Slot theItem = Slot(), varInt rotation = 0) : theItem(theItem), rotation(rotation), entity(theEntity) {}
	};

	struct item : entity
	{
		Slot theItem;

		item(entity theEntity, Slot theItem = Slot()) : theItem(theItem), entity(theEntity) {}
	};

	struct LivingEntity : public entity
	{
		Byte handState;
		bfloat health;
		varInt potionEffectColor, nOfArrowsInEntity, nOfBeeStingersInEntity;
		bool isPotionEffectAmbient;
		Position locationOfBed;

		LivingEntity(const entity& theEntity, Byte handState = 0, bfloat health = 1, varInt potionEffectColor = 0, bool isPotionEffectAmbient = false, varInt nOfArrowsInEntity = 0, varInt nOfBeeStingersInEntity = 0, Position locationOfBed = Position())
			: handState(handState), health(health), potionEffectColor(potionEffectColor), isPotionEffectAmbient(isPotionEffectAmbient), nOfArrowsInEntity(nOfArrowsInEntity), nOfBeeStingersInEntity(nOfBeeStingersInEntity), locationOfBed(locationOfBed), entity(theEntity) {}

		using entity::entity;
	};

	struct player : public LivingEntity
	{
	public:
		bfloat additionalHearts;
		varInt score;
		Byte displayedSkinParts, mainHand;
		nbt* leftShoulderEntityData;
		nbt* rightShoulderEntityData;

		player(const LivingEntity& theLivingEntity, bfloat additionalHearts = 0.0, varInt score = 0, Byte displayedSkinParts = 0, Byte mainHand = 1, nbt* leftShoulderEntityData = nullptr, nbt* rightShoulderEntityData = nullptr)
			: additionalHearts(additionalHearts), score(score), displayedSkinParts(displayedSkinParts), mainHand(mainHand), leftShoulderEntityData(leftShoulderEntityData), rightShoulderEntityData(rightShoulderEntityData), LivingEntity(theLivingEntity) {}

		using LivingEntity::LivingEntity;
	};

	struct armorStand : LivingEntity
	{
		Byte attributes;
		Rotation3f head, body, leftArm, rightArm, leftLeg, rightLeg;

		armorStand(LivingEntity theLivingEntity, Byte attributes = 0, Rotation3f head = Rotation3f(0, 0, 0), Rotation3f body = Rotation3f(0, 0, 0), Rotation3f leftArm = Rotation3f(-10, 0, -10), Rotation3f rightArm = Rotation3f(-15, 0, 10), Rotation3f leftLeg = Rotation3f(-1, 0, -1), Rotation3f rightLeg = Rotation3f(1, 0, 1))
			: attributes(attributes), head(head), body(body), leftArm(leftArm), rightArm(rightArm), leftLeg(leftLeg), rightLeg(rightLeg), LivingEntity(theLivingEntity) {}
	};

	struct Mob : LivingEntity
	{
		Byte attributes;

		Mob(LivingEntity theLivingEntity, Byte attributes = 0) : attributes(attributes), LivingEntity(theLivingEntity) {}
	};

	struct AmbientCreature : Mob
	{
		AmbientCreature(Mob theMob) : Mob(theMob) {}
	};

	struct bat : AmbientCreature
	{
		Byte isHanging;

		bat(AmbientCreature theAmbientCreature, Byte isHanging) : isHanging(isHanging), AmbientCreature(theAmbientCreature) {}
	};

	struct PathfinderMob : Mob
	{
		PathfinderMob(Mob theMob) : Mob(theMob) {}
	};

	struct WaterAnimal : PathfinderMob
	{
		WaterAnimal(PathfinderMob thePathfinderMob) : PathfinderMob(thePathfinderMob) {}
	};

	struct squid : WaterAnimal
	{
		squid(WaterAnimal theWaterAnimal) : WaterAnimal(theWaterAnimal) {}
	};

	struct dolphin : WaterAnimal
	{
		Position treasurePosition;
		bool canFindTreasure, hasFish;

		dolphin(WaterAnimal theWaterAnimal, Position treasurePosition = Position(0, 0, 0), bool canFindTreasure = false, bool hasFish = false)
			: treasurePosition(treasurePosition), canFindTreasure(canFindTreasure), hasFish(hasFish), WaterAnimal(theWaterAnimal) {}
	};

	struct AbstractFish : WaterAnimal
	{
		bool fromBucket;

		AbstractFish(WaterAnimal theWaterAnimal, bool fromBucket = false) : fromBucket(fromBucket), WaterAnimal(theWaterAnimal) {}
	};

	struct cod : AbstractFish
	{
		cod(AbstractFish theAbstractFish) : AbstractFish(theAbstractFish) {}
	};

	struct pufferFish : AbstractFish
	{
		varInt puffState;

		pufferFish(AbstractFish theAbstractFish, varInt puffState = 0) : puffState(puffState), AbstractFish(theAbstractFish) {}
	};

	struct salmon : AbstractFish
	{
		salmon(AbstractFish theAbstractFish) : AbstractFish(theAbstractFish) {}
	};

	struct tropicalFish : AbstractFish
	{
		varInt variant;

		tropicalFish(AbstractFish theAbstractFish, varInt variant = 0) : variant(variant), AbstractFish(theAbstractFish) {}
	};

	struct AgeableMob : PathfinderMob
	{
		bool isBaby;

		AgeableMob(PathfinderMob thePathfinderMob, bool isBaby = false) : isBaby(isBaby), PathfinderMob(thePathfinderMob) {}
	};

	struct Animal : AgeableMob
	{
		Animal(AgeableMob theAgeableMob) : AgeableMob(theAgeableMob) {}
	};

	struct AbstractHorse : Animal
	{
		Byte attributes;
		mcUUID owner;

		AbstractHorse(Animal theAnimal, Byte attributes = 0, mcUUID owner = mcUUID(0,0,0,0)) : attributes(attributes), owner(owner), Animal(theAnimal) {}
	}; //UUID MUST BE ABSENT

	struct horse : AbstractHorse
	{
		varInt variant;

		horse(AbstractHorse theAbstractHorse, varInt variant = 0) : variant(variant), AbstractHorse(theAbstractHorse) {}
	};

	struct zombieHorse : AbstractHorse
	{
		zombieHorse(AbstractHorse theAbstractHorse) : AbstractHorse(theAbstractHorse) {}
	};

	struct skeletonHorse : AbstractHorse
	{
		skeletonHorse(AbstractHorse theAbstractHorse) : AbstractHorse(theAbstractHorse) {}
	};

	struct chestedHorse : AbstractHorse
	{
		bool hasChest;

		chestedHorse(AbstractHorse theAbstractHorse, bool hasChest = false) : hasChest(hasChest), AbstractHorse(theAbstractHorse) {}
	};

	struct donkey : chestedHorse
	{
		donkey(chestedHorse theChestedHorse) : chestedHorse(theChestedHorse) {}
	};

	struct llama : chestedHorse
	{
		varInt strength, carpetColor, variant;

		llama(chestedHorse theChestedHorse, varInt strength = 0, varInt carpetColor = -1, varInt variant = 0)
			: strength(strength), carpetColor(carpetColor), variant(variant), chestedHorse(theChestedHorse) {}
	};

	struct traderLlama
	{
		traderLlama() {}
	}; //wtf?

	struct mule : chestedHorse
	{
		mule(chestedHorse theChestedHorse) : chestedHorse(theChestedHorse) {}
	};

	struct bee : Animal
	{
		Byte attributes;
		varInt angerTimeTicks;

		bee(Animal theAnimal, Byte attributes = 0, varInt angerTimeTicks = 0) : attributes(attributes), angerTimeTicks(angerTimeTicks), Animal(theAnimal) {}
	};

	struct fox : Animal
	{
		varInt type;
		Byte attributes;
		mcUUID firstUUID, secondUUID;

		fox(Animal theAnimal, varInt type = 0, Byte attributes = 0, mcUUID firstUUID = mcUUID(0,0,0,0), mcUUID secondUUID = mcUUID(0, 0, 0, 0))
			: type(type), attributes(attributes), firstUUID(firstUUID), secondUUID(secondUUID), Animal(theAnimal) {}
	}; //UUIDs MUST BE ABSENT

	struct ocelot : Animal
	{
		bool isTrusting;

		ocelot(Animal theAnimal, bool isTrusting = false) : isTrusting(isTrusting), Animal(theAnimal) {}
	};

	struct panda : Animal
	{
		bint breedTimer, sneezeTimer, eatTimer;
		Byte mainGene, hiddenGene, attributes;

		panda(Animal theAnimal, bint breedTimer = 0, bint sneezeTimer = 0, bint eatTimer = 0, Byte mainGene = 0, Byte hiddenGene = 0, Byte attributes = 0)
			: breedTimer(breedTimer), sneezeTimer(sneezeTimer), eatTimer(eatTimer), mainGene(mainGene), hiddenGene(hiddenGene), attributes(attributes), Animal(theAnimal) {}
	};

	struct pig : Animal
	{
		bool hasSaddle;
		varInt boostCarrotTime;

		pig(Animal theAnimal, bool hasSaddle = false, varInt boostCarrotTime = 0)
			: hasSaddle(hasSaddle), boostCarrotTime(boostCarrotTime), Animal(theAnimal) {}
	};

	struct rabbit : Animal
	{
		varInt type;

		rabbit(Animal theAnimal, varInt type = 0) : type(type), Animal(theAnimal) {}
	};

	struct turtle : Animal
	{
		Position homePosition, travelPosition;
		bool hasEggs, isLayingEggs, isGoingHome, isTravelling;

		turtle(Animal theAnimal, Position homePosition = Position(0,0,0), bool hasEggs = false, bool isLayingEggs = false, Position travelPosition = Position(0, 0, 0), bool isGoingHome = false, bool isTravelling = false)
			: homePosition(homePosition), hasEggs(hasEggs), isLayingEggs(isLayingEggs), travelPosition(travelPosition), isGoingHome(isGoingHome), isTravelling(isTravelling), Animal(theAnimal) {}
	};

	struct polarBear : Animal
	{
		bool isStandingUp;

		polarBear(Animal theAnimal, bool isStandingUp = false) : isStandingUp(isStandingUp), Animal(theAnimal) {}
	};

	struct chicken : Animal
	{
		chicken(Animal theAnimal) : Animal(theAnimal) {}
	};

	struct cow : Animal
	{
		cow(Animal theAnimal) : Animal(theAnimal) {}
	};

	struct hoglin : Animal
	{
		bool isImmune;

		hoglin(Animal theAnimal, bool isImmune = false) : isImmune(isImmune), Animal(theAnimal) {}
	};

	struct mooshroom : cow
	{
		mcString variant;

		mooshroom(cow theCow, mcString variant = "red") : variant(variant), cow(theCow) {}
	};

	struct sheep : Animal
	{
		Byte attributes;

		sheep(Animal theAnimal, Byte attributes = 0) : attributes(attributes), Animal(theAnimal) {}
	};

	struct strider : Animal
	{
		varInt boostTime;
		bool isShaking, hasSaddle;

		strider(Animal theAnimal, varInt boostTime = 0, bool isShaking = false, bool hasSaddle = false)
			: boostTime(boostTime), isShaking(isShaking), hasSaddle(hasSaddle), Animal(theAnimal) {}
	};

	struct TameableAnimal : Animal
	{
		Byte attributes;
		mcUUID ownerUUID;

		TameableAnimal(Animal theAnimal, Byte attributes = 0, mcUUID ownerUUID = mcUUID(0,0,0,0))
			: attributes(attributes), ownerUUID(ownerUUID), Animal(theAnimal) {}
	}; //MCUUID MUST BE ABSENT

	struct cat : TameableAnimal
	{
		varInt type, collarColor;
		bool isLying, isRelaxed;

		cat(TameableAnimal theTameableAnimal, varInt type = 1, bool isLying = false, bool isRelaxed = false, varInt collarColor = 14)
			: type(type), isLying(isLying), isRelaxed(isRelaxed), collarColor(collarColor), TameableAnimal(theTameableAnimal) {}
	};

	struct wolf : TameableAnimal
	{
		bool isBegging;
		varInt collarColor, angerTime;

		wolf(TameableAnimal theTameableAnimal, bool isBegging = false, varInt collarColor = 14, varInt angerTime = 0)
			: isBegging(isBegging), collarColor(collarColor), angerTime(angerTime), TameableAnimal(theTameableAnimal) {}
	};

	struct parrot : TameableAnimal
	{
		varInt variant;

		parrot(TameableAnimal theTameableAnimal, varInt variant = 0) : variant(variant), TameableAnimal(theTameableAnimal) {}
	};

	struct AbstractVillager : AgeableMob
	{
		varInt headShakeTime;

		AbstractVillager(AgeableMob theAgeableMob, varInt headShakeTime = 0) : headShakeTime(headShakeTime), AgeableMob(theAgeableMob) {}
	};

	struct villager : AbstractVillager
	{
		varInt villagerTyper, villagerProfession, level;

		villager(AbstractVillager theAbstractVillager, varInt villagerTyper = 0, varInt villagerProfession = 0, varInt level = 1)
			: villagerTyper(villagerTyper), AbstractVillager(theAbstractVillager) {}
	}; //redo this

	struct wanderingTrader : AbstractVillager
	{
		wanderingTrader(AbstractVillager theAbstractVillager) : AbstractVillager(theAbstractVillager) {}
	};

	struct AbstractGolem : PathfinderMob
	{
		AbstractGolem(PathfinderMob thePathfinderMob) : PathfinderMob(thePathfinderMob) {}
	};

	struct ironGolem : AbstractGolem
	{
		Byte attributes;

		ironGolem(AbstractGolem theAbstractGolem, Byte attributes = 0) : attributes(attributes), AbstractGolem(theAbstractGolem) {}
	};

	struct snowGolem : AbstractGolem
	{
		Byte attributes;

		snowGolem(AbstractGolem theAbstractGolem, Byte attributes = 0) : attributes(attributes), AbstractGolem(theAbstractGolem) {}
	};

	struct shulker : AbstractGolem
	{
		direction theDirection;
		Position attachmentPosition;
		Byte shieldHeight, color;

		shulker(AbstractGolem theAbstractGolem, direction theDirection = direction::south, Position attachmentPosition = Position(0, 0, 0), Byte shieldHeight = 0, Byte color = 10)
			: theDirection(theDirection), attachmentPosition(attachmentPosition), shieldHeight(shieldHeight), color(color), AbstractGolem(theAbstractGolem) {}
	}; //attachmentPosition must be absent

	struct Monster : PathfinderMob
	{
		Monster(PathfinderMob thePathfinderMob) : PathfinderMob(thePathfinderMob) {}
	};

	struct BasePiglin : Monster
	{
		bool isImmune;

		BasePiglin(Monster theMonster, bool isImmune = false)
			: isImmune(isImmune), Monster(theMonster) {}
	};

	struct piglin : BasePiglin
	{
		bool isBaby, isChargingCrossbow, isDancing;

		piglin(BasePiglin theBasePiglin, bool isBaby = false, bool isChargingCrossbow = false, bool isDancing = false)
			: isBaby(isBaby), isChargingCrossbow(isChargingCrossbow), isDancing(isDancing), BasePiglin(theBasePiglin) {}
	};

	struct piglinBrute : BasePiglin
	{
		piglinBrute(BasePiglin theBasePiglin) : BasePiglin(theBasePiglin) {}
	};

	struct blaze : Monster
	{
		Byte attributes;

		blaze(Monster theMonster, Byte attributes = 0) : attributes(attributes), Monster(theMonster) {}
	};

	struct creeper : Monster
	{
		varInt state;
		bool isCharged, isIgnited;

		creeper(Monster theMonster, varInt state = -1, bool isCharged = false, bool isIgnited = false)
			: state(state), isCharged(isCharged), isIgnited(isIgnited), Monster(theMonster) {}
	};

	struct endermite : Monster
	{
		endermite(Monster theMonster) : Monster(theMonster) {}
	};

	struct giant : Monster
	{
		giant(Monster theMonster) : Monster(theMonster) {}
	};

	struct guardian : Monster
	{
		bool isRetractingSpikes;
		varInt targetEid;

		guardian(Monster theMonster, bool isRetractingSpikes = false, varInt targetEid = 0)
			: isRetractingSpikes(isRetractingSpikes), targetEid(targetEid), Monster(theMonster) {}
	};

	struct elderGuardian : guardian
	{
		elderGuardian(guardian theguardian) : guardian(theguardian) {}
	};

	struct silverfish : Monster
	{
		silverfish(Monster theMonster) : Monster(theMonster) {}
	};

	struct raider : Monster
	{
		bool isCelebrating;

		raider(Monster theMonster, bool isCelebrating = false) : isCelebrating(isCelebrating), Monster(theMonster) {}
	};

	struct AbstractIllager : raider
	{
		AbstractIllager(raider theRaider) : raider(theRaider) {}
	};

	struct vindicator : AbstractIllager
	{
		vindicator(AbstractIllager theAbstractIllager) : AbstractIllager(theAbstractIllager) {}
	};

	struct pillager : AbstractIllager
	{
		bool isCharging;

		pillager(AbstractIllager theAbstractIllager, bool isCharging = false) : isCharging(isCharging), AbstractIllager(theAbstractIllager) {}
	};

	struct spellcasterIllager : AbstractIllager
	{
		Byte spell;

		spellcasterIllager(AbstractIllager theAbstractIllager, Byte spell = 0) : spell(spell), AbstractIllager(theAbstractIllager) {}
	};

	struct evoker : AbstractIllager
	{
		evoker(AbstractIllager theAbstractIllager) : AbstractIllager(theAbstractIllager) {}
	};

	struct illusioner : AbstractIllager
	{
		illusioner(AbstractIllager theAbstractIllager) : AbstractIllager(theAbstractIllager) {}
	};

	struct ravager : raider
	{
		ravager(raider theRaider) : raider(theRaider) {}
	};

	struct witch : raider
	{
		bool isDrinking;

		witch(raider theRaider, bool isDrinking = false) : isDrinking(isDrinking), raider(theRaider) {}
	};

	struct evokerFangs : entity
	{
		evokerFangs(entity theEntity) : entity(theEntity) {}
	};

	struct vex : Monster
	{
		Byte attributes;

		vex(Monster theMonster, Byte attributes = 0) : attributes(attributes), Monster(theMonster) {}
	};

	struct AbstractSkeleton : Monster
	{
		AbstractSkeleton(Monster theMonster) : Monster(theMonster) {}
	};

	struct skeleton : AbstractSkeleton
	{
		skeleton(AbstractSkeleton theAbstractSkeleton) : AbstractSkeleton(theAbstractSkeleton) {}
	};

	struct witherSkeleton : AbstractSkeleton
	{
		witherSkeleton(AbstractSkeleton theAbstractSkeleton) : AbstractSkeleton(theAbstractSkeleton) {}
	};

	struct stray : AbstractSkeleton
	{
		stray(AbstractSkeleton theAbstractSkeleton) : AbstractSkeleton(theAbstractSkeleton) {}
	};

	struct spider : Monster
	{
		Byte attributes;

		spider(Monster theMonster, Byte attributes = 0) : attributes(attributes), Monster(theMonster) {}
	};

	struct wither : Monster
	{
		varInt centerTargetEid, leftTargetEid, rightTargetEid, invulnerableTime;

		wither(Monster theMonster, varInt centerTargetEid = 0, varInt leftTargetEid = 0, varInt rightTargetEid = 0, varInt invulnerableTime = 0)
			: centerTargetEid(centerTargetEid), leftTargetEid(leftTargetEid), rightTargetEid(rightTargetEid), invulnerableTime(invulnerableTime), Monster(theMonster) {}
	};

	struct zoglin : Monster
	{
		bool isBaby;

		zoglin(Monster theMonster, bool isBaby = 0) : isBaby(isBaby), Monster(theMonster) {}
	};

	struct zombie : Monster
	{
		bool isBaby, isBecomingDrowned;
		varInt unused;

		zombie(Monster theMonster, bool isBaby = 0, varInt unused = 0, bool isBecomingDrowned = false) :
			isBaby(isBaby), unused(unused), isBecomingDrowned(isBecomingDrowned), Monster(theMonster) {}
	};

	struct zombieVillager : zombie
	{
		bool isConverting;
		varInt villagerTyper, villagerProfession, level;

		zombieVillager(zombie theZombie, bool isConverting = false, varInt villagerTyper = 0, varInt villagerProfession = 0, varInt level = 1)
			: isConverting(isConverting), villagerTyper(villagerTyper), villagerProfession(villagerProfession), level(level), zombie(theZombie) {}
	}; //redo this

	struct husk : zombie
	{
		husk(zombie theZombie) : zombie(theZombie) {}
	};

	struct drowned : zombie
	{
		drowned(zombie theZombie) : zombie(theZombie) {}
	};

	struct zombifiedPiglin : zombie
	{
		zombifiedPiglin(zombie theZombie) : zombie(theZombie) {}
	};

	struct enderman : Monster
	{
		varInt blockId;
		bool isScreaming, isStaring;

		enderman(Monster theMonster, varInt blockId = 0, bool isScreaming = false, bool isStaring = false)
			: blockId(blockId), isScreaming(isScreaming), isStaring(isStaring), Monster(theMonster) {}
	}; //blockId must be absent

	struct enderDragon : Mob
	{
		varInt phase;

		enderDragon(Mob theMob, varInt phase = 10) : phase(phase), Mob(theMob) {}
	};

	struct Flying : Mob
	{
		Flying(Mob theMob) : Mob(theMob) {}
	};

	struct ghast : Flying
	{
		bool isAttacking;

		ghast(Flying theFlying, bool isAttacking = false) : isAttacking(isAttacking), Flying(theFlying) {}
	};

	struct phantom : Flying
	{
		varInt size;

		phantom(Flying theFlying, varInt size = 0) : size(size), Flying(theFlying) {}
	};

	struct slime : Mob
	{
		varInt size;

		slime(Mob theMob, varInt size = 1) : size(size), Mob(theMob) {}
	};

	struct llamaSpit : entity
	{
		llamaSpit(entity theEntity) : entity(theEntity) {}
	};

	struct AbstractMinecart : entity
	{
		varInt shakingPower, shakingDirection, customBlockId, customBlockY;
		bfloat shakingMultiplier;
		bool showCustomBlock;

		AbstractMinecart(entity theEntity, varInt shakingPower = 0, varInt shakingDirection = 1, bfloat shakingMultiplier = 0, varInt customBlockId = 0, varInt customBlockY = 6, bool showCustomBlock = false)
			: shakingPower(shakingPower), shakingDirection(shakingDirection), shakingMultiplier(shakingMultiplier), customBlockId(customBlockId), customBlockY(customBlockY), showCustomBlock(showCustomBlock), entity(theEntity) {}
	};

	struct minecart : AbstractMinecart
	{
		minecart(AbstractMinecart theAbstractMinecart) : AbstractMinecart(theAbstractMinecart) {}
	};

	struct AbstractMinecartContainer : AbstractMinecart
	{
		AbstractMinecartContainer(AbstractMinecart theAbstractMinecart) : AbstractMinecart(theAbstractMinecart) {}
	};

	struct minecartHopper : AbstractMinecartContainer
	{
		minecartHopper(AbstractMinecart theAbstractMinecart, AbstractMinecartContainer theAbstractMinecartContainer) : AbstractMinecartContainer(theAbstractMinecartContainer) {}
	};

	struct minecartChest : AbstractMinecartContainer
	{
		minecartChest(AbstractMinecart theAbstractMinecart, AbstractMinecartContainer theAbstractMinecartContainer) : AbstractMinecartContainer(theAbstractMinecartContainer) {}
	};

	struct minecartFurnace : AbstractMinecart
	{
		bool hasFuel;

		minecartFurnace(AbstractMinecart theAbstractMinecart, bool hasFuel = false) : hasFuel(hasFuel), AbstractMinecart(theAbstractMinecart) {}
	};

	struct minecartTNT : AbstractMinecart
	{
		minecartTNT(AbstractMinecart theAbstractMinecart) : AbstractMinecart(theAbstractMinecart) {}
	};

	struct minecartSpawner : AbstractMinecart
	{
		minecartSpawner(AbstractMinecart theAbstractMinecart) : AbstractMinecart(theAbstractMinecart) {}
	};

	struct minecartCommandBlock : AbstractMinecart
	{
		mcString command;
		Chat &lastOutput;

		minecartCommandBlock(AbstractMinecart theAbstractMinecart, mcString command = "", Chat lastOutput = Chat(""))
			: command(command), lastOutput(lastOutput), AbstractMinecart(theAbstractMinecart) {}
	};

	struct primedTNT : entity
	{
		varInt fuseTime;

		primedTNT(entity theEntity, varInt fuseTime = 80) : fuseTime(fuseTime), entity(theEntity) {}
	};
}
