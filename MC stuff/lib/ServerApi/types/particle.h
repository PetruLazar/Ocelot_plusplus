#pragma once
#include "varData.h"
#include "endian.h"
#include "slot.h"

namespace particle
{
	enum type {
		ambient_entity_effect = 0,
		angry_villager,
		barrier,
		light,
		block,
		bubble,
		cloud,
		crit,
		damage_indicator,
		dragon_breath,
		dripping_lava,
		falling_lava,
		landing_lava,
		dripping_water,
		falling_water,
		dust,
		dust_color_transition,
		effect,
		elder_guardian,
		enchanted_hit,
		enchant,
		end_rod,
		entity_effect,
		explosion_emitter,
		explosion,
		falling_dust,
		firework,
		fishing,
		flame,
		soul_fire_flame,
		soul,
		flash,
		happy_villager,
		composter,
		heart,
		instant_effect,
		item,
		vibration,
		item_slime,
		item_snowball,
		large_smoke,
		lava,
		mycelium,
		note,
		poof,
		portal,
		rain,
		smoke,
		sneeze,
		spit,
		squid_ink,
		sweep_attack,
		totem_of_undying,
		underwater,
		splash,
		witch,
		bubble_pop,
		current_down,
		bubble_column_up,
		nautilus,
		dolphin,
		campfire_cosy_smoke,
		campfire_signal_smoke,
		dripping_honey,
		falling_honey,
		landing_honey,
		falling_nectar,
		falling_spore_blossom,
		ash,
		crimson_spore,
		warped_spore,
		spore_blossom_air,
		dripping_obsidian_tear,
		falling_obsidian_tear,
		landing_obsidian_tear,
		reverse_portal,
		white_ash,
		small_flame,
		snowflake,
		dripping_dripstone_lava,
		falling_dripstone_lava,
		dripping_dripstone_water,
		falling_dripstone_water,
		glow_squid_ink,
		glow,
		wax_on,
		wax_off,
		electric_spark,
		scrape
	};

	class Particle
	{
	public:
		/* withID este necesar din cauza ca unele pachete cer id-ul
		* de particle separat de pachetul acestuia de write */
		virtual void write(char*& buffer, bool withID) const = 0;
	};

	class ParticleEnumed : public Particle
	{
	private:
		varInt theType;

	public:
		ParticleEnumed(varInt theType) : theType(theType) { }

		void write(char*& buffer, bool withID = true) const;
	};

	class block : Particle
	{
	private:
		varInt blockState;

	public:
		block(varInt blockState) : blockState(blockState) {}

		void write(char*& buffer, bool withID) const;
	};

	class dust : Particle
	{
	private:
		bfloat red, green, blue, scale;

	public:
		dust(bfloat red, bfloat green, bfloat blue, bfloat scale) : red(red), green(green), blue(blue), scale(scale) {}

		void write(char*& buffer, bool withID) const;
	};

	class dustColorTransition : Particle
	{
	private:
		bfloat fromRed, fromGreen, fromBlue;
		bfloat scale;
		bfloat toRed, toGreen, toBlue;

	public:
		dustColorTransition(bfloat fromRed, bfloat fromGreen, bfloat fromBlue, bfloat scale, bfloat toRed, bfloat toGreen, bfloat toBlue) : 
			fromRed(fromRed), fromGreen(fromGreen), fromBlue(fromBlue), scale(scale), toRed(toRed), toGreen(toGreen), toBlue(toBlue) {}

		void write(char*& buffer, bool withID) const;
	};

	class fallingDust : Particle
	{
	private:
		varInt blockState;

	public:
		fallingDust(varInt blockState) : blockState(blockState) {}

		void write(char*& buffer, bool withID) const;
	};

	class item : Particle
	{
	private:
		Slot* slotItem;

	public:
		item(Slot* slotItem) : slotItem(slotItem) {}

		void write(char*& buffer, bool withID) const;
	};

	class vibration : Particle
	{
	private:
		bdouble originX, originY, originZ;
		bdouble destX, destY, destZ;
		bint ticks;

	public:
		vibration(bdouble originX, bdouble originY, bdouble originZ, bdouble destX, bdouble destY, bdouble destZ, bint ticks) :
			originX(originX), originY(originY), originZ(originZ), destX(destX), destY(destY), destZ(destZ), ticks(ticks) {}

		void write(char*& buffer, bool withID) const;
	};
}
