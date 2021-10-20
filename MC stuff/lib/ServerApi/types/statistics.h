#pragma once
#include "varData.h"

class statistic
{
public:
	enum category
	{
		mined,
		crafted,
		used,
		broken,
		picked_up,
		dropped,
		killed,
		killed_by,
		custom
	};
	enum custom
	{
		leave_game,
		play_one_minute,
		time_since_death,
		time_since_rest,
		sneak_time,
		walk_one_cm,
		crouch_one_cm,
		sprint_one_cm,
		walk_on_water_one_cm,
		fall_one_cm,
		climb_one_cm,
		fly_one_cm,
		walk_under_water_one_cm,
		minecart_one_cm,
		boat_one_cm,
		pig_one_cm,
		horse_one_cm,
		aviate_one_cm,
		swim_one_cm,
		strider_one_cm,
		jump,
		drop,
		damage_dealt,
		damage_dealt_absorbed,
		damage_dealt_resisted,
		damage_taken,
		damage_blocked_by_shield,
		damage_absorbed,
		damage_resisted,
		deaths,
		mob_kills,
		animals_bred,
		player_kills,
		fish_caught,
		talked_to_villager,
		traded_with_villager,
		eat_cake_slice,
		fill_cauldron,
		use_cauldron,
		clean_armor,
		clean_banner,
		clean_shulker_box,
		interact_with_brewingstand,
		interact_with_beacon,
		inspect_dropper,
		inspect_hopper,
		inspect_dispenser,
		play_noteblock,
		tune_noteblock,
		pot_flower,
		trigger_trapped_chest,
		open_enderchest,

	};

	varInt categoryId;
	varInt statisticId;
	varInt value;
};
