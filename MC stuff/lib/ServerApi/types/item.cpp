
#include "item.h"

Byte items::getStackableSize(int i) {
	switch (static_cast<Item>(i))
	{
	case Item::minecraft_diamond_horse_armor:
	case Item::minecraft_golden_horse_armor:
	case Item::minecraft_iron_horse_armor:
	case Item::minecraft_leather_horse_armor:
	case Item::minecraft_saddle:
	case Item::minecraft_leather_boots:
	case Item::minecraft_leather_leggings:
	case Item::minecraft_leather_chestplate:
	case Item::minecraft_leather_helmet:
	case Item::minecraft_chainmail_boots:
	case Item::minecraft_chainmail_leggings:
	case Item::minecraft_chainmail_chestplate:
	case Item::minecraft_chainmail_helmet:
	case Item::minecraft_iron_boots:
	case Item::minecraft_iron_leggings:
	case Item::minecraft_iron_chestplate:
	case Item::minecraft_iron_helmet:
	case Item::minecraft_golden_boots:
	case Item::minecraft_golden_leggings:
	case Item::minecraft_golden_chestplate:
	case Item::minecraft_golden_helmet:
	case Item::minecraft_diamond_boots:
	case Item::minecraft_diamond_leggings:
	case Item::minecraft_diamond_chestplate:
	case Item::minecraft_diamond_helmet:
	case Item::minecraft_netherite_boots:
	case Item::minecraft_netherite_leggings:
	case Item::minecraft_netherite_chestplate:
	case Item::minecraft_netherite_helmet:
	case Item::minecraft_turtle_helmet:
	case Item::minecraft_wooden_hoe:
	case Item::minecraft_wooden_shovel:
	case Item::minecraft_wooden_pickaxe:
	case Item::minecraft_wooden_axe:
	case Item::minecraft_wooden_sword:
	case Item::minecraft_iron_hoe:
	case Item::minecraft_iron_shovel:
	case Item::minecraft_iron_pickaxe:
	case Item::minecraft_iron_axe:
	case Item::minecraft_iron_sword:
	case Item::minecraft_golden_hoe:
	case Item::minecraft_golden_shovel:
	case Item::minecraft_golden_pickaxe:
	case Item::minecraft_golden_axe:
	case Item::minecraft_golden_sword:
	case Item::minecraft_diamond_hoe:
	case Item::minecraft_diamond_shovel:
	case Item::minecraft_diamond_pickaxe:
	case Item::minecraft_diamond_axe:
	case Item::minecraft_diamond_sword:
	case Item::minecraft_netherite_hoe:
	case Item::minecraft_netherite_shovel:
	case Item::minecraft_netherite_pickaxe:
	case Item::minecraft_netherite_axe:
	case Item::minecraft_netherite_sword:
	case Item::minecraft_bow:
	case Item::minecraft_crossbow:
	case Item::minecraft_carrot_on_a_stick:
	case Item::minecraft_totem_of_undying:
	case Item::minecraft_flint_and_steel:
	case Item::minecraft_fishing_rod:
	case Item::minecraft_trident:
	case Item::minecraft_shears:
	case Item::minecraft_spyglass:
	case Item::minecraft_shield:
	case Item::minecraft_elytra:
	case Item::minecraft_acacia_boat:
	case Item::minecraft_oak_boat:
	case Item::minecraft_spruce_boat:
	case Item::minecraft_birch_boat:
	case Item::minecraft_jungle_boat:
	case Item::minecraft_dark_oak_boat:
	case Item::minecraft_minecart:
	case Item::minecraft_command_block_minecart:
	case Item::minecraft_chest_minecart:
	case Item::minecraft_furnace_minecart:
	case Item::minecraft_hopper_minecart:
	case Item::minecraft_tnt_minecart:
	case Item::minecraft_shulker_box:
	case Item::minecraft_white_shulker_box:
	case Item::minecraft_orange_shulker_box:
	case Item::minecraft_magenta_shulker_box:
	case Item::minecraft_light_blue_shulker_box:
	case Item::minecraft_yellow_shulker_box:
	case Item::minecraft_lime_shulker_box:
	case Item::minecraft_pink_shulker_box:
	case Item::minecraft_gray_shulker_box:
	case Item::minecraft_light_gray_shulker_box:
	case Item::minecraft_cyan_shulker_box:
	case Item::minecraft_purple_shulker_box:
	case Item::minecraft_blue_shulker_box:
	case Item::minecraft_brown_shulker_box:
	case Item::minecraft_green_shulker_box:
	case Item::minecraft_red_shulker_box:
	case Item::minecraft_black_shulker_box:
	case Item::minecraft_music_disc_13:
	case Item::minecraft_music_disc_cat:
	case Item::minecraft_music_disc_blocks:
	case Item::minecraft_music_disc_chirp:
	case Item::minecraft_music_disc_far:
	case Item::minecraft_music_disc_mall:
	case Item::minecraft_music_disc_mellohi:
	case Item::minecraft_music_disc_stal:
	case Item::minecraft_music_disc_strad:
	case Item::minecraft_music_disc_ward:
	case Item::minecraft_music_disc_11:
	case Item::minecraft_music_disc_wait:
	case Item::minecraft_music_disc_otherside:
	case Item::minecraft_music_disc_pigstep:
	case Item::minecraft_white_bed:
	case Item::minecraft_orange_bed:
	case Item::minecraft_magenta_bed:
	case Item::minecraft_light_blue_bed:
	case Item::minecraft_yellow_bed:
	case Item::minecraft_lime_bed:
	case Item::minecraft_pink_bed:
	case Item::minecraft_gray_bed:
	case Item::minecraft_light_gray_bed:
	case Item::minecraft_cyan_bed:
	case Item::minecraft_purple_bed:
	case Item::minecraft_blue_bed:
	case Item::minecraft_brown_bed:
	case Item::minecraft_green_bed:
	case Item::minecraft_red_bed:
	case Item::minecraft_black_bed:
	case Item::minecraft_water_bucket:
	case Item::minecraft_lava_bucket:
	case Item::minecraft_powder_snow_bucket:
	case Item::minecraft_milk_bucket:
	case Item::minecraft_pufferfish_bucket:
	case Item::minecraft_salmon_bucket:
	case Item::minecraft_cod_bucket:
	case Item::minecraft_tropical_fish_bucket:
	case Item::minecraft_axolotl_bucket:
	case Item::minecraft_beetroot_soup:
	case Item::minecraft_mushroom_stew:
	case Item::minecraft_suspicious_stew:
	case Item::minecraft_rabbit_stew:
	case Item::minecraft_filled_map:
	case Item::minecraft_map:
	case Item::minecraft_book:
	case Item::minecraft_writable_book:
	case Item::minecraft_written_book:
	case Item::minecraft_enchanted_book:
	case Item::minecraft_knowledge_book:
	case Item::minecraft_potion:
	case Item::minecraft_splash_potion:
	case Item::minecraft_lingering_potion:
	case Item::minecraft_flower_banner_pattern:
	case Item::minecraft_creeper_banner_pattern:
	case Item::minecraft_skull_banner_pattern:
	case Item::minecraft_mojang_banner_pattern:
	case Item::minecraft_globe_banner_pattern:
	case Item::minecraft_piglin_banner_pattern:
		return 1;
	case Item::minecraft_snowball:
	case Item::minecraft_bucket:
	case Item::minecraft_egg:
	case Item::minecraft_armor_stand:
	case Item::minecraft_ender_pearl:
	case Item::minecraft_honey_bottle:
	case Item::minecraft_acacia_sign:
	case Item::minecraft_oak_sign:
	case Item::minecraft_spruce_sign:
	case Item::minecraft_birch_sign:
	case Item::minecraft_jungle_sign:
	case Item::minecraft_dark_oak_sign:
	case Item::minecraft_crimson_sign:
	case Item::minecraft_warped_sign:
	case Item::minecraft_white_banner:
	case Item::minecraft_orange_banner:
	case Item::minecraft_magenta_banner:
	case Item::minecraft_light_blue_banner:
	case Item::minecraft_yellow_banner:
	case Item::minecraft_lime_banner:
	case Item::minecraft_pink_banner:
	case Item::minecraft_gray_banner:
	case Item::minecraft_light_gray_banner:
	case Item::minecraft_cyan_banner:
	case Item::minecraft_purple_banner:
	case Item::minecraft_blue_banner:
	case Item::minecraft_brown_banner:
	case Item::minecraft_green_banner:
	case Item::minecraft_red_banner:
	case Item::minecraft_black_banner:
		return 16;
	}

	return 64;
}
