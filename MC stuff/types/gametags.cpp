#include "gametags.h"

Tags::Tag::Tag(const mcString& name, varInt entryCount, varInt* entries) : name(name), entryCount(entryCount), entries(entries) { }
Tags::Tag::~Tag()
{
	if (entries) delete[] entries;
}

Tags::Tags(varInt tagCount, const mcString& tagType, Tag* tags) : tagCount(tagCount), tagType(tagType), tags(tags) { }
Tags::~Tags()
{
	if (tags) delete[] tags;
}

const varInt Tags::defaltTagsLengthCount = 5;
Tags* Tags::defaultTags = new Tags[5]{
	Tags(118, "minecraft:block", new Tag[118]{
		Tag("minecraft:enderman_holdable",0,nullptr),
		Tag("minecraft:soul_fire_base_blocks",0,nullptr),
		Tag("minecraft:infiniburn_nether",0,nullptr),
		Tag("minecraft:flower_pots",0,nullptr),
		Tag("minecraft:infiniburn_overworld",0,nullptr),
		Tag("minecraft:wooden_fences",0,nullptr),
		Tag("minecraft:piglin_repellents",0,nullptr),
		Tag("minecraft:wall_post_override",0,nullptr),
		Tag("minecraft:wooden_slabs",0,nullptr),
		Tag("minecraft:portals",0,nullptr),
		Tag("minecraft:occludes_vibration_signals",0,nullptr),
		Tag("minecraft:small_flowers",0,nullptr),
		Tag("minecraft:wooden_trapdoors",0,nullptr),
		Tag("minecraft:pressure_plates",0,nullptr),
		Tag("minecraft:jungle_logs",0,nullptr),
		Tag("minecraft:wooden_stairs",0,nullptr),
		Tag("minecraft:spruce_logs",0,nullptr),
		Tag("minecraft:signs",0,nullptr),
		Tag("minecraft:wool",0,nullptr),
		Tag("minecraft:wooden_buttons",8,new varInt[8]{
			(int)Block::minecraft_oak_button,
			(int)Block::minecraft_spruce_button,
			(int)Block::minecraft_birch_button,
			(int)Block::minecraft_jungle_button,
			(int)Block::minecraft_acacia_button,
			(int)Block::minecraft_dark_oak_button,
			(int)Block::minecraft_crimson_button,
			(int)Block::minecraft_warped_button
		}),
		Tag("minecraft:stairs",0,nullptr),
		Tag("minecraft:wither_summon_base_blocks",0,nullptr),
		Tag("minecraft:logs",0,nullptr),
		Tag("minecraft:dripstone_replaceable_blocks",0,nullptr),
		Tag("minecraft:stone_bricks",0,nullptr),
		Tag("minecraft:hoglin_repellents",0,nullptr),
		Tag("minecraft:fire",0,nullptr),
		Tag("minecraft:lava_pool_stone_replaceables",0,nullptr),
		Tag("minecraft:ice",0,nullptr),
		Tag("minecraft:needs_diamond_tool",0,nullptr),
		Tag("minecraft:dragon_immune",0,nullptr),
		Tag("minecraft:wall_signs",0,nullptr),
		Tag("minecraft:features_cannot_replace",0,nullptr),
		Tag("minecraft:slabs",0,nullptr),
		Tag("minecraft:valid_spawn",0,nullptr),
		Tag("minecraft:mushroom_grow_block",0,nullptr),
		Tag("minecraft:guarded_by_piglins",0,nullptr),
		Tag("minecraft:wooden_doors",0,nullptr),
		Tag("minecraft:warped_stems",0,nullptr),
		Tag("minecraft:standing_signs",0,nullptr),
		Tag("minecraft:infiniburn_end",0,nullptr),
		Tag("minecraft:emerald_ores",0,nullptr),
		Tag("minecraft:stone_ore_replaceables",0,nullptr),
		Tag("minecraft:trapdoors",0,nullptr),
		Tag("minecraft:redstone_ores",0,nullptr),
		Tag("minecraft:needs_stone_tool",0,nullptr),
		Tag("minecraft:inside_step_sound_blocks",0,nullptr),
		Tag("minecraft:flowers",0,nullptr),
		Tag("minecraft:prevent_mob_spawning_inside",0,nullptr),
		Tag("minecraft:wart_blocks",0,nullptr),
		Tag("minecraft:planks",0,nullptr),
		Tag("minecraft:soul_speed_blocks",0,nullptr),
		Tag("minecraft:rails",4,new varInt[4]{
			(int)Block::minecraft_rail,
			(int)Block::minecraft_powered_rail,
			(int)Block::minecraft_detector_rail,
			(int)Block::minecraft_activator_rail
		}),
		Tag("minecraft:geode_invalid_blocks",0,nullptr),
		Tag("minecraft:non_flammable_wood",0,nullptr),
		Tag("minecraft:leaves",0,nullptr),
		Tag("minecraft:walls",0,nullptr),
		Tag("minecraft:strider_warm_blocks",0,nullptr),
		Tag("minecraft:fence_gates",0,nullptr),
		Tag("minecraft:shulker_boxes",0,nullptr),
		Tag("minecraft:bee_growables",0,nullptr),
		Tag("minecraft:wooden_pressure_plates",0,nullptr),
		Tag("minecraft:wither_immune",0,nullptr),
		Tag("minecraft:tall_flowers",0,nullptr),
		Tag("minecraft:lapis_ores",0,nullptr),
		Tag("minecraft:moss_replaceable",0,nullptr),
		Tag("minecraft:wall_corals",0,nullptr),
		Tag("minecraft:underwater_bonemeals",0,nullptr),
		Tag("minecraft:stone_pressure_plates",0,nullptr),
		Tag("minecraft:lush_ground_replaceable",0,nullptr),
		Tag("minecraft:impermeable",0,nullptr),
		Tag("minecraft:sand",2,new varInt[2]{
			(int)Block::minecraft_sand,
			(int)Block::minecraft_red_sand
		}),
		Tag("minecraft:nylium",0,nullptr),
		Tag("minecraft:snow",0,nullptr),
		Tag("minecraft:gold_ores",0,nullptr),
		Tag("minecraft:small_dripleaf_placeable",0,nullptr),
		Tag("minecraft:fences",0,nullptr),
		Tag("minecraft:logs_that_burn",0,nullptr),
		Tag("minecraft:saplings",0,nullptr),
		Tag("minecraft:mineable/hoe",0,nullptr),
		Tag("minecraft:dirt",7,new varInt[7]{
			(int)Block::minecraft_dirt,
			(int)Block::minecraft_grass_block,
			(int)Block::minecraft_podzol,
			(int)Block::minecraft_coarse_dirt,
			(int)Block::minecraft_mycelium,
			(int)Block::minecraft_rooted_dirt,
			(int)Block::minecraft_moss_block
		}),
		Tag("minecraft:iron_ores",0,nullptr),
		Tag("minecraft:oak_logs",0,nullptr),
		Tag("minecraft:unstable_bottom_center",0,nullptr),
		Tag("minecraft:doors",0,nullptr),
		Tag("minecraft:needs_iron_tool",0,nullptr),
		Tag("minecraft:mineable/axe",0, nullptr),
		Tag("minecraft:mineable/pickaxe",0, nullptr),
		Tag("minecraft:mineable/shovel",0, nullptr),
		Tag("minecraft:acacia_logs", 4, new varInt[]{
			(int)Block::minecraft_acacia_log,
			(int)Block::minecraft_acacia_wood,
			(int)Block::minecraft_stripped_acacia_log,
			(int)Block::minecraft_stripped_acacia_wood
		}),
		Tag("minecraft:anvil", 3, new varInt[]{
			(int)Block::minecraft_anvil,
			(int)Block::minecraft_chipped_anvil,
			(int)Block::minecraft_damaged_anvil
		}),
		Tag("minecraft:bamboo_plantable_on", 12, new varInt[12]{
			(int)Block::minecraft_sand,
			(int)Block::minecraft_red_sand,
			(int)Block::minecraft_dirt,
			(int)Block::minecraft_grass_block,
			(int)Block::minecraft_podzol,
			(int)Block::minecraft_coarse_dirt,
			(int)Block::minecraft_mycelium,
			(int)Block::minecraft_rooted_dirt,
			(int)Block::minecraft_moss_block,
			(int)Block::minecraft_bamboo,
			(int)Block::minecraft_bamboo_sapling,
			(int)Block::minecraft_gravel
		}),
		Tag("minecraft:banners", 32, new varInt[32]{
			(int)Block::minecraft_white_banner,
			(int)Block::minecraft_orange_banner,
			(int)Block::minecraft_magenta_banner,
			(int)Block::minecraft_light_blue_banner,
			(int)Block::minecraft_yellow_banner,
			(int)Block::minecraft_lime_banner,
			(int)Block::minecraft_pink_banner,
			(int)Block::minecraft_gray_banner,
			(int)Block::minecraft_light_gray_banner,
			(int)Block::minecraft_cyan_banner,
			(int)Block::minecraft_purple_banner,
			(int)Block::minecraft_blue_banner,
			(int)Block::minecraft_brown_banner,
			(int)Block::minecraft_green_banner,
			(int)Block::minecraft_red_banner,
			(int)Block::minecraft_black_banner,
			(int)Block::minecraft_white_wall_banner,
			(int)Block::minecraft_orange_wall_banner,
			(int)Block::minecraft_magenta_wall_banner,
			(int)Block::minecraft_light_blue_wall_banner,
			(int)Block::minecraft_yellow_wall_banner,
			(int)Block::minecraft_lime_wall_banner,
			(int)Block::minecraft_pink_wall_banner,
			(int)Block::minecraft_gray_wall_banner,
			(int)Block::minecraft_light_gray_wall_banner,
			(int)Block::minecraft_cyan_wall_banner,
			(int)Block::minecraft_purple_wall_banner,
			(int)Block::minecraft_blue_wall_banner,
			(int)Block::minecraft_brown_wall_banner,
			(int)Block::minecraft_green_wall_banner,
			(int)Block::minecraft_red_wall_banner,
			(int)Block::minecraft_black_wall_banner
		}),
		Tag("minecraft:base_stone_nether", 3, new varInt[3]{
			(int)Block::minecraft_netherrack,
			(int)Block::minecraft_basalt,
			(int)Block::minecraft_blackstone
		}),
		Tag("minecraft:base_stone_overworld",6, new varInt[6]{
			(int)Block::minecraft_stone,
			(int)Block::minecraft_granite,
			(int)Block::minecraft_diorite,
			(int)Block::minecraft_andesite,
			(int)Block::minecraft_tuff,
			(int)Block::minecraft_deepslate
		}),
		Tag("minecraft:beacon_base_blocks",5, new varInt[5]{
			(int)Block::minecraft_netherite_block,
			(int)Block::minecraft_emerald_block,
			(int)Block::minecraft_diamond_block,
			(int)Block::minecraft_gold_block,
			(int)Block::minecraft_iron_block
		}),
		Tag("minecraft:beds",16, new varInt[16]{
			(int)Block::minecraft_red_bed,
			(int)Block::minecraft_black_bed,
			(int)Block::minecraft_blue_bed,
			(int)Block::minecraft_brown_bed,
			(int)Block::minecraft_cyan_bed,
			(int)Block::minecraft_gray_bed,
			(int)Block::minecraft_green_bed,
			(int)Block::minecraft_light_blue_bed,
			(int)Block::minecraft_light_gray_bed,
			(int)Block::minecraft_lime_bed,
			(int)Block::minecraft_magenta_bed,
			(int)Block::minecraft_orange_bed,
			(int)Block::minecraft_pink_bed,
			(int)Block::minecraft_purple_bed,
			(int)Block::minecraft_white_bed,
			(int)Block::minecraft_yellow_bed
		}),
		Tag("minecraft:beehives",2, new varInt[2]{
			(int)Block::minecraft_bee_nest,
			(int)Block::minecraft_beehive
		}),
		Tag("minecraft:birch_logs", 4, new varInt[4]{
			(int)Block::minecraft_birch_log,
			(int)Block::minecraft_birch_wood,
			(int)Block::minecraft_stripped_birch_log,
			(int)Block::minecraft_stripped_birch_wood
		}),
		Tag("minecraft:buttons",10, new varInt[10]{
			(int)Block::minecraft_oak_button,
			(int)Block::minecraft_spruce_button,
			(int)Block::minecraft_birch_button,
			(int)Block::minecraft_jungle_button,
			(int)Block::minecraft_acacia_button,
			(int)Block::minecraft_dark_oak_button,
			(int)Block::minecraft_crimson_button,
			(int)Block::minecraft_warped_button,
			(int)Block::minecraft_stone_button,
			(int)Block::minecraft_polished_blackstone_button
			}),
		Tag("minecraft:campfires",2, new varInt[2]{
			(int)Block::minecraft_campfire,
			(int)Block::minecraft_soul_campfire
		}),
		Tag("minecraft:candle_cakes",17, new varInt[17]{
			(int)Block::minecraft_candle_cake,
			(int)Block::minecraft_white_candle_cake,
			(int)Block::minecraft_orange_candle_cake,
			(int)Block::minecraft_magenta_candle_cake,
			(int)Block::minecraft_light_blue_candle_cake,
			(int)Block::minecraft_yellow_candle_cake,
			(int)Block::minecraft_lime_candle_cake,
			(int)Block::minecraft_pink_candle_cake,
			(int)Block::minecraft_gray_candle_cake,
			(int)Block::minecraft_light_gray_candle_cake,
			(int)Block::minecraft_cyan_candle_cake,
			(int)Block::minecraft_purple_candle_cake,
			(int)Block::minecraft_blue_candle_cake,
			(int)Block::minecraft_brown_candle_cake,
			(int)Block::minecraft_green_candle_cake,
			(int)Block::minecraft_red_candle_cake,
			(int)Block::minecraft_black_candle_cake
		}),
		Tag("minecraft:candles",17, new varInt[17]{
			(int)Block::minecraft_candle,
			(int)Block::minecraft_white_candle,
			(int)Block::minecraft_orange_candle,
			(int)Block::minecraft_magenta_candle,
			(int)Block::minecraft_light_blue_candle,
			(int)Block::minecraft_yellow_candle,
			(int)Block::minecraft_lime_candle,
			(int)Block::minecraft_pink_candle,
			(int)Block::minecraft_gray_candle,
			(int)Block::minecraft_light_gray_candle,
			(int)Block::minecraft_cyan_candle,
			(int)Block::minecraft_purple_candle,
			(int)Block::minecraft_blue_candle,
			(int)Block::minecraft_brown_candle,
			(int)Block::minecraft_green_candle,
			(int)Block::minecraft_red_candle,
			(int)Block::minecraft_black_candle
		}),
		Tag("minecraft:carpets",16, new varInt[16]{
			(int)Block::minecraft_white_carpet,
			(int)Block::minecraft_orange_carpet,
			(int)Block::minecraft_magenta_carpet,
			(int)Block::minecraft_light_blue_carpet,
			(int)Block::minecraft_yellow_carpet,
			(int)Block::minecraft_lime_carpet,
			(int)Block::minecraft_pink_carpet,
			(int)Block::minecraft_gray_carpet,
			(int)Block::minecraft_light_gray_carpet,
			(int)Block::minecraft_cyan_carpet,
			(int)Block::minecraft_purple_carpet,
			(int)Block::minecraft_blue_carpet,
			(int)Block::minecraft_brown_carpet,
			(int)Block::minecraft_green_carpet,
			(int)Block::minecraft_red_carpet,
			(int)Block::minecraft_black_carpet
		}),
		Tag("minecraft:cauldrons", 4, new varInt[4]{
			(int)Block::minecraft_cauldron,
			(int)Block::minecraft_water_cauldron,
			(int)Block::minecraft_lava_cauldron,
			(int)Block::minecraft_powder_snow_cauldron
		}),
		Tag("minecraft:cave_vines",2, new varInt[2]{
			(int)Block::minecraft_cave_vines_plant,
			(int)Block::minecraft_cave_vines
		}),
		Tag("minecraft:climbable", 9, new varInt[9]{
			(int)Block::minecraft_ladder,
			(int)Block::minecraft_vine,
			(int)Block::minecraft_scaffolding,
			(int)Block::minecraft_weeping_vines,
			(int)Block::minecraft_weeping_vines_plant,
			(int)Block::minecraft_twisting_vines,
			(int)Block::minecraft_twisting_vines_plant,
			(int)Block::minecraft_cave_vines,
			(int)Block::minecraft_cave_vines_plant
		}),
		Tag("minecraft:coal_ores",2, new varInt[2]{
			(int)Block::minecraft_coal_ore,
			(int)Block::minecraft_deepslate_coal_ore
		}),
		Tag("minecraft:copper_ores",2, new varInt[2]{
			(int)Block::minecraft_copper_ore,
			(int)Block::minecraft_deepslate_copper_ore
		}),
		Tag("minecraft:corals",10, new varInt[10]{
			(int)Block::minecraft_tube_coral,
			(int)Block::minecraft_brain_coral,
			(int)Block::minecraft_bubble_coral,
			(int)Block::minecraft_fire_coral,
			(int)Block::minecraft_horn_coral,
			(int)Block::minecraft_tube_coral_fan,
			(int)Block::minecraft_brain_coral_fan,
			(int)Block::minecraft_bubble_coral_fan,
			(int)Block::minecraft_fire_coral_fan,
			(int)Block::minecraft_horn_coral_fan
		}),
		Tag("minecraft:coral_blocks",5, new varInt[5]{
			(int)Block::minecraft_tube_coral_block,
			(int)Block::minecraft_brain_coral_block,
			(int)Block::minecraft_bubble_coral_block,
			(int)Block::minecraft_fire_coral_block,
			(int)Block::minecraft_horn_coral_block
		}),
		Tag("minecraft:coral_plants",5, new varInt[5]{
			(int)Block::minecraft_tube_coral,
			(int)Block::minecraft_brain_coral,
			(int)Block::minecraft_bubble_coral,
			(int)Block::minecraft_fire_coral,
			(int)Block::minecraft_horn_coral
		}),
		Tag("minecraft:crimson_stems",4, new varInt[4]{
			(int)Block::minecraft_crimson_stem,
			(int)Block::minecraft_stripped_crimson_stem,
			(int)Block::minecraft_crimson_hyphae,
			(int)Block::minecraft_stripped_crimson_hyphae
		}),
		Tag("minecraft:crops",6, new varInt[6]{
			(int)Block::minecraft_beetroots,
			(int)Block::minecraft_carrots,
			(int)Block::minecraft_potatoes,
			(int)Block::minecraft_wheat,
			(int)Block::minecraft_melon_stem,
			(int)Block::minecraft_pumpkin_stem
		}),
		Tag("minecraft:crystal_sound_blocks",2, new varInt[2]{
			(int)Block::minecraft_amethyst_block,
			(int)Block::minecraft_budding_amethyst
		}),
		Tag("minecraft:dark_oak_logs",4, new varInt[4]{
			(int)Block::minecraft_dark_oak_log,
			(int)Block::minecraft_dark_oak_wood,
			(int)Block::minecraft_stripped_dark_oak_log,
			(int)Block::minecraft_stripped_dark_oak_wood
		}),
		Tag("minecraft:deepslate_ore_replaceables",2, new varInt[2]{
			(int)Block::minecraft_deepslate,
			(int)Block::minecraft_tuff
		}),
		Tag("minecraft:diamond_ores",2, new varInt[2]{
			(int)Block::minecraft_diamond_ore,
			(int)Block::minecraft_deepslate_diamond_ore
		}),
	}),
	Tags(69, "minecraft:item", new Tag[69]{
		Tag("minecraft:coals",0,nullptr),
		Tag("minecraft:banners",0,nullptr),
		Tag("minecraft:soul_fire_base_blocks",0,nullptr),
		Tag("minecraft:stone_crafting_materials",0,nullptr),
		Tag("minecraft:wooden_fences",0,nullptr),
		Tag("minecraft:piglin_repellents",0,nullptr),
		Tag("minecraft:beacon_payment_items",0,nullptr),
		Tag("minecraft:wooden_slabs",0,nullptr),
		Tag("minecraft:coal_ores",0,nullptr),
		Tag("minecraft:occludes_vibration_signals",0,nullptr),
		Tag("minecraft:small_flowers",0,nullptr),
		Tag("minecraft:wooden_trapdoors",0,nullptr),
		Tag("minecraft:jungle_logs",0,nullptr),
		Tag("minecraft:lectern_books",0,nullptr),
		Tag("minecraft:wooden_stairs",0,nullptr),
		Tag("minecraft:spruce_logs",0,nullptr),
		Tag("minecraft:signs",0,nullptr),
		Tag("minecraft:axolotl_tempt_items",0,nullptr),
		Tag("minecraft:carpets",0,nullptr),
		Tag("minecraft:wool",0,nullptr),
		Tag("minecraft:wooden_buttons",0,nullptr),
		Tag("minecraft:stairs",0,nullptr),
		Tag("minecraft:fishes",0,nullptr),
		Tag("minecraft:logs",0,nullptr),
		Tag("minecraft:stone_bricks",0,nullptr),
		Tag("minecraft:creeper_drop_music_discs",0,nullptr),
		Tag("minecraft:arrows",3,new varInt[3]{
			(int)Item::minecraft_arrow,
			(int)Item::minecraft_tipped_arrow,
			(int)Item::minecraft_spectral_arrow
		}),
		Tag("minecraft:slabs",0,nullptr),
		Tag("minecraft:wooden_doors",0,nullptr),
		Tag("minecraft:warped_stems",0,nullptr),
		Tag("minecraft:emerald_ores",0,nullptr),
		Tag("minecraft:trapdoors",0,nullptr),
		Tag("minecraft:redstone_ores",0,nullptr),
		Tag("minecraft:crimson_stems",0,nullptr),
		Tag("minecraft:ignored_by_piglin_babies",0,nullptr),
		Tag("minecraft:buttons",0,nullptr),
		Tag("minecraft:flowers",0,nullptr),
		Tag("minecraft:stone_tool_materials",0,nullptr),
		Tag("minecraft:planks",0,nullptr),
		Tag("minecraft:fox_food",0,nullptr),
		Tag("minecraft:boats",0,nullptr),
		Tag("minecraft:dark_oak_logs",0,nullptr),
		Tag("minecraft:rails",4,new varInt[4]{
			(int)Item::minecraft_rail,
			(int)Item::minecraft_powered_rail,
			(int)Item::minecraft_detector_rail,
			(int)Item::minecraft_activator_rail
		}),
		Tag("minecraft:diamond_ores",0,nullptr),
		Tag("minecraft:non_flammable_wood",0,nullptr),
		Tag("minecraft:leaves",0,nullptr),
		Tag("minecraft:walls",0,nullptr),
		Tag("minecraft:piglin_food",0,nullptr),
		Tag("minecraft:wooden_pressure_plates",0,nullptr),
		Tag("minecraft:acacia_logs",4,new varInt[4]{
			(int)Item::minecraft_acacia_log,
			(int)Item::minecraft_acacia_wood,
			(int)Item::minecraft_stripped_acacia_log,
			(int)Item::minecraft_stripped_acacia_wood
		}),
		Tag("minecraft:anvil",3,new varInt[3]{
			(int)Item::minecraft_anvil,
			(int)Item::minecraft_chipped_anvil,
			(int)Item::minecraft_damaged_anvil
		}),
		Tag("minecraft:piglin_loved",0,nullptr),
		Tag("minecraft:candles",0,nullptr),
		Tag("minecraft:music_discs",0,nullptr),
		Tag("minecraft:birch_logs",0,nullptr),
		Tag("minecraft:tall_flowers",0,nullptr),
		Tag("minecraft:lapis_ores",0,nullptr),
		Tag("minecraft:sand",0,nullptr),
		Tag("minecraft:copper_ores",0,nullptr),
		Tag("minecraft:gold_ores",0,nullptr),
		Tag("minecraft:freeze_immune_wearables",0,nullptr),
		Tag("minecraft:logs_that_burn",0,nullptr),
		Tag("minecraft:fences",0,nullptr),
		Tag("minecraft:saplings",0,nullptr),
		Tag("minecraft:beds",0,nullptr),
		Tag("minecraft:iron_ores",0,nullptr),
		Tag("minecraft:oak_logs",0,nullptr),
		Tag("minecraft:doors",0,nullptr),
		Tag("minecraft:cluster_max_harvestables",0,nullptr)
	}),
	Tags(2, "minecraft:fluid", new Tag[2]{
		Tag("minecraft:water",2,new varInt[2]{
			(int)Fluid::minecraft_water,
			(int)Fluid::minecraft_flowing_water
		}),
		Tag("minecraft:lava",2,new varInt[2]{
			(int)Fluid::minecraft_lava,
			(int)Fluid::minecraft_flowing_lava
		})
	}),
	Tags(10, "minecraft:entity_type", new Tag[10]{
		Tag("minecraft:axolotl_always_hostiles",0,nullptr),
		Tag("minecraft:freeze_hurts_extra_types",0,nullptr),
		Tag("minecraft:freeze_immune_entity_types",0,nullptr),
		Tag("minecraft:beehive_inhabitors",0,nullptr),
		Tag("minecraft:impact_projectiles",0,nullptr),
		Tag("minecraft:skeletons",0,nullptr),
		Tag("minecraft:raiders",0,nullptr),
		Tag("minecraft:powder_snow_walkable_mobs",0,nullptr),
		Tag("minecraft:axolotl_hunt_targets",0,nullptr),
		Tag("minecraft:arrows",0,nullptr)
	}),
	Tags(2, "minecraft:game_event", new Tag[2]{
		Tag("minecraft:vibrations",0,nullptr),
		Tag("minecraft:ignore_vibrations_sneaking",0,nullptr)
	})
};