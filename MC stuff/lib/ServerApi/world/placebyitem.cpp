#include "../world.h"
#include "../types/item.h"
#include "../player/message.h"

enum class slabType : Byte
{
	bottom,
	top,
	dbl
};
enum class logAxis : Byte
{
	x,
	y,
	z
};
enum class blockFacing : Byte
{
	north,
	south,
	east,
	west,
	up,
	down
};
enum class doorHinge :Byte
{
	right,
	left
};

const int waterSurceBlockStateId = 34;

Block stateToBlock(const BlockState& state)
{
	return (Block)Registry::getId(Registry::blockRegistry, Registry::getBlock(state.id));
}

float rotationOffset(Item itemId)
{
	switch (itemId)
	{
	case Item::minecraft_anvil:
	case Item::minecraft_chipped_anvil:
	case Item::minecraft_damaged_anvil:
		return -90.f;
	case Item::minecraft_observer:
	case Item::minecraft_iron_door:
	case Item::minecraft_oak_door:
	case Item::minecraft_spruce_door:
	case Item::minecraft_birch_door:
	case Item::minecraft_jungle_door:
	case Item::minecraft_acacia_door:
	case Item::minecraft_dark_oak_door:
	case Item::minecraft_crimson_door:
	case Item::minecraft_warped_door:
		return 180.f;
	}
	return 0.f;
}
std::string facingToString(blockFacing f)
{
	switch (f)
	{
	case blockFacing::north:
		return "north";
	case blockFacing::south:
		return "south";
	case blockFacing::east:
		return "east";
	case blockFacing::west:
		return "west";
	case blockFacing::up:
		return "up";
	case blockFacing::down:
		return "down";
	}
	throw std::exception("invalid");
}
blockFacing getHorizontalFacing(Player* p, float playerYaw, Item itemId)
{
	//yaw modulation [0, 360)
	playerYaw += rotationOffset(itemId);
	int yawInt = (int)(playerYaw);
	yawInt -= yawInt % 360;
	playerYaw -= yawInt;
	if (playerYaw < 0) playerYaw += 360;

	//temp
	if (playerYaw == 45.f || playerYaw == 135.f || playerYaw == 225.f || playerYaw == 315.f)
		message::play::send::chatMessage(p, Chat("Are you a robot?", Chat::color::red), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));

	if (playerYaw < 45 || playerYaw>315)
	{
		return blockFacing::north;
	}
	if (playerYaw <= 135)
	{
		return blockFacing::east;
	}
	if (playerYaw < 225)
	{
		return blockFacing::south;
	}
	return blockFacing::west;
}
blockFacing get3DFacing(Player* p, float playerYaw, float playerPitch, Item itemId)
{
	if (playerPitch == 45.f || playerPitch == -45.f)
		message::play::send::chatMessage(p, Chat("Are you a robot?", Chat::color::red), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));

	if (playerPitch >= 45.f) return blockFacing::up;
	if (playerPitch <= -45.f) return blockFacing::down;
	return getHorizontalFacing(p, playerYaw, itemId);
}

//is this block full and solid? = can any block be placed on any side of this block and do fences, glass panes and walls connect to it?
bool fullSolidBlock(Block id)
{
	switch (id)
	{
	case Block::minecraft_stone:
	case Block::minecraft_granite:
	case Block::minecraft_polished_granite:
	case Block::minecraft_diorite:
	case Block::minecraft_polished_diorite:
	case Block::minecraft_andesite:
	case Block::minecraft_polished_andesite:
	case Block::minecraft_grass_block:
	case Block::minecraft_dirt:
	case Block::minecraft_coarse_dirt:
	case Block::minecraft_podzol:
	case Block::minecraft_cobblestone:
	case Block::minecraft_oak_planks:
	case Block::minecraft_spruce_planks:
	case Block::minecraft_birch_planks:
	case Block::minecraft_jungle_planks:
	case Block::minecraft_acacia_planks:
	case Block::minecraft_dark_oak_planks:
	case Block::minecraft_bedrock:
	case Block::minecraft_sand:
	case Block::minecraft_red_sand:
	case Block::minecraft_gravel:
	case Block::minecraft_gold_ore:
	case Block::minecraft_deepslate_gold_ore:
	case Block::minecraft_iron_ore:
	case Block::minecraft_deepslate_iron_ore:
	case Block::minecraft_coal_ore:
	case Block::minecraft_deepslate_coal_ore:
	case Block::minecraft_nether_gold_ore:
	case Block::minecraft_oak_log:
	case Block::minecraft_spruce_log:
	case Block::minecraft_birch_log:
	case Block::minecraft_jungle_log:
	case Block::minecraft_acacia_log:
	case Block::minecraft_dark_oak_log:
	case Block::minecraft_stripped_spruce_log:
	case Block::minecraft_stripped_birch_log:
	case Block::minecraft_stripped_jungle_log:
	case Block::minecraft_stripped_acacia_log:
	case Block::minecraft_stripped_dark_oak_log:
	case Block::minecraft_stripped_oak_log:
	case Block::minecraft_oak_wood:
	case Block::minecraft_spruce_wood:
	case Block::minecraft_birch_wood:
	case Block::minecraft_jungle_wood:
	case Block::minecraft_acacia_wood:
	case Block::minecraft_dark_oak_wood:
	case Block::minecraft_stripped_oak_wood:
	case Block::minecraft_stripped_spruce_wood:
	case Block::minecraft_stripped_birch_wood:
	case Block::minecraft_stripped_jungle_wood:
	case Block::minecraft_stripped_acacia_wood:
	case Block::minecraft_stripped_dark_oak_wood:
	case Block::minecraft_sponge:
	case Block::minecraft_wet_sponge:
	case Block::minecraft_glass:
	case Block::minecraft_lapis_ore:
	case Block::minecraft_deepslate_lapis_ore:
	case Block::minecraft_lapis_block:
	case Block::minecraft_dispenser:
	case Block::minecraft_sandstone:
	case Block::minecraft_chiseled_sandstone:
	case Block::minecraft_cut_sandstone:
	case Block::minecraft_note_block:
	case Block::minecraft_sticky_piston:
	case Block::minecraft_piston:
	case Block::minecraft_white_wool:
	case Block::minecraft_orange_wool:
	case Block::minecraft_magenta_wool:
	case Block::minecraft_light_blue_wool:
	case Block::minecraft_yellow_wool:
	case Block::minecraft_lime_wool:
	case Block::minecraft_pink_wool:
	case Block::minecraft_gray_wool:
	case Block::minecraft_light_gray_wool:
	case Block::minecraft_cyan_wool:
	case Block::minecraft_purple_wool:
	case Block::minecraft_blue_wool:
	case Block::minecraft_brown_wool:
	case Block::minecraft_green_wool:
	case Block::minecraft_red_wool:
	case Block::minecraft_black_wool:
	case Block::minecraft_gold_block:
	case Block::minecraft_iron_block:
	case Block::minecraft_bricks:
	case Block::minecraft_tnt:
	case Block::minecraft_bookshelf:
	case Block::minecraft_mossy_cobblestone:
	case Block::minecraft_obsidian:
	case Block::minecraft_spawner:
	case Block::minecraft_diamond_ore:
	case Block::minecraft_deepslate_diamond_ore:
	case Block::minecraft_diamond_block:
	case Block::minecraft_crafting_table:
		//case Block::minecraft_farmland:
	case Block::minecraft_furnace:
	case Block::minecraft_redstone_ore:
	case Block::minecraft_deepslate_redstone_ore:
	case Block::minecraft_snow_block:
	case Block::minecraft_clay:
	case Block::minecraft_jukebox:
	case Block::minecraft_netherrack:
	case Block::minecraft_soul_sand:
	case Block::minecraft_soul_soil:
	case Block::minecraft_basalt:
	case Block::minecraft_polished_basalt:
	case Block::minecraft_glowstone: //?
	case Block::minecraft_white_stained_glass:
	case Block::minecraft_orange_stained_glass:
	case Block::minecraft_magenta_stained_glass:
	case Block::minecraft_light_blue_stained_glass:
	case Block::minecraft_yellow_stained_glass:
	case Block::minecraft_lime_stained_glass:
	case Block::minecraft_pink_stained_glass:
	case Block::minecraft_gray_stained_glass:
	case Block::minecraft_light_gray_stained_glass:
	case Block::minecraft_cyan_stained_glass:
	case Block::minecraft_purple_stained_glass:
	case Block::minecraft_blue_stained_glass:
	case Block::minecraft_brown_stained_glass:
	case Block::minecraft_green_stained_glass:
	case Block::minecraft_red_stained_glass:
	case Block::minecraft_black_stained_glass:
	case Block::minecraft_stone_bricks:
	case Block::minecraft_mossy_stone_bricks:
	case Block::minecraft_cracked_stone_bricks:
	case Block::minecraft_chiseled_stone_bricks:
	case Block::minecraft_infested_stone:
	case Block::minecraft_infested_cobblestone:
	case Block::minecraft_infested_stone_bricks:
	case Block::minecraft_infested_mossy_stone_bricks:
	case Block::minecraft_infested_cracked_stone_bricks:
	case Block::minecraft_infested_chiseled_stone_bricks:
	case Block::minecraft_brown_mushroom_block:
	case Block::minecraft_red_mushroom_block:
	case Block::minecraft_mushroom_stem:
	case Block::minecraft_mycelium:
	case Block::minecraft_nether_bricks:
	case Block::minecraft_end_stone:
	case Block::minecraft_redstone_lamp:
	case Block::minecraft_emerald_ore:
	case Block::minecraft_deepslate_emerald_ore:
	case Block::minecraft_emerald_block:
	case Block::minecraft_command_block:
	case Block::minecraft_beacon:
	case Block::minecraft_redstone_block:
	case Block::minecraft_nether_quartz_ore:
	case Block::minecraft_quartz_block:
	case Block::minecraft_chiseled_quartz_block:
	case Block::minecraft_quartz_pillar:
	case Block::minecraft_dropper:
	case Block::minecraft_white_terracotta:
	case Block::minecraft_orange_terracotta:
	case Block::minecraft_magenta_terracotta:
	case Block::minecraft_light_blue_terracotta:
	case Block::minecraft_yellow_terracotta:
	case Block::minecraft_lime_terracotta:
	case Block::minecraft_pink_terracotta:
	case Block::minecraft_gray_terracotta:
	case Block::minecraft_light_gray_terracotta:
	case Block::minecraft_cyan_terracotta:
	case Block::minecraft_purple_terracotta:
	case Block::minecraft_blue_terracotta:
	case Block::minecraft_brown_terracotta:
	case Block::minecraft_green_terracotta:
	case Block::minecraft_red_terracotta:
	case Block::minecraft_black_terracotta:
	case Block::minecraft_slime_block:
	case Block::minecraft_prismarine:
	case Block::minecraft_prismarine_bricks:
	case Block::minecraft_dark_prismarine:
	case Block::minecraft_sea_lantern:
	case Block::minecraft_hay_block:
	case Block::minecraft_terracotta:
	case Block::minecraft_coal_block:
	case Block::minecraft_red_sandstone:
	case Block::minecraft_chiseled_red_sandstone:
	case Block::minecraft_cut_red_sandstone:
	case Block::minecraft_smooth_stone:
	case Block::minecraft_smooth_sandstone:
	case Block::minecraft_smooth_quartz:
	case Block::minecraft_smooth_red_sandstone:
	case Block::minecraft_purpur_block:
	case Block::minecraft_purpur_pillar:
	case Block::minecraft_end_stone_bricks:
		//case Block::minecraft_dirt_path:
	case Block::minecraft_repeating_command_block:
	case Block::minecraft_chain_command_block:
	case Block::minecraft_magma_block:
	case Block::minecraft_nether_wart_block:
	case Block::minecraft_red_nether_bricks:
	case Block::minecraft_bone_block:
	case Block::minecraft_observer:
	case Block::minecraft_white_glazed_terracotta:
	case Block::minecraft_orange_glazed_terracotta:
	case Block::minecraft_magenta_glazed_terracotta:
	case Block::minecraft_light_blue_glazed_terracotta:
	case Block::minecraft_yellow_glazed_terracotta:
	case Block::minecraft_lime_glazed_terracotta:
	case Block::minecraft_pink_glazed_terracotta:
	case Block::minecraft_gray_glazed_terracotta:
	case Block::minecraft_light_gray_glazed_terracotta:
	case Block::minecraft_cyan_glazed_terracotta:
	case Block::minecraft_purple_glazed_terracotta:
	case Block::minecraft_blue_glazed_terracotta:
	case Block::minecraft_brown_glazed_terracotta:
	case Block::minecraft_green_glazed_terracotta:
	case Block::minecraft_red_glazed_terracotta:
	case Block::minecraft_black_glazed_terracotta:
	case Block::minecraft_white_concrete:
	case Block::minecraft_orange_concrete:
	case Block::minecraft_magenta_concrete:
	case Block::minecraft_light_blue_concrete:
	case Block::minecraft_yellow_concrete:
	case Block::minecraft_lime_concrete:
	case Block::minecraft_pink_concrete:
	case Block::minecraft_gray_concrete:
	case Block::minecraft_light_gray_concrete:
	case Block::minecraft_cyan_concrete:
	case Block::minecraft_purple_concrete:
	case Block::minecraft_blue_concrete:
	case Block::minecraft_brown_concrete:
	case Block::minecraft_green_concrete:
	case Block::minecraft_red_concrete:
	case Block::minecraft_black_concrete:
	case Block::minecraft_white_concrete_powder:
	case Block::minecraft_orange_concrete_powder:
	case Block::minecraft_magenta_concrete_powder:
	case Block::minecraft_light_blue_concrete_powder:
	case Block::minecraft_yellow_concrete_powder:
	case Block::minecraft_lime_concrete_powder:
	case Block::minecraft_pink_concrete_powder:
	case Block::minecraft_gray_concrete_powder:
	case Block::minecraft_light_gray_concrete_powder:
	case Block::minecraft_cyan_concrete_powder:
	case Block::minecraft_purple_concrete_powder:
	case Block::minecraft_blue_concrete_powder:
	case Block::minecraft_brown_concrete_powder:
	case Block::minecraft_green_concrete_powder:
	case Block::minecraft_red_concrete_powder:
	case Block::minecraft_black_concrete_powder:
	case Block::minecraft_dried_kelp_block:
	case Block::minecraft_dead_tube_coral_block:
	case Block::minecraft_dead_brain_coral_block:
	case Block::minecraft_dead_bubble_coral_block:
	case Block::minecraft_dead_fire_coral_block:
	case Block::minecraft_dead_horn_coral_block:
	case Block::minecraft_tube_coral_block:
	case Block::minecraft_brain_coral_block:
	case Block::minecraft_bubble_coral_block:
	case Block::minecraft_fire_coral_block:
	case Block::minecraft_horn_coral_block:
	case Block::minecraft_blue_ice:
	case Block::minecraft_loom:
	case Block::minecraft_barrel:
	case Block::minecraft_smoker:
	case Block::minecraft_blast_furnace:
	case Block::minecraft_cartography_table:
	case Block::minecraft_fletching_table:
	case Block::minecraft_smithing_table:
	case Block::minecraft_warped_stem:
	case Block::minecraft_stripped_warped_stem:
	case Block::minecraft_warped_hyphae:
	case Block::minecraft_stripped_warped_hyphae:
	case Block::minecraft_warped_nylium:
	case Block::minecraft_warped_wart_block:
	case Block::minecraft_crimson_stem:
	case Block::minecraft_stripped_crimson_stem:
	case Block::minecraft_crimson_hyphae:
	case Block::minecraft_stripped_crimson_hyphae:
	case Block::minecraft_crimson_nylium:
	case Block::minecraft_shroomlight:
	case Block::minecraft_crimson_planks:
	case Block::minecraft_warped_planks:
	case Block::minecraft_structure_block:
	case Block::minecraft_jigsaw:
	case Block::minecraft_target:
	case Block::minecraft_bee_nest:
	case Block::minecraft_beehive:
	case Block::minecraft_honeycomb_block:
	case Block::minecraft_netherite_block:
	case Block::minecraft_ancient_debris:
	case Block::minecraft_crying_obsidian:
	case Block::minecraft_respawn_anchor:
	case Block::minecraft_lodestone:
	case Block::minecraft_blackstone:
	case Block::minecraft_polished_blackstone:
	case Block::minecraft_polished_blackstone_bricks:
	case Block::minecraft_cracked_polished_blackstone_bricks:
	case Block::minecraft_chiseled_polished_blackstone:
	case Block::minecraft_gilded_blackstone:
	case Block::minecraft_chiseled_nether_bricks:
	case Block::minecraft_cracked_nether_bricks:
	case Block::minecraft_quartz_bricks:
	case Block::minecraft_amethyst_block:
	case Block::minecraft_budding_amethyst:
	case Block::minecraft_tuff:
	case Block::minecraft_calcite:
	case Block::minecraft_tinted_glass:
	case Block::minecraft_oxidized_copper:
	case Block::minecraft_weathered_copper:
	case Block::minecraft_exposed_copper:
	case Block::minecraft_copper_block:
	case Block::minecraft_copper_ore:
	case Block::minecraft_deepslate_copper_ore:
	case Block::minecraft_oxidized_cut_copper:
	case Block::minecraft_weathered_cut_copper:
	case Block::minecraft_exposed_cut_copper:
	case Block::minecraft_cut_copper:
	case Block::minecraft_waxed_copper_block:
	case Block::minecraft_waxed_weathered_copper:
	case Block::minecraft_waxed_exposed_copper:
	case Block::minecraft_waxed_oxidized_copper:
	case Block::minecraft_waxed_oxidized_cut_copper:
	case Block::minecraft_waxed_weathered_cut_copper:
	case Block::minecraft_waxed_exposed_cut_copper:
	case Block::minecraft_waxed_cut_copper:
	case Block::minecraft_moss_block:
	case Block::minecraft_rooted_dirt:
	case Block::minecraft_deepslate:
	case Block::minecraft_cobbled_deepslate:
	case Block::minecraft_polished_deepslate:
	case Block::minecraft_deepslate_bricks:
	case Block::minecraft_chiseled_deepslate:
	case Block::minecraft_cracked_deepslate_bricks:
	case Block::minecraft_cracked_deepslate_tiles:
	case Block::minecraft_infested_deepslate:
	case Block::minecraft_smooth_basalt:
	case Block::minecraft_raw_iron_block:
	case Block::minecraft_raw_copper_block:
	case Block::minecraft_raw_gold_block:
	case Block::minecraft_deepslate_tiles:
	case Block::minecraft_dripstone_block:
		return true;

		//non-full solid blocks
	//case Block::minecraft_air:
	//case Block::minecraft_oak_sapling:
	//case Block::minecraft_spruce_sapling:
	//case Block::minecraft_birch_sapling:
	//case Block::minecraft_jungle_sapling:
	//case Block::minecraft_acacia_sapling:
	//case Block::minecraft_dark_oak_sapling:
	//case Block::minecraft_water:
	//case Block::minecraft_lava:
	//case Block::minecraft_oak_leaves:
	//case Block::minecraft_spruce_leaves:
	//case Block::minecraft_birch_leaves:
	//case Block::minecraft_jungle_leaves:
	//case Block::minecraft_acacia_leaves:
	//case Block::minecraft_dark_oak_leaves:
	//case Block::minecraft_azalea_leaves:
	//case Block::minecraft_flowering_azalea_leaves:
	//case Block::minecraft_white_bed:
	//case Block::minecraft_orange_bed:
	//case Block::minecraft_magenta_bed:
	//case Block::minecraft_light_blue_bed:
	//case Block::minecraft_yellow_bed:
	//case Block::minecraft_lime_bed:
	//case Block::minecraft_pink_bed:
	//case Block::minecraft_gray_bed:
	//case Block::minecraft_light_gray_bed:
	//case Block::minecraft_cyan_bed:
	//case Block::minecraft_purple_bed:
	//case Block::minecraft_blue_bed:
	//case Block::minecraft_brown_bed:
	//case Block::minecraft_green_bed:
	//case Block::minecraft_red_bed:
	//case Block::minecraft_black_bed:
	//case Block::minecraft_powered_rail:
	//case Block::minecraft_detector_rail:
	//case Block::minecraft_cobweb:
	//case Block::minecraft_grass:
	//case Block::minecraft_fern:
	//case Block::minecraft_dead_bush:
	//case Block::minecraft_seagrass:
	//case Block::minecraft_tall_seagrass:
	//case Block::minecraft_piston_head:
	//case Block::minecraft_moving_piston:
	//case Block::minecraft_dandelion:
	//case Block::minecraft_poppy:
	//case Block::minecraft_blue_orchid:
	//case Block::minecraft_allium:
	//case Block::minecraft_azure_bluet:
	//case Block::minecraft_red_tulip:
	//case Block::minecraft_orange_tulip:
	//case Block::minecraft_white_tulip:
	//case Block::minecraft_pink_tulip:
	//case Block::minecraft_oxeye_daisy:
	//case Block::minecraft_cornflower:
	//case Block::minecraft_wither_rose:
	//case Block::minecraft_lily_of_the_valley:
	//case Block::minecraft_brown_mushroom:
	//case Block::minecraft_red_mushroom:
	//case Block::minecraft_torch:
	//case Block::minecraft_wall_torch:
	//case Block::minecraft_fire:
	//case Block::minecraft_soul_fire:
	//case Block::minecraft_oak_stairs:
	//case Block::minecraft_chest:
	//case Block::minecraft_redstone_wire:
	//case Block::minecraft_wheat:
	//case Block::minecraft_oak_sign:
	//case Block::minecraft_spruce_sign:
	//case Block::minecraft_birch_sign:
	//case Block::minecraft_acacia_sign:
	//case Block::minecraft_jungle_sign:
	//case Block::minecraft_dark_oak_sign:
	//case Block::minecraft_oak_door:
	//case Block::minecraft_ladder:
	//case Block::minecraft_rail:
	//case Block::minecraft_cobblestone_stairs:
	//case Block::minecraft_oak_wall_sign:
	//case Block::minecraft_spruce_wall_sign:
	//case Block::minecraft_birch_wall_sign:
	//case Block::minecraft_acacia_wall_sign:
	//case Block::minecraft_jungle_wall_sign:
	//case Block::minecraft_dark_oak_wall_sign:
	//case Block::minecraft_lever:
	//case Block::minecraft_stone_pressure_plate:
	//case Block::minecraft_iron_door:
	//case Block::minecraft_oak_pressure_plate:
	//case Block::minecraft_spruce_pressure_plate:
	//case Block::minecraft_birch_pressure_plate:
	//case Block::minecraft_jungle_pressure_plate:
	//case Block::minecraft_acacia_pressure_plate:
	//case Block::minecraft_dark_oak_pressure_plate:
	//case Block::minecraft_redstone_torch:
	//case Block::minecraft_redstone_wall_torch:
	//case Block::minecraft_stone_button:
	//case Block::minecraft_snow:
	//case Block::minecraft_ice:
	//case Block::minecraft_cactus:
	//case Block::minecraft_sugar_cane:
	//case Block::minecraft_oak_fence:
	//case Block::minecraft_pumpkin:
	//case Block::minecraft_soul_torch:
	//case Block::minecraft_soul_wall_torch:
	//case Block::minecraft_nether_portal:
	//case Block::minecraft_carved_pumpkin:
	//case Block::minecraft_jack_o_lantern:
	//case Block::minecraft_cake:
	//case Block::minecraft_repeater:
	//case Block::minecraft_oak_trapdoor:
	//case Block::minecraft_spruce_trapdoor:
	//case Block::minecraft_birch_trapdoor:
	//case Block::minecraft_jungle_trapdoor:
	//case Block::minecraft_acacia_trapdoor:
	//case Block::minecraft_dark_oak_trapdoor:
	//case Block::minecraft_iron_bars:
	//case Block::minecraft_chain:
	//case Block::minecraft_glass_pane:
	//case Block::minecraft_melon:
	//case Block::minecraft_attached_pumpkin_stem:
	//case Block::minecraft_attached_melon_stem:
	//case Block::minecraft_pumpkin_stem:
	//case Block::minecraft_melon_stem:
	//case Block::minecraft_vine:
	//case Block::minecraft_glow_lichen:
	//case Block::minecraft_oak_fence_gate:
	//case Block::minecraft_brick_stairs:
	//case Block::minecraft_stone_brick_stairs:
	//case Block::minecraft_lily_pad:
	//case Block::minecraft_nether_brick_fence:
	//case Block::minecraft_nether_brick_stairs:
	//case Block::minecraft_nether_wart:
	//case Block::minecraft_enchanting_table:
	//case Block::minecraft_brewing_stand:
	//case Block::minecraft_cauldron:
	//case Block::minecraft_water_cauldron:
	//case Block::minecraft_lava_cauldron:
	//case Block::minecraft_powder_snow_cauldron:
	//case Block::minecraft_end_portal:
	//case Block::minecraft_end_portal_frame:
	//case Block::minecraft_dragon_egg:
	//case Block::minecraft_cocoa:
	//case Block::minecraft_sandstone_stairs:
	//case Block::minecraft_ender_chest:
	//case Block::minecraft_tripwire_hook:
	//case Block::minecraft_tripwire:
	//case Block::minecraft_spruce_stairs:
	//case Block::minecraft_birch_stairs:
	//case Block::minecraft_jungle_stairs:
	//case Block::minecraft_cobblestone_wall:
	//case Block::minecraft_mossy_cobblestone_wall:
	//case Block::minecraft_flower_pot:
	//case Block::minecraft_potted_oak_sapling:
	//case Block::minecraft_potted_spruce_sapling:
	//case Block::minecraft_potted_birch_sapling:
	//case Block::minecraft_potted_jungle_sapling:
	//case Block::minecraft_potted_acacia_sapling:
	//case Block::minecraft_potted_dark_oak_sapling:
	//case Block::minecraft_potted_fern:
	//case Block::minecraft_potted_dandelion:
	//case Block::minecraft_potted_poppy:
	//case Block::minecraft_potted_blue_orchid:
	//case Block::minecraft_potted_allium:
	//case Block::minecraft_potted_azure_bluet:
	//case Block::minecraft_potted_red_tulip:
	//case Block::minecraft_potted_orange_tulip:
	//case Block::minecraft_potted_white_tulip:
	//case Block::minecraft_potted_pink_tulip:
	//case Block::minecraft_potted_oxeye_daisy:
	//case Block::minecraft_potted_cornflower:
	//case Block::minecraft_potted_lily_of_the_valley:
	//case Block::minecraft_potted_wither_rose:
	//case Block::minecraft_potted_red_mushroom:
	//case Block::minecraft_potted_brown_mushroom:
	//case Block::minecraft_potted_dead_bush:
	//case Block::minecraft_potted_cactus:
	//case Block::minecraft_carrots:
	//case Block::minecraft_potatoes:
	//case Block::minecraft_oak_button:
	//case Block::minecraft_spruce_button:
	//case Block::minecraft_birch_button:
	//case Block::minecraft_jungle_button:
	//case Block::minecraft_acacia_button:
	//case Block::minecraft_dark_oak_button:
	//case Block::minecraft_skeleton_skull:
	//case Block::minecraft_skeleton_wall_skull:
	//case Block::minecraft_wither_skeleton_skull:
	//case Block::minecraft_wither_skeleton_wall_skull:
	//case Block::minecraft_zombie_head:
	//case Block::minecraft_zombie_wall_head:
	//case Block::minecraft_player_head:
	//case Block::minecraft_player_wall_head:
	//case Block::minecraft_creeper_head:
	//case Block::minecraft_creeper_wall_head:
	//case Block::minecraft_dragon_head:
	//case Block::minecraft_dragon_wall_head:
	//case Block::minecraft_anvil:
	//case Block::minecraft_chipped_anvil:
	//case Block::minecraft_damaged_anvil:
	//case Block::minecraft_trapped_chest:
	//case Block::minecraft_light_weighted_pressure_plate:
	//case Block::minecraft_heavy_weighted_pressure_plate:
	//case Block::minecraft_comparator:
	//case Block::minecraft_daylight_detector:
	//case Block::minecraft_hopper: //?
	//case Block::minecraft_quartz_stairs:
	//case Block::minecraft_activator_rail:
	//case Block::minecraft_white_stained_glass_pane:
	//case Block::minecraft_orange_stained_glass_pane:
	//case Block::minecraft_magenta_stained_glass_pane:
	//case Block::minecraft_light_blue_stained_glass_pane:
	//case Block::minecraft_yellow_stained_glass_pane:
	//case Block::minecraft_lime_stained_glass_pane:
	//case Block::minecraft_pink_stained_glass_pane:
	//case Block::minecraft_gray_stained_glass_pane:
	//case Block::minecraft_light_gray_stained_glass_pane:
	//case Block::minecraft_cyan_stained_glass_pane:
	//case Block::minecraft_purple_stained_glass_pane:
	//case Block::minecraft_blue_stained_glass_pane:
	//case Block::minecraft_brown_stained_glass_pane:
	//case Block::minecraft_green_stained_glass_pane:
	//case Block::minecraft_red_stained_glass_pane:
	//case Block::minecraft_black_stained_glass_pane:
	//case Block::minecraft_acacia_stairs:
	//case Block::minecraft_dark_oak_stairs:
	//case Block::minecraft_barrier:
	//case Block::minecraft_light:
	//case Block::minecraft_iron_trapdoor:
	//case Block::minecraft_prismarine_stairs:
	//case Block::minecraft_prismarine_brick_stairs:
	//case Block::minecraft_dark_prismarine_stairs:
	//case Block::minecraft_prismarine_slab:
	//case Block::minecraft_prismarine_brick_slab:
	//case Block::minecraft_dark_prismarine_slab:
	//case Block::minecraft_white_carpet:
	//case Block::minecraft_orange_carpet:
	//case Block::minecraft_magenta_carpet:
	//case Block::minecraft_light_blue_carpet:
	//case Block::minecraft_yellow_carpet:
	//case Block::minecraft_lime_carpet:
	//case Block::minecraft_pink_carpet:
	//case Block::minecraft_gray_carpet:
	//case Block::minecraft_light_gray_carpet:
	//case Block::minecraft_cyan_carpet:
	//case Block::minecraft_purple_carpet:
	//case Block::minecraft_blue_carpet:
	//case Block::minecraft_brown_carpet:
	//case Block::minecraft_green_carpet:
	//case Block::minecraft_red_carpet:
	//case Block::minecraft_black_carpet:
	//case Block::minecraft_packed_ice: //?
	//case Block::minecraft_sunflower:
	//case Block::minecraft_lilac:
	//case Block::minecraft_rose_bush:
	//case Block::minecraft_peony:
	//case Block::minecraft_tall_grass:
	//case Block::minecraft_large_fern:
	//case Block::minecraft_white_banner:
	//case Block::minecraft_orange_banner:
	//case Block::minecraft_magenta_banner:
	//case Block::minecraft_light_blue_banner:
	//case Block::minecraft_yellow_banner:
	//case Block::minecraft_lime_banner:
	//case Block::minecraft_pink_banner:
	//case Block::minecraft_gray_banner:
	//case Block::minecraft_light_gray_banner:
	//case Block::minecraft_cyan_banner:
	//case Block::minecraft_purple_banner:
	//case Block::minecraft_blue_banner:
	//case Block::minecraft_brown_banner:
	//case Block::minecraft_green_banner:
	//case Block::minecraft_red_banner:
	//case Block::minecraft_black_banner:
	//case Block::minecraft_white_wall_banner:
	//case Block::minecraft_orange_wall_banner:
	//case Block::minecraft_magenta_wall_banner:
	//case Block::minecraft_light_blue_wall_banner:
	//case Block::minecraft_yellow_wall_banner:
	//case Block::minecraft_lime_wall_banner:
	//case Block::minecraft_pink_wall_banner:
	//case Block::minecraft_gray_wall_banner:
	//case Block::minecraft_light_gray_wall_banner:
	//case Block::minecraft_cyan_wall_banner:
	//case Block::minecraft_purple_wall_banner:
	//case Block::minecraft_blue_wall_banner:
	//case Block::minecraft_brown_wall_banner:
	//case Block::minecraft_green_wall_banner:
	//case Block::minecraft_red_wall_banner:
	//case Block::minecraft_black_wall_banner:
	//case Block::minecraft_red_sandstone_stairs:
	//case Block::minecraft_oak_slab:
	//case Block::minecraft_spruce_slab:
	//case Block::minecraft_birch_slab:
	//case Block::minecraft_jungle_slab:
	//case Block::minecraft_acacia_slab:
	//case Block::minecraft_dark_oak_slab:
	//case Block::minecraft_stone_slab:
	//case Block::minecraft_smooth_stone_slab:
	//case Block::minecraft_sandstone_slab:
	//case Block::minecraft_cut_sandstone_slab:
	//case Block::minecraft_petrified_oak_slab:
	//case Block::minecraft_cobblestone_slab:
	//case Block::minecraft_brick_slab:
	//case Block::minecraft_stone_brick_slab:
	//case Block::minecraft_nether_brick_slab:
	//case Block::minecraft_quartz_slab:
	//case Block::minecraft_red_sandstone_slab:
	//case Block::minecraft_cut_red_sandstone_slab:
	//case Block::minecraft_purpur_slab:
	//case Block::minecraft_spruce_fence_gate:
	//case Block::minecraft_birch_fence_gate:
	//case Block::minecraft_jungle_fence_gate:
	//case Block::minecraft_acacia_fence_gate:
	//case Block::minecraft_dark_oak_fence_gate:
	//case Block::minecraft_spruce_fence:
	//case Block::minecraft_birch_fence:
	//case Block::minecraft_jungle_fence:
	//case Block::minecraft_acacia_fence:
	//case Block::minecraft_dark_oak_fence:
	//case Block::minecraft_spruce_door:
	//case Block::minecraft_birch_door:
	//case Block::minecraft_jungle_door:
	//case Block::minecraft_acacia_door:
	//case Block::minecraft_dark_oak_door:
	//case Block::minecraft_end_rod:
	//case Block::minecraft_chorus_plant: //?
	//case Block::minecraft_chorus_flower: //?
	//case Block::minecraft_purpur_stairs:
	//case Block::minecraft_beetroots:
	//case Block::minecraft_end_gateway:
	//case Block::minecraft_frosted_ice: //?
	//case Block::minecraft_structure_void:
	//case Block::minecraft_shulker_box:
	//case Block::minecraft_white_shulker_box:
	//case Block::minecraft_orange_shulker_box:
	//case Block::minecraft_magenta_shulker_box:
	//case Block::minecraft_light_blue_shulker_box:
	//case Block::minecraft_yellow_shulker_box:
	//case Block::minecraft_lime_shulker_box:
	//case Block::minecraft_pink_shulker_box:
	//case Block::minecraft_gray_shulker_box:
	//case Block::minecraft_light_gray_shulker_box:
	//case Block::minecraft_cyan_shulker_box:
	//case Block::minecraft_purple_shulker_box:
	//case Block::minecraft_blue_shulker_box:
	//case Block::minecraft_brown_shulker_box:
	//case Block::minecraft_green_shulker_box:
	//case Block::minecraft_red_shulker_box:
	//case Block::minecraft_black_shulker_box:
	//case Block::minecraft_kelp:
	//case Block::minecraft_kelp_plant:
	//case Block::minecraft_turtle_egg:
	//case Block::minecraft_dead_tube_coral:
	//case Block::minecraft_dead_brain_coral:
	//case Block::minecraft_dead_bubble_coral:
	//case Block::minecraft_dead_fire_coral:
	//case Block::minecraft_dead_horn_coral:
	//case Block::minecraft_tube_coral:
	//case Block::minecraft_brain_coral:
	//case Block::minecraft_bubble_coral:
	//case Block::minecraft_fire_coral:
	//case Block::minecraft_horn_coral:
	//case Block::minecraft_dead_tube_coral_fan:
	//case Block::minecraft_dead_brain_coral_fan:
	//case Block::minecraft_dead_bubble_coral_fan:
	//case Block::minecraft_dead_fire_coral_fan:
	//case Block::minecraft_dead_horn_coral_fan:
	//case Block::minecraft_tube_coral_fan:
	//case Block::minecraft_brain_coral_fan:
	//case Block::minecraft_bubble_coral_fan:
	//case Block::minecraft_fire_coral_fan:
	//case Block::minecraft_horn_coral_fan:
	//case Block::minecraft_dead_tube_coral_wall_fan:
	//case Block::minecraft_dead_brain_coral_wall_fan:
	//case Block::minecraft_dead_bubble_coral_wall_fan:
	//case Block::minecraft_dead_fire_coral_wall_fan:
	//case Block::minecraft_dead_horn_coral_wall_fan:
	//case Block::minecraft_tube_coral_wall_fan:
	//case Block::minecraft_brain_coral_wall_fan:
	//case Block::minecraft_bubble_coral_wall_fan:
	//case Block::minecraft_fire_coral_wall_fan:
	//case Block::minecraft_horn_coral_wall_fan:
	//case Block::minecraft_sea_pickle:
	//case Block::minecraft_conduit:
	//case Block::minecraft_bamboo_sapling:
	//case Block::minecraft_bamboo:
	//case Block::minecraft_potted_bamboo:
	//case Block::minecraft_void_air:
	//case Block::minecraft_cave_air:
	//case Block::minecraft_bubble_column:
	//case Block::minecraft_polished_granite_stairs:
	//case Block::minecraft_smooth_red_sandstone_stairs:
	//case Block::minecraft_mossy_stone_brick_stairs:
	//case Block::minecraft_polished_diorite_stairs:
	//case Block::minecraft_mossy_cobblestone_stairs:
	//case Block::minecraft_end_stone_brick_stairs:
	//case Block::minecraft_stone_stairs:
	//case Block::minecraft_smooth_sandstone_stairs:
	//case Block::minecraft_smooth_quartz_stairs:
	//case Block::minecraft_granite_stairs:
	//case Block::minecraft_andesite_stairs:
	//case Block::minecraft_red_nether_brick_stairs:
	//case Block::minecraft_polished_andesite_stairs:
	//case Block::minecraft_diorite_stairs:
	//case Block::minecraft_polished_granite_slab:
	//case Block::minecraft_smooth_red_sandstone_slab:
	//case Block::minecraft_mossy_stone_brick_slab:
	//case Block::minecraft_polished_diorite_slab:
	//case Block::minecraft_mossy_cobblestone_slab:
	//case Block::minecraft_end_stone_brick_slab:
	//case Block::minecraft_smooth_sandstone_slab:
	//case Block::minecraft_smooth_quartz_slab:
	//case Block::minecraft_granite_slab:
	//case Block::minecraft_andesite_slab:
	//case Block::minecraft_red_nether_brick_slab:
	//case Block::minecraft_polished_andesite_slab:
	//case Block::minecraft_diorite_slab:
	//case Block::minecraft_brick_wall:
	//case Block::minecraft_prismarine_wall:
	//case Block::minecraft_red_sandstone_wall:
	//case Block::minecraft_mossy_stone_brick_wall:
	//case Block::minecraft_granite_wall:
	//case Block::minecraft_stone_brick_wall:
	//case Block::minecraft_nether_brick_wall:
	//case Block::minecraft_andesite_wall:
	//case Block::minecraft_red_nether_brick_wall:
	//case Block::minecraft_sandstone_wall:
	//case Block::minecraft_end_stone_brick_wall:
	//case Block::minecraft_diorite_wall:
	//case Block::minecraft_scaffolding:
	//case Block::minecraft_grindstone:
	//case Block::minecraft_lectern:
	//case Block::minecraft_stonecutter:
	//case Block::minecraft_bell:
	//case Block::minecraft_lantern:
	//case Block::minecraft_soul_lantern:
	//case Block::minecraft_campfire:
	//case Block::minecraft_soul_campfire:
	//case Block::minecraft_sweet_berry_bush:
	//case Block::minecraft_warped_fungus:
	//case Block::minecraft_warped_roots:
	//case Block::minecraft_nether_sprouts:
	//case Block::minecraft_crimson_fungus:
	//case Block::minecraft_weeping_vines:
	//case Block::minecraft_weeping_vines_plant:
	//case Block::minecraft_twisting_vines:
	//case Block::minecraft_twisting_vines_plant:
	//case Block::minecraft_crimson_roots:
	//case Block::minecraft_crimson_slab:
	//case Block::minecraft_warped_slab:
	//case Block::minecraft_crimson_pressure_plate:
	//case Block::minecraft_warped_pressure_plate:
	//case Block::minecraft_crimson_fence:
	//case Block::minecraft_warped_fence:
	//case Block::minecraft_crimson_trapdoor:
	//case Block::minecraft_warped_trapdoor:
	//case Block::minecraft_crimson_fence_gate:
	//case Block::minecraft_warped_fence_gate:
	//case Block::minecraft_crimson_stairs:
	//case Block::minecraft_warped_stairs:
	//case Block::minecraft_crimson_button:
	//case Block::minecraft_warped_button:
	//case Block::minecraft_crimson_door:
	//case Block::minecraft_warped_door:
	//case Block::minecraft_crimson_sign:
	//case Block::minecraft_warped_sign:
	//case Block::minecraft_crimson_wall_sign:
	//case Block::minecraft_warped_wall_sign:
	//case Block::minecraft_composter:
	//case Block::minecraft_honey_block:
	//case Block::minecraft_potted_crimson_fungus:
	//case Block::minecraft_potted_warped_fungus:
	//case Block::minecraft_potted_crimson_roots:
	//case Block::minecraft_potted_warped_roots:
	//case Block::minecraft_blackstone_stairs:
	//case Block::minecraft_blackstone_wall:
	//case Block::minecraft_blackstone_slab:
	//case Block::minecraft_polished_blackstone_brick_slab:
	//case Block::minecraft_polished_blackstone_brick_stairs:
	//case Block::minecraft_polished_blackstone_brick_wall:
	//case Block::minecraft_polished_blackstone_stairs:
	//case Block::minecraft_polished_blackstone_slab:
	//case Block::minecraft_polished_blackstone_pressure_plate:
	//case Block::minecraft_polished_blackstone_button:
	//case Block::minecraft_polished_blackstone_wall:
	//case Block::minecraft_candle:
	//case Block::minecraft_white_candle:
	//case Block::minecraft_orange_candle:
	//case Block::minecraft_magenta_candle:
	//case Block::minecraft_light_blue_candle:
	//case Block::minecraft_yellow_candle:
	//case Block::minecraft_lime_candle:
	//case Block::minecraft_pink_candle:
	//case Block::minecraft_gray_candle:
	//case Block::minecraft_light_gray_candle:
	//case Block::minecraft_cyan_candle:
	//case Block::minecraft_purple_candle:
	//case Block::minecraft_blue_candle:
	//case Block::minecraft_brown_candle:
	//case Block::minecraft_green_candle:
	//case Block::minecraft_red_candle:
	//case Block::minecraft_black_candle:
	//case Block::minecraft_candle_cake:
	//case Block::minecraft_white_candle_cake:
	//case Block::minecraft_orange_candle_cake:
	//case Block::minecraft_magenta_candle_cake:
	//case Block::minecraft_light_blue_candle_cake:
	//case Block::minecraft_yellow_candle_cake:
	//case Block::minecraft_lime_candle_cake:
	//case Block::minecraft_pink_candle_cake:
	//case Block::minecraft_gray_candle_cake:
	//case Block::minecraft_light_gray_candle_cake:
	//case Block::minecraft_cyan_candle_cake:
	//case Block::minecraft_purple_candle_cake:
	//case Block::minecraft_blue_candle_cake:
	//case Block::minecraft_brown_candle_cake:
	//case Block::minecraft_green_candle_cake:
	//case Block::minecraft_red_candle_cake:
	//case Block::minecraft_black_candle_cake:
	//case Block::minecraft_amethyst_cluster:
	//case Block::minecraft_large_amethyst_bud:
	//case Block::minecraft_medium_amethyst_bud:
	//case Block::minecraft_small_amethyst_bud:
	//case Block::minecraft_powder_snow:
	//case Block::minecraft_sculk_sensor:
	//case Block::minecraft_oxidized_cut_copper_stairs:
	//case Block::minecraft_weathered_cut_copper_stairs:
	//case Block::minecraft_exposed_cut_copper_stairs:
	//case Block::minecraft_cut_copper_stairs:
	//case Block::minecraft_oxidized_cut_copper_slab:
	//case Block::minecraft_weathered_cut_copper_slab:
	//case Block::minecraft_exposed_cut_copper_slab:
	//case Block::minecraft_cut_copper_slab:
	//case Block::minecraft_waxed_oxidized_cut_copper_stairs:
	//case Block::minecraft_waxed_weathered_cut_copper_stairs:
	//case Block::minecraft_waxed_exposed_cut_copper_stairs:
	//case Block::minecraft_waxed_cut_copper_stairs:
	//case Block::minecraft_waxed_oxidized_cut_copper_slab:
	//case Block::minecraft_waxed_weathered_cut_copper_slab:
	//case Block::minecraft_waxed_exposed_cut_copper_slab:
	//case Block::minecraft_waxed_cut_copper_slab:
	//case Block::minecraft_lightning_rod:
	//case Block::minecraft_pointed_dripstone:
	//case Block::minecraft_cave_vines:
	//case Block::minecraft_cave_vines_plant:
	//case Block::minecraft_spore_blossom:
	//case Block::minecraft_azalea:
	//case Block::minecraft_flowering_azalea:
	//case Block::minecraft_moss_carpet:
	//case Block::minecraft_big_dripleaf:
	//case Block::minecraft_big_dripleaf_stem:
	//case Block::minecraft_small_dripleaf:
	//case Block::minecraft_hanging_roots:
	//case Block::minecraft_cobbled_deepslate_stairs:
	//case Block::minecraft_cobbled_deepslate_slab:
	//case Block::minecraft_cobbled_deepslate_wall:
	//case Block::minecraft_polished_deepslate_stairs:
	//case Block::minecraft_polished_deepslate_slab:
	//case Block::minecraft_polished_deepslate_wall:
	//case Block::minecraft_deepslate_tile_stairs:
	//case Block::minecraft_deepslate_tile_slab:
	//case Block::minecraft_deepslate_tile_wall:
	//case Block::minecraft_deepslate_brick_stairs:
	//case Block::minecraft_deepslate_brick_slab:
	//case Block::minecraft_potted_flowering_azalea_bush:
	//case Block::minecraft_deepslate_brick_wall:
	//case Block::minecraft_potted_azalea_bush:
	}
	return false;
}
//does
bool woodenFencesConnectible(Block id, playerDigging::face, const BlockState state)
{
	if (fullSolidBlock(id)) return true;
}
bool netherBricksConnectible(Block id, playerDigging::face, const BlockState state)
{
	//switch (id)
	if (fullSolidBlock(id)) return true;
}
bool wallsConnectible(Block id, playerDigging::face, const BlockState state)
{
	if (fullSolidBlock(id)) return true;
}
bool canSupportTorch(Block id, playerDigging::face, const BlockState state)
{
	if (fullSolidBlock(id)) return true;
	switch (id)
	{
	case Block::minecraft_pumpkin:
	case Block::minecraft_carved_pumpkin:
	case Block::minecraft_jack_o_lantern:
		return true;
	}
	return false;
}
bool glassPaneConnectible(Block id, playerDigging::face, const BlockState state)
{
	if (fullSolidBlock(id)) return true;
}

bool isDoorLower(Block id, const BlockState& state)
{
	switch (id)
	{
	case Block::minecraft_iron_door:
	case Block::minecraft_oak_door:
	case Block::minecraft_spruce_door:
	case Block::minecraft_birch_door:
	case Block::minecraft_jungle_door:
	case Block::minecraft_acacia_door:
	case Block::minecraft_dark_oak_door:
	case Block::minecraft_crimson_door:
	case Block::minecraft_warped_door:
		if (state.getState("half") == "lower") return true;
	}
	return false;
}
//does a door hinge attach to this block?
bool canSupportDoorHinge(Block id)
{
	if (fullSolidBlock(id)) return true;
	switch (id)
	{
	case Block::minecraft_oak_leaves:
	case Block::minecraft_spruce_leaves:
	case Block::minecraft_birch_leaves:
	case Block::minecraft_jungle_leaves:
	case Block::minecraft_acacia_leaves:
	case Block::minecraft_dark_oak_leaves:
	case Block::minecraft_azalea_leaves:
	case Block::minecraft_flowering_azalea_leaves:
	case Block::minecraft_ice:
	case Block::minecraft_pumpkin:
	case Block::minecraft_carved_pumpkin:
	case Block::minecraft_jack_o_lantern:
	case Block::minecraft_melon:
	case Block::minecraft_barrier:
	case Block::minecraft_packed_ice:
	case Block::minecraft_chorus_flower:
	case Block::minecraft_frosted_ice:
	case Block::minecraft_shulker_box:
	case Block::minecraft_white_shulker_box:
	case Block::minecraft_orange_shulker_box:
	case Block::minecraft_magenta_shulker_box:
	case Block::minecraft_light_blue_shulker_box:
	case Block::minecraft_yellow_shulker_box:
	case Block::minecraft_lime_shulker_box:
	case Block::minecraft_pink_shulker_box:
	case Block::minecraft_gray_shulker_box:
	case Block::minecraft_light_gray_shulker_box:
	case Block::minecraft_cyan_shulker_box:
	case Block::minecraft_purple_shulker_box:
	case Block::minecraft_blue_shulker_box:
	case Block::minecraft_brown_shulker_box:
	case Block::minecraft_green_shulker_box:
	case Block::minecraft_red_shulker_box:
	case Block::minecraft_black_shulker_box:
		return true;
	}
	return false;
}
//can a door be placed on the top of this block?
bool canSupportDoor(Block id, const BlockState& state)
{
	if (fullSolidBlock(id)) return true;
	switch (id)
	{
	case Block::minecraft_ice:
	case Block::minecraft_packed_ice:
	case Block::minecraft_blue_ice:
	case Block::minecraft_pumpkin:
	case Block::minecraft_carved_pumpkin:
	case Block::minecraft_jack_o_lantern:
	case Block::minecraft_melon:
	case Block::minecraft_barrier:
	case Block::minecraft_chorus_flower:
	case Block::minecraft_frosted_ice:
	case Block::minecraft_shulker_box:
	case Block::minecraft_white_shulker_box:
	case Block::minecraft_orange_shulker_box:
	case Block::minecraft_magenta_shulker_box:
	case Block::minecraft_light_blue_shulker_box:
	case Block::minecraft_yellow_shulker_box:
	case Block::minecraft_lime_shulker_box:
	case Block::minecraft_pink_shulker_box:
	case Block::minecraft_gray_shulker_box:
	case Block::minecraft_light_gray_shulker_box:
	case Block::minecraft_cyan_shulker_box:
	case Block::minecraft_purple_shulker_box:
	case Block::minecraft_blue_shulker_box:
	case Block::minecraft_brown_shulker_box:
	case Block::minecraft_green_shulker_box:
	case Block::minecraft_red_shulker_box:
	case Block::minecraft_black_shulker_box:
	case Block::minecraft_scaffolding:
	case Block::minecraft_azalea:
	case Block::minecraft_flowering_azalea:
		return true;

	case Block::minecraft_piston_head:
		if (state.getState("facing") == "up") return true;
		return false;

	case Block::minecraft_snow:
		if (state.getState("layers") == "8") return true;
		return false;

	case Block::minecraft_crimson_trapdoor:
	case Block::minecraft_warped_trapdoor:
	case Block::minecraft_oak_trapdoor:
	case Block::minecraft_spruce_trapdoor:
	case Block::minecraft_birch_trapdoor:
	case Block::minecraft_jungle_trapdoor:
	case Block::minecraft_acacia_trapdoor:
	case Block::minecraft_dark_oak_trapdoor:
	case Block::minecraft_iron_trapdoor:
		if (state.getState("half") == "top" && state.getState("open") == "false") return true;
		return false;

	case Block::minecraft_prismarine_slab:
	case Block::minecraft_prismarine_brick_slab:
	case Block::minecraft_dark_prismarine_slab:
	case Block::minecraft_oak_slab:
	case Block::minecraft_spruce_slab:
	case Block::minecraft_birch_slab:
	case Block::minecraft_jungle_slab:
	case Block::minecraft_acacia_slab:
	case Block::minecraft_dark_oak_slab:
	case Block::minecraft_stone_slab:
	case Block::minecraft_smooth_stone_slab:
	case Block::minecraft_sandstone_slab:
	case Block::minecraft_cut_sandstone_slab:
	case Block::minecraft_petrified_oak_slab:
	case Block::minecraft_cobblestone_slab:
	case Block::minecraft_brick_slab:
	case Block::minecraft_stone_brick_slab:
	case Block::minecraft_nether_brick_slab:
	case Block::minecraft_quartz_slab:
	case Block::minecraft_red_sandstone_slab:
	case Block::minecraft_cut_red_sandstone_slab:
	case Block::minecraft_purpur_slab:
	case Block::minecraft_cobbled_deepslate_slab:
	case Block::minecraft_polished_deepslate_slab:
	case Block::minecraft_deepslate_tile_slab:
	case Block::minecraft_deepslate_brick_slab:
	case Block::minecraft_blackstone_slab:
	case Block::minecraft_polished_blackstone_brick_slab:
	case Block::minecraft_polished_blackstone_slab:
	case Block::minecraft_oxidized_cut_copper_slab:
	case Block::minecraft_weathered_cut_copper_slab:
	case Block::minecraft_exposed_cut_copper_slab:
	case Block::minecraft_cut_copper_slab:
	case Block::minecraft_waxed_oxidized_cut_copper_slab:
	case Block::minecraft_waxed_weathered_cut_copper_slab:
	case Block::minecraft_waxed_exposed_cut_copper_slab:
	case Block::minecraft_waxed_cut_copper_slab:
	case Block::minecraft_polished_granite_slab:
	case Block::minecraft_smooth_red_sandstone_slab:
	case Block::minecraft_mossy_stone_brick_slab:
	case Block::minecraft_polished_diorite_slab:
	case Block::minecraft_mossy_cobblestone_slab:
	case Block::minecraft_end_stone_brick_slab:
	case Block::minecraft_smooth_sandstone_slab:
	case Block::minecraft_smooth_quartz_slab:
	case Block::minecraft_granite_slab:
	case Block::minecraft_andesite_slab:
	case Block::minecraft_red_nether_brick_slab:
	case Block::minecraft_polished_andesite_slab:
	case Block::minecraft_diorite_slab:
	case Block::minecraft_crimson_slab:
	case Block::minecraft_warped_slab:
		if (state.getState("type") == "bottom") return false;
		return true;

	case Block::minecraft_crimson_stairs:
	case Block::minecraft_warped_stairs:
	case Block::minecraft_deepslate_brick_stairs:
	case Block::minecraft_deepslate_tile_stairs:
	case Block::minecraft_polished_deepslate_stairs:
	case Block::minecraft_cobbled_deepslate_stairs:
	case Block::minecraft_waxed_oxidized_cut_copper_stairs:
	case Block::minecraft_waxed_weathered_cut_copper_stairs:
	case Block::minecraft_waxed_exposed_cut_copper_stairs:
	case Block::minecraft_waxed_cut_copper_stairs:
	case Block::minecraft_oxidized_cut_copper_stairs:
	case Block::minecraft_weathered_cut_copper_stairs:
	case Block::minecraft_exposed_cut_copper_stairs:
	case Block::minecraft_cut_copper_stairs:
	case Block::minecraft_blackstone_stairs:
	case Block::minecraft_polished_blackstone_brick_stairs:
	case Block::minecraft_polished_blackstone_stairs:
	case Block::minecraft_polished_granite_stairs:
	case Block::minecraft_smooth_red_sandstone_stairs:
	case Block::minecraft_mossy_stone_brick_stairs:
	case Block::minecraft_polished_diorite_stairs:
	case Block::minecraft_mossy_cobblestone_stairs:
	case Block::minecraft_end_stone_brick_stairs:
	case Block::minecraft_stone_stairs:
	case Block::minecraft_smooth_sandstone_stairs:
	case Block::minecraft_smooth_quartz_stairs:
	case Block::minecraft_granite_stairs:
	case Block::minecraft_andesite_stairs:
	case Block::minecraft_red_nether_brick_stairs:
	case Block::minecraft_polished_andesite_stairs:
	case Block::minecraft_diorite_stairs:
	case Block::minecraft_brick_stairs:
	case Block::minecraft_stone_brick_stairs:
	case Block::minecraft_nether_brick_stairs:
	case Block::minecraft_sandstone_stairs:
	case Block::minecraft_spruce_stairs:
	case Block::minecraft_birch_stairs:
	case Block::minecraft_jungle_stairs:
	case Block::minecraft_quartz_stairs:
	case Block::minecraft_acacia_stairs:
	case Block::minecraft_dark_oak_stairs:
	case Block::minecraft_prismarine_stairs:
	case Block::minecraft_prismarine_brick_stairs:
	case Block::minecraft_dark_prismarine_stairs:
	case Block::minecraft_red_sandstone_stairs:
	case Block::minecraft_purpur_stairs:
	case Block::minecraft_oak_stairs:
	case Block::minecraft_cobblestone_stairs:
		if (state.getState("half") == "top") return true;
		return false;

		/*case Block::minecraft_shulker_box:
		case Block::minecraft_white_shulker_box:
		case Block::minecraft_orange_shulker_box:
		case Block::minecraft_magenta_shulker_box:
		case Block::minecraft_light_blue_shulker_box:
		case Block::minecraft_yellow_shulker_box:
		case Block::minecraft_lime_shulker_box:
		case Block::minecraft_pink_shulker_box:
		case Block::minecraft_gray_shulker_box:
		case Block::minecraft_light_gray_shulker_box:
		case Block::minecraft_cyan_shulker_box:
		case Block::minecraft_purple_shulker_box:
		case Block::minecraft_blue_shulker_box:
		case Block::minecraft_brown_shulker_box:
		case Block::minecraft_green_shulker_box:
		case Block::minecraft_red_shulker_box:
		case Block::minecraft_black_shulker_box:
		case Block::minecraft_carved_pumpkin:
		case Block::minecraft_jack_o_lantern:
		case Block::minecraft_packed_ice:
		case Block::minecraft_barrier:
		case Block::minecraft_melon:
		case Block::minecraft_pumpkin:
		case Block::minecraft_ice:
			return true;
		case Block::minecraft_iron_trapdoor:
		case Block::minecraft_oak_trapdoor:
		case Block::minecraft_spruce_trapdoor:
		case Block::minecraft_birch_trapdoor:
		case Block::minecraft_jungle_trapdoor:
		case Block::minecraft_acacia_trapdoor:
		case Block::minecraft_dark_oak_trapdoor:
		case Block::minecraft_crimson_trapdoor:
		case Block::minecraft_warped_trapdoor:
			if (state.getState("half") == "top" && state.getState("open") == "false") return true;
			return false;*/
	}
	return false;
}
std::string hingeToString(doorHinge h)
{
	if (h == doorHinge::left) return "left";
	return "right";
}
doorHinge chooseDoorHinge(blockFacing facing, World* wld, int destX, int destY, int destZ, float curX, float curZ)
{
	switch (facing)
	{
	case blockFacing::north:
	{
		BlockState bottomLeftState = wld->getBlock(destX - 1, destY, destZ),
			bottomRightState = wld->getBlock(destX + 1, destY, destZ);

		Block bottomLeftBlock = stateToBlock(bottomLeftState),
			bottomRightBlock = stateToBlock(bottomRightState);

		bool leftIsDoor = isDoorLower(bottomLeftBlock, bottomLeftState),
			rightIsDoor = isDoorLower(bottomRightBlock, bottomRightState);

		if (leftIsDoor && rightIsDoor)
		{
			//doors on both sides
			//hinge dep on cursor
			return curX < .5f ? doorHinge::left : doorHinge::right;
		}
		if (leftIsDoor)
		{
			//door on left
			//hinge on right
			return doorHinge::right;
		}
		if (rightIsDoor)
		{
			//door on right
			//hinge on left
			return doorHinge::left;
		}

		BlockState topLeftState = wld->getBlock(destX - 1, destY, destZ),
			topRightState = wld->getBlock(destX + 1, destY, destZ);

		Block topLeftBlock = stateToBlock(topLeftState),
			topRightBlock = stateToBlock(topRightState);

		Byte leftWallCount = (Byte)canSupportDoorHinge(bottomLeftBlock) + canSupportDoorHinge(topLeftBlock),
			rightWallCount = (Byte)canSupportDoorHinge(bottomRightBlock) + canSupportDoorHinge(topRightBlock);

		if (leftWallCount > rightWallCount)
		{
			//more wall on left
			//hinge on left
			return doorHinge::left;
		}
		else if (leftWallCount < rightWallCount)
		{
			//more wall on right
			//hinge on right
			return doorHinge::right;
		}
		else
		{
			//the same amound of walls on both sides
			//hinge dep on cursor
			return curX < .5f ? doorHinge::left : doorHinge::right;
		}
	}
	break;
	case blockFacing::south:
	{
		BlockState bottomLeftState = wld->getBlock(destX + 1, destY, destZ),
			bottomRightState = wld->getBlock(destX - 1, destY, destZ);

		Block bottomLeftBlock = stateToBlock(bottomLeftState),
			bottomRightBlock = stateToBlock(bottomRightState);

		bool leftIsDoor = isDoorLower(bottomLeftBlock, bottomLeftState),
			rightIsDoor = isDoorLower(bottomRightBlock, bottomRightState);

		if (leftIsDoor && rightIsDoor)
		{
			//doors on both sides
			//hinge dep on cursor
			return curX >= .5f ? doorHinge::left : doorHinge::right;
		}
		if (leftIsDoor)
		{
			//door on left
			//hinge on right
			return doorHinge::right;
		}
		if (rightIsDoor)
		{
			//door on right
			//hinge on left
			return doorHinge::left;
		}

		BlockState topLeftState = wld->getBlock(destX + 1, destY, destZ),
			topRightState = wld->getBlock(destX - 1, destY, destZ);

		Block topLeftBlock = stateToBlock(topLeftState),
			topRightBlock = stateToBlock(topRightState);

		Byte leftWallCount = (Byte)canSupportDoorHinge(bottomLeftBlock) + canSupportDoorHinge(topLeftBlock),
			rightWallCount = (Byte)canSupportDoorHinge(bottomRightBlock) + canSupportDoorHinge(topRightBlock);

		if (leftWallCount > rightWallCount)
		{
			//more wall on left
			//hinge on left
			return doorHinge::left;
		}
		else if (leftWallCount < rightWallCount)
		{
			//more wall on right
			//hinge on right
			return doorHinge::right;
		}
		else
		{
			//the same amound of walls on both sides
			//hinge dep on cursor
			return curX >= .5f ? doorHinge::left : doorHinge::right;
		}
	}
	break;
	case blockFacing::east:
	{
		BlockState bottomLeftState = wld->getBlock(destX, destY, destZ - 1),
			bottomRightState = wld->getBlock(destX, destY, destZ + 1);

		Block bottomLeftBlock = stateToBlock(bottomLeftState),
			bottomRightBlock = stateToBlock(bottomRightState);

		bool leftIsDoor = isDoorLower(bottomLeftBlock, bottomLeftState),
			rightIsDoor = isDoorLower(bottomRightBlock, bottomRightState);

		if (leftIsDoor && rightIsDoor)
		{
			//doors on both sides
			//hinge dep on cursor
			return curZ < .5f ? doorHinge::left : doorHinge::right;
		}
		if (leftIsDoor)
		{
			//door on left
			//hinge on right
			return doorHinge::right;
		}
		if (rightIsDoor)
		{
			//door on right
			//hinge on left
			return doorHinge::left;
		}

		BlockState topLeftState = wld->getBlock(destX, destY, destZ - 1),
			topRightState = wld->getBlock(destX, destY, destZ + 1);

		Block topLeftBlock = stateToBlock(topLeftState),
			topRightBlock = stateToBlock(topRightState);

		Byte leftWallCount = (Byte)canSupportDoorHinge(bottomLeftBlock) + canSupportDoorHinge(topLeftBlock),
			rightWallCount = (Byte)canSupportDoorHinge(bottomRightBlock) + canSupportDoorHinge(topRightBlock);

		if (leftWallCount > rightWallCount)
		{
			//more wall on left
			//hinge on left
			return doorHinge::left;
		}
		else if (leftWallCount < rightWallCount)
		{
			//more wall on right
			//hinge on right
			return doorHinge::right;
		}
		else
		{
			//the same amound of walls on both sides
			//hinge dep on cursor
			return curZ < .5f ? doorHinge::left : doorHinge::right;
		}
	}
	break;
	case blockFacing::west:
	{
		BlockState bottomLeftState = wld->getBlock(destX, destY, destZ + 1),
			bottomRightState = wld->getBlock(destX, destY, destZ - 1);

		Block bottomLeftBlock = stateToBlock(bottomLeftState),
			bottomRightBlock = stateToBlock(bottomRightState);

		bool leftIsDoor = isDoorLower(bottomLeftBlock, bottomLeftState),
			rightIsDoor = isDoorLower(bottomRightBlock, bottomRightState);

		if (leftIsDoor && rightIsDoor)
		{
			//doors on both sides
			//hinge dep on cursor
			return curZ >= .5f ? doorHinge::left : doorHinge::right;
		}
		if (leftIsDoor)
		{
			//door on left
			//hinge on right
			return doorHinge::right;
		}
		if (rightIsDoor)
		{
			//door on right
			//hinge on left
			return doorHinge::left;
		}

		BlockState topLeftState = wld->getBlock(destX, destY, destZ + 1),
			topRightState = wld->getBlock(destX, destY, destZ - 1);

		Block topLeftBlock = stateToBlock(topLeftState),
			topRightBlock = stateToBlock(topRightState);

		Byte leftWallCount = (Byte)canSupportDoorHinge(bottomLeftBlock) + canSupportDoorHinge(topLeftBlock),
			rightWallCount = (Byte)canSupportDoorHinge(bottomRightBlock) + canSupportDoorHinge(topRightBlock);

		if (leftWallCount > rightWallCount)
		{
			//more wall on left
			//hinge on left
			return doorHinge::left;
		}
		else if (leftWallCount < rightWallCount)
		{
			//more wall on right
			//hinge on right
			return doorHinge::right;
		}
		else
		{
			//the same amound of walls on both sides
			//hinge dep on cursor
			return curZ >= .5f ? doorHinge::left : doorHinge::right;
		}
	}
	}
	throw std::exception("Could not choose door hinge");
}

//is this block waterloggable?
bool waterloggable(Block id)
{
	switch (id)
	{
	case Block::minecraft_cut_copper_slab:
	case Block::minecraft_exposed_cut_copper_slab:
	case Block::minecraft_weathered_cut_copper_slab:
	case Block::minecraft_oxidized_cut_copper_slab:
	case Block::minecraft_waxed_cut_copper_slab:
	case Block::minecraft_waxed_exposed_cut_copper_slab:
	case Block::minecraft_waxed_weathered_cut_copper_slab:
	case Block::minecraft_waxed_oxidized_cut_copper_slab:
	case Block::minecraft_oak_slab:
	case Block::minecraft_spruce_slab:
	case Block::minecraft_birch_slab:
	case Block::minecraft_jungle_slab:
	case Block::minecraft_acacia_slab:
	case Block::minecraft_dark_oak_slab:
	case Block::minecraft_crimson_slab:
	case Block::minecraft_warped_slab:
	case Block::minecraft_stone_slab:
	case Block::minecraft_smooth_stone_slab:
	case Block::minecraft_sandstone_slab:
	case Block::minecraft_cut_sandstone_slab:
	case Block::minecraft_petrified_oak_slab:
	case Block::minecraft_cobblestone_slab:
	case Block::minecraft_brick_slab:
	case Block::minecraft_stone_brick_slab:
	case Block::minecraft_nether_brick_slab:
	case Block::minecraft_quartz_slab:
	case Block::minecraft_red_sandstone_slab:
	case Block::minecraft_cut_red_sandstone_slab:
	case Block::minecraft_purpur_slab:
	case Block::minecraft_prismarine_slab:
	case Block::minecraft_prismarine_brick_slab:
	case Block::minecraft_dark_prismarine_slab:
	case Block::minecraft_polished_granite_slab:
	case Block::minecraft_smooth_red_sandstone_slab:
	case Block::minecraft_mossy_stone_brick_slab:
	case Block::minecraft_polished_diorite_slab:
	case Block::minecraft_mossy_cobblestone_slab:
	case Block::minecraft_end_stone_brick_slab:
	case Block::minecraft_smooth_sandstone_slab:
	case Block::minecraft_smooth_quartz_slab:
	case Block::minecraft_granite_slab:
	case Block::minecraft_andesite_slab:
	case Block::minecraft_red_nether_brick_slab:
	case Block::minecraft_polished_andesite_slab:
	case Block::minecraft_diorite_slab:
	case Block::minecraft_cobbled_deepslate_slab:
	case Block::minecraft_polished_deepslate_slab:
	case Block::minecraft_deepslate_brick_slab:
	case Block::minecraft_deepslate_tile_slab:
	case Block::minecraft_small_amethyst_bud:
	case Block::minecraft_medium_amethyst_bud:
	case Block::minecraft_large_amethyst_bud:
	case Block::minecraft_amethyst_cluster:
	case Block::minecraft_lightning_rod:
	case Block::minecraft_iron_trapdoor:
	case Block::minecraft_oak_trapdoor:
	case Block::minecraft_spruce_trapdoor:
	case Block::minecraft_birch_trapdoor:
	case Block::minecraft_jungle_trapdoor:
	case Block::minecraft_acacia_trapdoor:
	case Block::minecraft_dark_oak_trapdoor:
	case Block::minecraft_crimson_trapdoor:
	case Block::minecraft_warped_trapdoor:
		return true;
	}
	return false;
}
//does water destroy this block?
bool destroyedByWater(Block id)
{
	switch (id)
	{
	case Block::minecraft_oak_sapling:
	case Block::minecraft_spruce_sapling:
	case Block::minecraft_birch_sapling:
	case Block::minecraft_jungle_sapling:
	case Block::minecraft_acacia_sapling:
	case Block::minecraft_dark_oak_sapling:
	case Block::minecraft_grass:
	case Block::minecraft_fern:
	case Block::minecraft_dandelion:
	case Block::minecraft_poppy:
	case Block::minecraft_blue_orchid:
	case Block::minecraft_allium:
	case Block::minecraft_azure_bluet:
	case Block::minecraft_red_tulip:
	case Block::minecraft_orange_tulip:
	case Block::minecraft_white_tulip:
	case Block::minecraft_pink_tulip:
	case Block::minecraft_oxeye_daisy:
	case Block::minecraft_azalea:
	case Block::minecraft_flowering_azalea:
	case Block::minecraft_cornflower:
	case Block::minecraft_lily_of_the_valley:
	case Block::minecraft_wither_rose:
		return true;
	}
	return false;
}
//replaceable by rightclicking on the block?
bool replaceableDirect(Block id)
{
	switch (id)
	{
	case Block::minecraft_grass:
	case Block::minecraft_fern:
		return true;
	}
	return false;
}
//replaceable by clicking on a wall nearby and placing a block there?
bool replaceableIndirect(Block id)
{
	switch (id)
	{
	case Block::minecraft_air:
	case Block::minecraft_water:
	case Block::minecraft_lava:
	case Block::minecraft_grass:
	case Block::minecraft_fern:
		return true;
	}
	return false;
}

bool rightClickBlock(Player* p, Block bid, int destX, int destY, int destZ, BlockState& state, Position loc)
{
	//cannot right-click while crouching
	if (p->attributes & (Byte)Entity::Attributes::isCrouching) return false;
	switch (bid)
	{
	case Block::minecraft_crafting_table:
		//message::play::send::openWindow(p,)
		message::play::send::chatMessage(p, Chat("Crafting table right-clicked"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		return true;
	case Block::minecraft_enchanting_table:
		message::play::send::chatMessage(p, Chat("Enchanting table right-clicked"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		return true;
	case Block::minecraft_cartography_table:
		message::play::send::chatMessage(p, Chat("Cartography table right-clicked"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		return true;
	case Block::minecraft_smithing_table:
		message::play::send::chatMessage(p, Chat("Smithing table right-clicked"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		return true;
	case Block::minecraft_anvil:
	case Block::minecraft_chipped_anvil:
	case Block::minecraft_damaged_anvil:
		message::play::send::chatMessage(p, Chat("Anvil right-clicked"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		return true;
	case Block::minecraft_stonecutter:
		message::play::send::chatMessage(p, Chat("Stonecutter right-clicked"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		return true;
	case Block::minecraft_furnace:
		message::play::send::chatMessage(p, Chat("Furnace right-clicked"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		return true;
	case Block::minecraft_blast_furnace:
		message::play::send::chatMessage(p, Chat("Blast furnace right-clicked"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		return true;
	case Block::minecraft_smoker:
		message::play::send::chatMessage(p, Chat("Smoker right-clicked"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		return true;
	case Block::minecraft_loom:
		message::play::send::chatMessage(p, Chat("Loom right-clicked"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		return true;
	case Block::minecraft_shulker_box:
	case Block::minecraft_white_shulker_box:
	case Block::minecraft_orange_shulker_box:
	case Block::minecraft_magenta_shulker_box:
	case Block::minecraft_light_blue_shulker_box:
	case Block::minecraft_yellow_shulker_box:
	case Block::minecraft_lime_shulker_box:
	case Block::minecraft_pink_shulker_box:
	case Block::minecraft_gray_shulker_box:
	case Block::minecraft_light_gray_shulker_box:
	case Block::minecraft_cyan_shulker_box:
	case Block::minecraft_purple_shulker_box:
	case Block::minecraft_blue_shulker_box:
	case Block::minecraft_brown_shulker_box:
	case Block::minecraft_green_shulker_box:
	case Block::minecraft_red_shulker_box:
	case Block::minecraft_black_shulker_box:
		message::play::send::chatMessage(p, Chat("Shulker box right-clicked"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		return true;
	case Block::minecraft_barrel:
		message::play::send::chatMessage(p, Chat("Barrel right-clicked"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		return true;
	case Block::minecraft_dispenser:
		message::play::send::chatMessage(p, Chat("Dispenser right-clicked"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		return true;
	case Block::minecraft_dropper:
		message::play::send::chatMessage(p, Chat("Dropper right-clicked"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		return true;
	case Block::minecraft_oak_trapdoor:
	case Block::minecraft_spruce_trapdoor:
	case Block::minecraft_birch_trapdoor:
	case Block::minecraft_jungle_trapdoor:
	case Block::minecraft_acacia_trapdoor:
	case Block::minecraft_dark_oak_trapdoor:
	case Block::minecraft_crimson_trapdoor:
	case Block::minecraft_warped_trapdoor:
		message::play::send::chatMessage(p, Chat("Trapdoor right-clicked"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		state.setState("open", state.getState("open") == "true" ? "false" : "true");
		p->world->setBlock(destX, destY, destZ, state);
		for (Player* seener : p->world->players) if (seener != p && seener->positionInRange(loc)) message::play::send::blockChange(seener, loc, state.id);
		//send the opening to other players + sound
		return true;
	case Block::minecraft_oak_door:
	case Block::minecraft_spruce_door:
	case Block::minecraft_birch_door:
	case Block::minecraft_jungle_door:
	case Block::minecraft_acacia_door:
	case Block::minecraft_dark_oak_door:
	case Block::minecraft_crimson_door:
	case Block::minecraft_warped_door:
		message::play::send::chatMessage(p, Chat("Door right-clicked"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		std::string open = state.getState("open"),
			inverseOpen = (open == "true" ? "false" : "true");
		World* wld = p->world;
		if (state.getState("half") == "lower")
		{
			//lower half right-clicked
			state.setState("open", inverseOpen);
			wld->setBlock(destX, destY, destZ, state);

			//open the upper door too
			if (!p->world->checkCoordinates(++destY)) throw std::exception("Door outside of world used");
			BlockState upperState = wld->getBlock(destX, destY, destZ);
			if (stateToBlock(upperState) != bid) throw std::exception("Upper half of door not found");
			upperState.setState("open", inverseOpen);
			wld->setBlock(destX, destY, destZ, upperState);
			Position loc2 = loc;
			loc2.setY(loc2.y() + 1);
			for (Player* seener : wld->players)
			{
				if (seener != p && seener->positionInRange(loc))
				{
					message::play::send::blockChange(seener, loc, state.id);
					message::play::send::blockChange(seener, loc2, upperState.id);
				}
			}
		}
		else
		{
			//upper half right-clicked
			state.setState("open", inverseOpen);
			wld->setBlock(destX, destY, destZ, state);

			//open the lower door too
			if (!p->world->checkCoordinates(--destY)) throw std::exception("Door outside of world used");
			BlockState lowerState = wld->getBlock(destX, destY, destZ);
			if (stateToBlock(lowerState) != bid) throw std::exception("Lower half of door not found");
			lowerState.setState("open", inverseOpen);
			wld->setBlock(destX, destY, destZ, lowerState);
			Position loc2 = loc;
			loc2.setY(loc2.y() - 1);
			for (Player* seener : wld->players)
			{
				if (seener != p && seener->positionInRange(loc))
				{
					message::play::send::blockChange(seener, loc, state.id);
					message::play::send::blockChange(seener, loc2, lowerState.id);
				}
			}
		}
		return true;
	}
	return false;
}

SERVER_API void World::setBlockByItem(Player* p, Slot* slot, Position loc, playerDigging::face face, bfloat curX, bfloat curY, bfloat curZ)
{
	//throw "WIP";
	json* stateJson = nullptr;
	int itemId = slot->getItemId(),
		destX = loc.x(),
		destY = p->world->AbsToRelHeight(loc.y()),
		destZ = loc.z();

	if (!p->world->checkCoordinates(destY))
		//clicked block outside of world, maybe kick the player?
		return;

	//check block interaction first
	BlockState targetBlockState = p->world->getBlock(destX, destY, destZ);
	std::string targetBlockName = Registry::getBlock(targetBlockState.id);
	Block targetBlockId = (Block)Registry::getId(Registry::blockRegistry, targetBlockName);

	if (rightClickBlock(p, targetBlockId, destX, destY, destZ, targetBlockState, loc))
	{
		//block succsessfully right clicked
		return;
	}

	try
	{
		switch ((Item)itemId)
		{
			//unplaceable items
			{
		case Item::minecraft_air:
		case Item::minecraft_saddle:
			return;
			}

			//blocks with no states dependent on placement
			{
		case Item::minecraft_stone:
		case Item::minecraft_granite:
		case Item::minecraft_polished_granite:
		case Item::minecraft_diorite:
		case Item::minecraft_polished_diorite:
		case Item::minecraft_andesite:
		case Item::minecraft_polished_andesite:
		case Item::minecraft_cobbled_deepslate:
		case Item::minecraft_polished_deepslate:
		case Item::minecraft_calcite:
		case Item::minecraft_tuff:
		case Item::minecraft_dripstone_block:
		case Item::minecraft_dirt:
		case Item::minecraft_coarse_dirt:
		case Item::minecraft_rooted_dirt:
		case Item::minecraft_crimson_nylium:
		case Item::minecraft_warped_nylium:
		case Item::minecraft_cobblestone:
		case Item::minecraft_oak_planks:
		case Item::minecraft_spruce_planks:
		case Item::minecraft_birch_planks:
		case Item::minecraft_jungle_planks:
		case Item::minecraft_acacia_planks:
		case Item::minecraft_dark_oak_planks:
		case Item::minecraft_crimson_planks:
		case Item::minecraft_warped_planks:
		case Item::minecraft_bedrock:
		case Item::minecraft_sand:
		case Item::minecraft_red_sand:
		case Item::minecraft_gravel:
		case Item::minecraft_coal_ore:
		case Item::minecraft_deepslate_coal_ore:
		case Item::minecraft_iron_ore:
		case Item::minecraft_deepslate_iron_ore:
		case Item::minecraft_copper_ore:
		case Item::minecraft_deepslate_copper_ore:
		case Item::minecraft_gold_ore:
		case Item::minecraft_deepslate_gold_ore:
		case Item::minecraft_redstone_ore:
		case Item::minecraft_deepslate_redstone_ore:
		case Item::minecraft_emerald_ore:
		case Item::minecraft_deepslate_emerald_ore:
		case Item::minecraft_lapis_ore:
		case Item::minecraft_deepslate_lapis_ore:
		case Item::minecraft_diamond_ore:
		case Item::minecraft_deepslate_diamond_ore:
		case Item::minecraft_nether_gold_ore:
		case Item::minecraft_nether_quartz_ore:
		case Item::minecraft_ancient_debris:
		case Item::minecraft_coal_block:
		case Item::minecraft_raw_iron_block:
		case Item::minecraft_raw_copper_block:
		case Item::minecraft_raw_gold_block:
		case Item::minecraft_amethyst_block:
		case Item::minecraft_budding_amethyst:
		case Item::minecraft_iron_block:
		case Item::minecraft_copper_block:
		case Item::minecraft_gold_block:
		case Item::minecraft_diamond_block:
		case Item::minecraft_netherite_block:
		case Item::minecraft_exposed_copper:
		case Item::minecraft_weathered_copper:
		case Item::minecraft_oxidized_copper:
		case Item::minecraft_cut_copper:
		case Item::minecraft_exposed_cut_copper:
		case Item::minecraft_weathered_cut_copper:
		case Item::minecraft_oxidized_cut_copper:
		case Item::minecraft_waxed_copper_block:
		case Item::minecraft_waxed_exposed_copper:
		case Item::minecraft_waxed_weathered_copper:
		case Item::minecraft_waxed_oxidized_copper:
		case Item::minecraft_waxed_cut_copper:
		case Item::minecraft_waxed_exposed_cut_copper:
		case Item::minecraft_waxed_weathered_cut_copper:
		case Item::minecraft_waxed_oxidized_cut_copper:
		case Item::minecraft_white_wool:
		case Item::minecraft_orange_wool:
		case Item::minecraft_magenta_wool:
		case Item::minecraft_light_blue_wool:
		case Item::minecraft_yellow_wool:
		case Item::minecraft_lime_wool:
		case Item::minecraft_pink_wool:
		case Item::minecraft_gray_wool:
		case Item::minecraft_light_gray_wool:
		case Item::minecraft_cyan_wool:
		case Item::minecraft_purple_wool:
		case Item::minecraft_blue_wool:
		case Item::minecraft_brown_wool:
		case Item::minecraft_green_wool:
		case Item::minecraft_red_wool:
		case Item::minecraft_black_wool:
		case Item::minecraft_sponge:
		case Item::minecraft_wet_sponge:
		case Item::minecraft_glass:
		case Item::minecraft_tinted_glass:
		case Item::minecraft_lapis_block:
		case Item::minecraft_sandstone:
		case Item::minecraft_chiseled_sandstone:
		case Item::minecraft_cut_sandstone:
		case Item::minecraft_cobweb:
		case Item::minecraft_smooth_quartz:
		case Item::minecraft_smooth_red_sandstone:
		case Item::minecraft_smooth_sandstone:
		case Item::minecraft_smooth_stone:
		case Item::minecraft_bricks:
		case Item::minecraft_bookshelf:
		case Item::minecraft_mossy_cobblestone:
		case Item::minecraft_obsidian:
		case Item::minecraft_purpur_block:
		case Item::minecraft_glowstone:
		case Item::minecraft_infested_stone:
		case Item::minecraft_infested_cobblestone:
		case Item::minecraft_infested_stone_bricks:
		case Item::minecraft_infested_mossy_stone_bricks:
		case Item::minecraft_infested_cracked_stone_bricks:
		case Item::minecraft_infested_chiseled_stone_bricks:
		case Item::minecraft_infested_deepslate:
		case Item::minecraft_stone_bricks:
		case Item::minecraft_mossy_stone_bricks:
		case Item::minecraft_cracked_stone_bricks:
		case Item::minecraft_chiseled_stone_bricks:
		case Item::minecraft_deepslate_bricks:
		case Item::minecraft_cracked_deepslate_bricks:
		case Item::minecraft_deepslate_tiles:
		case Item::minecraft_cracked_deepslate_tiles:
		case Item::minecraft_chiseled_deepslate:
		case Item::minecraft_crafting_table:
		case Item::minecraft_snow_block:
		case Item::minecraft_ice:
		case Item::minecraft_clay:
		case Item::minecraft_jukebox:
		case Item::minecraft_netherrack:
		case Item::minecraft_soul_sand:
		case Item::minecraft_soul_soil:
		case Item::minecraft_pumpkin:
		case Item::minecraft_smooth_basalt:
		case Item::minecraft_melon:
		case Item::minecraft_nether_bricks:
		case Item::minecraft_cracked_nether_bricks:
		case Item::minecraft_chiseled_nether_bricks:
		case Item::minecraft_emerald_block:
		case Item::minecraft_white_terracotta:
		case Item::minecraft_orange_terracotta:
		case Item::minecraft_magenta_terracotta:
		case Item::minecraft_light_blue_terracotta:
		case Item::minecraft_yellow_terracotta:
		case Item::minecraft_lime_terracotta:
		case Item::minecraft_pink_terracotta:
		case Item::minecraft_gray_terracotta:
		case Item::minecraft_light_gray_terracotta:
		case Item::minecraft_cyan_terracotta:
		case Item::minecraft_purple_terracotta:
		case Item::minecraft_blue_terracotta:
		case Item::minecraft_brown_terracotta:
		case Item::minecraft_green_terracotta:
		case Item::minecraft_red_terracotta:
		case Item::minecraft_black_terracotta:
		case Item::minecraft_barrier:
		case Item::minecraft_white_stained_glass:
		case Item::minecraft_orange_stained_glass:
		case Item::minecraft_magenta_stained_glass:
		case Item::minecraft_light_blue_stained_glass:
		case Item::minecraft_yellow_stained_glass:
		case Item::minecraft_lime_stained_glass:
		case Item::minecraft_pink_stained_glass:
		case Item::minecraft_gray_stained_glass:
		case Item::minecraft_light_gray_stained_glass:
		case Item::minecraft_cyan_stained_glass:
		case Item::minecraft_purple_stained_glass:
		case Item::minecraft_blue_stained_glass:
		case Item::minecraft_brown_stained_glass:
		case Item::minecraft_green_stained_glass:
		case Item::minecraft_red_stained_glass:
		case Item::minecraft_black_stained_glass:
		case Item::minecraft_terracotta:
		case Item::minecraft_packed_ice:
		case Item::minecraft_end_stone:
		case Item::minecraft_end_stone_bricks:
		case Item::minecraft_chiseled_quartz_block:
		case Item::minecraft_quartz_block:
		case Item::minecraft_quartz_bricks:
		case Item::minecraft_white_concrete:
		case Item::minecraft_orange_concrete:
		case Item::minecraft_magenta_concrete:
		case Item::minecraft_light_blue_concrete:
		case Item::minecraft_yellow_concrete:
		case Item::minecraft_lime_concrete:
		case Item::minecraft_pink_concrete:
		case Item::minecraft_gray_concrete:
		case Item::minecraft_light_gray_concrete:
		case Item::minecraft_cyan_concrete:
		case Item::minecraft_purple_concrete:
		case Item::minecraft_blue_concrete:
		case Item::minecraft_brown_concrete:
		case Item::minecraft_green_concrete:
		case Item::minecraft_red_concrete:
		case Item::minecraft_black_concrete:
		case Item::minecraft_white_concrete_powder:
		case Item::minecraft_orange_concrete_powder:
		case Item::minecraft_magenta_concrete_powder:
		case Item::minecraft_light_blue_concrete_powder:
		case Item::minecraft_yellow_concrete_powder:
		case Item::minecraft_lime_concrete_powder:
		case Item::minecraft_pink_concrete_powder:
		case Item::minecraft_gray_concrete_powder:
		case Item::minecraft_light_gray_concrete_powder:
		case Item::minecraft_cyan_concrete_powder:
		case Item::minecraft_purple_concrete_powder:
		case Item::minecraft_blue_concrete_powder:
		case Item::minecraft_brown_concrete_powder:
		case Item::minecraft_green_concrete_powder:
		case Item::minecraft_red_concrete_powder:
		case Item::minecraft_black_concrete_powder:
		case Item::minecraft_prismarine:
		case Item::minecraft_prismarine_bricks:
		case Item::minecraft_dark_prismarine:
		case Item::minecraft_sea_lantern:
		case Item::minecraft_red_sandstone:
		case Item::minecraft_chiseled_red_sandstone:
		case Item::minecraft_cut_red_sandstone:
		case Item::minecraft_magma_block:
		case Item::minecraft_nether_wart_block:
		case Item::minecraft_warped_wart_block:
		case Item::minecraft_red_nether_bricks:
		case Item::minecraft_dead_tube_coral_block:
		case Item::minecraft_dead_brain_coral_block:
		case Item::minecraft_dead_bubble_coral_block:
		case Item::minecraft_dead_fire_coral_block:
		case Item::minecraft_dead_horn_coral_block:
		case Item::minecraft_tube_coral_block:
		case Item::minecraft_brain_coral_block:
		case Item::minecraft_bubble_coral_block:
		case Item::minecraft_fire_coral_block:
		case Item::minecraft_horn_coral_block:
		case Item::minecraft_blue_ice:
		case Item::minecraft_redstone_block:
		case Item::minecraft_slime_block:
		case Item::minecraft_honey_block:
		case Item::minecraft_target:
		case Item::minecraft_tnt:
		case Item::minecraft_dried_kelp_block:
		case Item::minecraft_moss_block:
		case Item::minecraft_enchanting_table:
		case Item::minecraft_shroomlight:
		case Item::minecraft_honeycomb_block:
		case Item::minecraft_lodestone:
		case Item::minecraft_crying_obsidian:
		case Item::minecraft_blackstone:
		case Item::minecraft_gilded_blackstone:
		case Item::minecraft_polished_blackstone:
		case Item::minecraft_chiseled_polished_blackstone:
		case Item::minecraft_polished_blackstone_bricks:
		case Item::minecraft_cracked_polished_blackstone_bricks:
		case Item::minecraft_cartography_table:
		case Item::minecraft_fletching_table:
		case Item::minecraft_smithing_table:
		case Item::minecraft_cauldron:
			if (replaceableDirect(targetBlockId))
			{
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId));
				break;
			}
			switch (face)
			{
			case playerDigging::top:
				destY++;
				break;
			case playerDigging::bottom:
				destY--;
				break;
			case playerDigging::east:
				destX++;
				break;
			case playerDigging::west:
				destX--;
				break;
			case playerDigging::south:
				destZ++;
				break;
			case playerDigging::north:
				destZ--;
			}
			if (!p->world->checkCoordinates(destY))
				//destY out of world
				return;

			BlockState oldBlockState = getBlock(destX, destY, destZ);
			std::string oldBlockName = Registry::getBlock(oldBlockState.id);
			Block oldBlockId = (Block)Registry::getId(Registry::blockRegistry, oldBlockName);

			if (replaceableIndirect(oldBlockId)) stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId));
			break;
			}

			//blocks that need to be placed on dirt

			{ {
			case Item::minecraft_grass:
			case Item::minecraft_fern:
			case Item::minecraft_dandelion:
			case Item::minecraft_poppy:
			case Item::minecraft_blue_orchid:
			case Item::minecraft_allium:
			case Item::minecraft_azure_bluet:
			case Item::minecraft_red_tulip:
			case Item::minecraft_orange_tulip:
			case Item::minecraft_white_tulip:
			case Item::minecraft_pink_tulip:
			case Item::minecraft_oxeye_daisy:
			case Item::minecraft_oak_sapling:
			case Item::minecraft_spruce_sapling:
			case Item::minecraft_birch_sapling:
			case Item::minecraft_jungle_sapling:
			case Item::minecraft_acacia_sapling:
			case Item::minecraft_dark_oak_sapling:
			case Item::minecraft_azalea:
			case Item::minecraft_flowering_azalea:
			case Item::minecraft_cornflower:
			case Item::minecraft_lily_of_the_valley:
			case Item::minecraft_wither_rose:
			{
				std::string heldBlockName = Registry::getName(Registry::itemRegistry, itemId);
				Block heldBlockId = (Block)Registry::getId(Registry::blockRegistry, heldBlockName);
				if (targetBlockId != heldBlockId && replaceableDirect(targetBlockId))
				{
					//get the block below, if in world
					int soilY = destY - 1;
					if (!p->world->checkCoordinates(soilY)) break;
					BlockState soilBlockState = getBlock(destX, soilY, destZ);
					std::string soilBlockName = Registry::getBlock(soilBlockState.id);
					Block soilBlockId = (Block)Registry::getId(Registry::blockRegistry, soilBlockName);
					//check if the block below is dirt
					if (!TagGroup::checkTagEntry("minecraft:block", "minecraft:dirt", (int)soilBlockId)) break;
					stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId));
					break;
				}
				switch (face)
				{
				case playerDigging::top:
					destY++;
					break;
				case playerDigging::bottom:
					destY--;
					break;
				case playerDigging::east:
					destX++;
					break;
				case playerDigging::west:
					destX--;
					break;
				case playerDigging::south:
					destZ++;
					break;
				case playerDigging::north:
					destZ--;
				}
				if (!p->world->checkCoordinates(destY))
					//destY out of world
					return;

				BlockState oldBlockState = getBlock(destX, destY, destZ);
				std::string oldBlockName = Registry::getBlock(oldBlockState.id);
				Block oldBlockId = (Block)Registry::getId(Registry::blockRegistry, oldBlockName);

				if (heldBlockId != oldBlockId && replaceableIndirect(oldBlockId))
				{
					int soilY = destY - 1;
					if (!p->world->checkCoordinates(soilY)) break;
					BlockState soilBlockState = getBlock(destX, soilY, destZ);
					std::string soilBlockName = Registry::getBlock(soilBlockState.id);
					Block soilBlockId = (Block)Registry::getId(Registry::blockRegistry, soilBlockName);
					//check if the block below is dirt
					if (!TagGroup::checkTagEntry("minecraft:block", "minecraft:dirt", (int)soilBlockId)) break;
					stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId));
				}
				break;
			}
				}}

			//stairs
			{
		case Item::minecraft_cut_copper_stairs:
		case Item::minecraft_exposed_cut_copper_stairs:
		case Item::minecraft_weathered_cut_copper_stairs:
		case Item::minecraft_oxidized_cut_copper_stairs:
		case Item::minecraft_waxed_cut_copper_stairs:
		case Item::minecraft_waxed_exposed_cut_copper_stairs:
		case Item::minecraft_waxed_weathered_cut_copper_stairs:
		case Item::minecraft_waxed_oxidized_cut_copper_stairs:
		case Item::minecraft_purpur_stairs:
		case Item::minecraft_oak_stairs:
		case Item::minecraft_spruce_stairs:
		case Item::minecraft_birch_stairs:
		case Item::minecraft_jungle_stairs:
		case Item::minecraft_crimson_stairs:
		case Item::minecraft_warped_stairs:
		case Item::minecraft_cobblestone_stairs:
		case Item::minecraft_acacia_stairs:
		case Item::minecraft_dark_oak_stairs:
		case Item::minecraft_brick_stairs:
		case Item::minecraft_stone_brick_stairs:
		case Item::minecraft_nether_brick_stairs:
		case Item::minecraft_sandstone_stairs:
		case Item::minecraft_quartz_stairs:
		case Item::minecraft_prismarine_stairs:
		case Item::minecraft_prismarine_brick_stairs:
		case Item::minecraft_dark_prismarine_stairs:
		case Item::minecraft_red_sandstone_stairs:
		case Item::minecraft_polished_granite_stairs:
		case Item::minecraft_smooth_red_sandstone_stairs:
		case Item::minecraft_mossy_stone_brick_stairs:
		case Item::minecraft_polished_diorite_stairs:
		case Item::minecraft_mossy_cobblestone_stairs:
		case Item::minecraft_end_stone_brick_stairs:
		case Item::minecraft_stone_stairs:
		case Item::minecraft_smooth_sandstone_stairs:
		case Item::minecraft_smooth_quartz_stairs:
		case Item::minecraft_granite_stairs:
		case Item::minecraft_andesite_stairs:
		case Item::minecraft_red_nether_brick_stairs:
		case Item::minecraft_polished_andesite_stairs:
		case Item::minecraft_diorite_stairs:
		case Item::minecraft_cobbled_deepslate_stairs:
		case Item::minecraft_polished_deepslate_stairs:
		case Item::minecraft_deepslate_brick_stairs:
		case Item::minecraft_deepslate_tile_stairs:
		case Item::minecraft_blackstone_stairs:
		case Item::minecraft_polished_blackstone_stairs:
		case Item::minecraft_polished_blackstone_brick_stairs:
			message::play::send::chatMessage(p, Chat("No stairs allowed!", Chat::color::red), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
			return;

			}

			//blocks that have the "facing" property, dependend on player's yaw
			{
		case Item::minecraft_anvil:
		case Item::minecraft_chipped_anvil:
		case Item::minecraft_damaged_anvil:
		case Item::minecraft_loom:
		case Item::minecraft_carved_pumpkin:
		case Item::minecraft_jack_o_lantern:
		case Item::minecraft_white_glazed_terracotta:
		case Item::minecraft_orange_glazed_terracotta:
		case Item::minecraft_magenta_glazed_terracotta:
		case Item::minecraft_light_blue_glazed_terracotta:
		case Item::minecraft_yellow_glazed_terracotta:
		case Item::minecraft_lime_glazed_terracotta:
		case Item::minecraft_pink_glazed_terracotta:
		case Item::minecraft_gray_glazed_terracotta:
		case Item::minecraft_light_gray_glazed_terracotta:
		case Item::minecraft_cyan_glazed_terracotta:
		case Item::minecraft_purple_glazed_terracotta:
		case Item::minecraft_blue_glazed_terracotta:
		case Item::minecraft_brown_glazed_terracotta:
		case Item::minecraft_green_glazed_terracotta:
		case Item::minecraft_red_glazed_terracotta:
		case Item::minecraft_black_glazed_terracotta:
		case Item::minecraft_stonecutter:
		{
			if (replaceableDirect(targetBlockId))
			{
				BlockProperty* props = new BlockProperty[1];
				props[0].name = "facing";
				props[0].value = facingToString(getHorizontalFacing(p, p->yaw, (Item)itemId));
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
				break;
			}
			switch (face)
			{
			case playerDigging::top:
				destY++;
				break;
			case playerDigging::bottom:
				destY--;
				break;
			case playerDigging::east:
				destX++;
				break;
			case playerDigging::west:
				destX--;
				break;
			case playerDigging::south:
				destZ++;
				break;
			case playerDigging::north:
				destZ--;
			}
			if (!p->world->checkCoordinates(destY))
				//destY out of world
				return;

			BlockState oldBlockState = getBlock(destX, destY, destZ);
			std::string oldBlockName = Registry::getBlock(oldBlockState.id);
			Block oldBlockId = (Block)Registry::getId(Registry::blockRegistry, oldBlockName);

			if (replaceableIndirect(oldBlockId))
			{
				BlockProperty* props = new BlockProperty[1];
				props[0].name = "facing";
				props[0].value = facingToString(getHorizontalFacing(p, p->yaw, (Item)itemId));
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
			}
			break;
		}
		case Item::minecraft_bee_nest: //honey_level property missing
		case Item::minecraft_beehive: //honey_level property missing
		{
			if (replaceableDirect(targetBlockId))
			{
				BlockProperty* props = new BlockProperty[2];
				props[0].name = "facing";
				props[0].value = facingToString(getHorizontalFacing(p, p->yaw, (Item)itemId));
				props[1].name = "honey_level";
				props[1].value = "0";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
				break;
			}
			switch (face)
			{
			case playerDigging::top:
				destY++;
				break;
			case playerDigging::bottom:
				destY--;
				break;
			case playerDigging::east:
				destX++;
				break;
			case playerDigging::west:
				destX--;
				break;
			case playerDigging::south:
				destZ++;
				break;
			case playerDigging::north:
				destZ--;
			}
			if (!p->world->checkCoordinates(destY))
				//destY out of world
				return;

			BlockState oldBlockState = getBlock(destX, destY, destZ);
			std::string oldBlockName = Registry::getBlock(oldBlockState.id);
			Block oldBlockId = (Block)Registry::getId(Registry::blockRegistry, oldBlockName);

			if (replaceableIndirect(oldBlockId))
			{
				BlockProperty* props = new BlockProperty[2];
				props[0].name = "facing";
				props[0].value = facingToString(getHorizontalFacing(p, p->yaw, (Item)itemId));
				props[1].name = "honey_level";
				props[1].value = "0";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
			}
			break;
		}
		case Item::minecraft_end_portal_frame:
		{
			if (replaceableDirect(targetBlockId))
			{
				BlockProperty* props = new BlockProperty[2];
				props[0].name = "facing";
				props[0].value = facingToString(getHorizontalFacing(p, p->yaw, (Item)itemId));
				props[1].name = "eye";
				props[1].value = "false";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
				break;
			}
			switch (face)
			{
			case playerDigging::top:
				destY++;
				break;
			case playerDigging::bottom:
				destY--;
				break;
			case playerDigging::east:
				destX++;
				break;
			case playerDigging::west:
				destX--;
				break;
			case playerDigging::south:
				destZ++;
				break;
			case playerDigging::north:
				destZ--;
			}
			if (!p->world->checkCoordinates(destY))
				//destY out of world
				return;

			BlockState oldBlockState = getBlock(destX, destY, destZ);
			std::string oldBlockName = Registry::getBlock(oldBlockState.id);
			Block oldBlockId = (Block)Registry::getId(Registry::blockRegistry, oldBlockName);

			if (replaceableIndirect(oldBlockId))
			{
				BlockProperty* props = new BlockProperty[2];
				props[0].name = "facing";
				props[0].value = facingToString(getHorizontalFacing(p, p->yaw, (Item)itemId));
				props[1].name = "eye";
				props[1].value = "false";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
			}
			break;
		}
		case Item::minecraft_furnace:
		case Item::minecraft_smoker:
		case Item::minecraft_blast_furnace:
		{
			if (replaceableDirect(targetBlockId))
			{
				BlockProperty* props = new BlockProperty[2];
				props[0].name = "facing";
				props[0].value = facingToString(getHorizontalFacing(p, p->yaw, (Item)itemId));
				props[1].name = "lit";
				props[1].value = "false";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
				break;
			}
			switch (face)
			{
			case playerDigging::top:
				destY++;
				break;
			case playerDigging::bottom:
				destY--;
				break;
			case playerDigging::east:
				destX++;
				break;
			case playerDigging::west:
				destX--;
				break;
			case playerDigging::south:
				destZ++;
				break;
			case playerDigging::north:
				destZ--;
			}
			if (!p->world->checkCoordinates(destY))
				//destY out of world
				return;

			BlockState oldBlockState = getBlock(destX, destY, destZ);
			std::string oldBlockName = Registry::getBlock(oldBlockState.id);
			Block oldBlockId = (Block)Registry::getId(Registry::blockRegistry, oldBlockName);

			if (replaceableIndirect(oldBlockId))
			{
				BlockProperty* props = new BlockProperty[2];
				props[0].name = "facing";
				props[0].value = facingToString(getHorizontalFacing(p, p->yaw, (Item)itemId));
				props[1].name = "lit";
				props[1].value = "false";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
			}
			break;
		}
			}

			//blocks that have the "facing" property, dependent on player's yaw and pitch
			{ {
		case Item::minecraft_piston:
		case Item::minecraft_sticky_piston:
		{
			if (replaceableDirect(targetBlockId))
			{
				BlockProperty* props = new BlockProperty[2];
				props[0].name = "facing";
				props[0].value = facingToString(get3DFacing(p, p->yaw, p->pitch, (Item)itemId));
				props[1].name = "extended";
				props[1].value = "false";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
				break;
			}
			switch (face)
			{
			case playerDigging::top:
				destY++;
				break;
			case playerDigging::bottom:
				destY--;
				break;
			case playerDigging::east:
				destX++;
				break;
			case playerDigging::west:
				destX--;
				break;
			case playerDigging::south:
				destZ++;
				break;
			case playerDigging::north:
				destZ--;
			}
			if (!p->world->checkCoordinates(destY))
				//destY out of world
				return;

			BlockState oldBlockState = getBlock(destX, destY, destZ);
			std::string oldBlockName = Registry::getBlock(oldBlockState.id);
			Block oldBlockId = (Block)Registry::getId(Registry::blockRegistry, oldBlockName);

			if (replaceableIndirect(oldBlockId))
			{
				BlockProperty* props = new BlockProperty[2];
				props[0].name = "facing";
				props[0].value = facingToString(get3DFacing(p, p->yaw, p->pitch, (Item)itemId));
				props[1].name = "extended";
				props[1].value = "false";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
			}
			break;
		}
		case Item::minecraft_observer:
		{
			if (replaceableDirect(targetBlockId))
			{
				BlockProperty* props = new BlockProperty[2];
				props[0].name = "facing";
				props[0].value = facingToString(get3DFacing(p, p->yaw, -p->pitch, (Item)itemId));
				props[1].name = "powered";
				props[1].value = "false";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
				break;
			}
			switch (face)
			{
			case playerDigging::top:
				destY++;
				break;
			case playerDigging::bottom:
				destY--;
				break;
			case playerDigging::east:
				destX++;
				break;
			case playerDigging::west:
				destX--;
				break;
			case playerDigging::south:
				destZ++;
				break;
			case playerDigging::north:
				destZ--;
			}
			if (!p->world->checkCoordinates(destY))
				//destY out of world
				return;

			BlockState oldBlockState = getBlock(destX, destY, destZ);
			std::string oldBlockName = Registry::getBlock(oldBlockState.id);
			Block oldBlockId = (Block)Registry::getId(Registry::blockRegistry, oldBlockName);

			if (replaceableIndirect(oldBlockId))
			{
				BlockProperty* props = new BlockProperty[2];
				props[0].name = "facing";
				props[0].value = facingToString(get3DFacing(p, p->yaw, -p->pitch, (Item)itemId));
				props[1].name = "powered";
				props[1].value = "false";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
			}
			break;
		}
		case Item::minecraft_dispenser:
		case Item::minecraft_dropper:
		{
			if (replaceableDirect(targetBlockId))
			{
				BlockProperty* props = new BlockProperty[2];
				props[0].name = "facing";
				props[0].value = facingToString(get3DFacing(p, p->yaw, p->pitch, (Item)itemId));
				props[1].name = "triggered";
				props[1].value = "false";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
				break;
			}
			switch (face)
			{
			case playerDigging::top:
				destY++;
				break;
			case playerDigging::bottom:
				destY--;
				break;
			case playerDigging::east:
				destX++;
				break;
			case playerDigging::west:
				destX--;
				break;
			case playerDigging::south:
				destZ++;
				break;
			case playerDigging::north:
				destZ--;
			}
			if (!p->world->checkCoordinates(destY))
				//destY out of world
				return;

			BlockState oldBlockState = getBlock(destX, destY, destZ);
			std::string oldBlockName = Registry::getBlock(oldBlockState.id);
			Block oldBlockId = (Block)Registry::getId(Registry::blockRegistry, oldBlockName);

			if (replaceableIndirect(oldBlockId))
			{
				BlockProperty* props = new BlockProperty[2];
				props[0].name = "facing";
				props[0].value = facingToString(get3DFacing(p, p->yaw, p->pitch, (Item)itemId));
				props[1].name = "triggered";
				props[1].value = "false";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
			}
			break;
		}
		case Item::minecraft_barrel:
		{
			if (replaceableDirect(targetBlockId))
			{
				BlockProperty* props = new BlockProperty[2];
				props[0].name = "facing";
				props[0].value = facingToString(get3DFacing(p, p->yaw, p->pitch, (Item)itemId));
				props[1].name = "open";
				props[1].value = "false";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
				break;
			}
			switch (face)
			{
			case playerDigging::top:
				destY++;
				break;
			case playerDigging::bottom:
				destY--;
				break;
			case playerDigging::east:
				destX++;
				break;
			case playerDigging::west:
				destX--;
				break;
			case playerDigging::south:
				destZ++;
				break;
			case playerDigging::north:
				destZ--;
			}
			if (!p->world->checkCoordinates(destY))
				//destY out of world
				return;

			BlockState oldBlockState = getBlock(destX, destY, destZ);
			std::string oldBlockName = Registry::getBlock(oldBlockState.id);
			Block oldBlockId = (Block)Registry::getId(Registry::blockRegistry, oldBlockName);

			if (replaceableIndirect(oldBlockId))
			{
				BlockProperty* props = new BlockProperty[2];
				props[0].name = "facing";
				props[0].value = facingToString(get3DFacing(p, p->yaw, p->pitch, (Item)itemId));
				props[1].name = "open";
				props[1].value = "false";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
			}
			break;
		}
				}}

			//block that have the "facing" propery, dependent on what face it is being placed
			{ {
		case Item::minecraft_end_rod:
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
		{
			if (replaceableDirect(targetBlockId))
			{
				BlockProperty* props = new BlockProperty[1];
				props[0].name = "facing";
				switch (face)
				{
				case playerDigging::bottom:
					props[0].value = "down";
					break;
				case playerDigging::top:
					props[0].value = "up";
					break;
				case playerDigging::north:
					props[0].value = "north";
					break;
				case playerDigging::south:
					props[0].value = "south";
					break;
				case playerDigging::west:
					props[0].value = "west";
					break;
				case playerDigging::east:
					props[0].value = "east";
				}
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
				break;
			}
			blockFacing facing;
			switch (face)
			{
			case playerDigging::top:
				facing = blockFacing::up;
				destY++;
				break;
			case playerDigging::bottom:
				facing = blockFacing::down;
				destY--;
				break;
			case playerDigging::east:
				facing = blockFacing::east;
				destX++;
				break;
			case playerDigging::west:
				facing = blockFacing::west;
				destX--;
				break;
			case playerDigging::south:
				facing = blockFacing::south;
				destZ++;
				break;
			case playerDigging::north:
				facing = blockFacing::north;
				destZ--;
			}
			if (!p->world->checkCoordinates(destY))
				//destY out of world
				return;

			BlockState oldBlockState = getBlock(destX, destY, destZ);
			std::string oldBlockName = Registry::getBlock(oldBlockState.id);
			Block oldBlockId = (Block)Registry::getId(Registry::blockRegistry, oldBlockName);

			if (replaceableIndirect(oldBlockId))
			{
				BlockProperty* props = new BlockProperty[1];
				props[0].name = "facing";
				switch (facing)
				{
				case blockFacing::north:
					props[0].value = "north";
					break;
				case blockFacing::south:
					props[0].value = "south";
					break;
				case blockFacing::west:
					props[0].value = "west";
					break;
				case blockFacing::east:
					props[0].value = "east";
					break;
				case blockFacing::up:
					props[0].value = "up";
					break;
				case blockFacing::down:
					props[0].value = "down";
				}
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
			}
			break;
		}
		case Item::minecraft_amethyst_cluster:
		case Item::minecraft_large_amethyst_bud:
		case Item::minecraft_medium_amethyst_bud:
		case Item::minecraft_small_amethyst_bud:
		{
			if (replaceableDirect(targetBlockId))
			{
				BlockProperty* props = new BlockProperty[2];
				props[0].name = "facing";
				switch (face)
				{
				case playerDigging::bottom:
					props[0].value = "down";
					break;
				case playerDigging::top:
					props[0].value = "up";
					break;
				case playerDigging::north:
					props[0].value = "north";
					break;
				case playerDigging::south:
					props[0].value = "south";
					break;
				case playerDigging::west:
					props[0].value = "west";
					break;
				case playerDigging::east:
					props[0].value = "east";
				}
				props[1].name = "waterlogged";
				props[1].value = "false";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
				break;
			}
			blockFacing facing;
			switch (face)
			{
			case playerDigging::top:
				facing = blockFacing::up;
				destY++;
				break;
			case playerDigging::bottom:
				facing = blockFacing::down;
				destY--;
				break;
			case playerDigging::east:
				facing = blockFacing::east;
				destX++;
				break;
			case playerDigging::west:
				facing = blockFacing::west;
				destX--;
				break;
			case playerDigging::south:
				facing = blockFacing::south;
				destZ++;
				break;
			case playerDigging::north:
				facing = blockFacing::north;
				destZ--;
			}
			if (!p->world->checkCoordinates(destY))
				//destY out of world
				return;

			BlockState oldBlockState = getBlock(destX, destY, destZ);
			std::string oldBlockName = Registry::getBlock(oldBlockState.id);
			Block oldBlockId = (Block)Registry::getId(Registry::blockRegistry, oldBlockName);

			if (replaceableIndirect(oldBlockId))
			{
				BlockProperty* props = new BlockProperty[2];
				props[0].name = "facing";
				switch (facing)
				{
				case blockFacing::north:
					props[0].value = "north";
					break;
				case blockFacing::south:
					props[0].value = "south";
					break;
				case blockFacing::west:
					props[0].value = "west";
					break;
				case blockFacing::east:
					props[0].value = "east";
					break;
				case blockFacing::up:
					props[0].value = "up";
					break;
				case blockFacing::down:
					props[0].value = "down";
				}
				props[1].name = "waterlogged";
				props[1].value = oldBlockState.id == waterSurceBlockStateId ? "true" : "false";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
			}
			break;
		}
		case Item::minecraft_lightning_rod:
		{
			if (replaceableDirect(targetBlockId))
			{
				BlockProperty* props = new BlockProperty[3];
				props[0].name = "facing";
				switch (face)
				{
				case playerDigging::bottom:
					props[0].value = "down";
					break;
				case playerDigging::top:
					props[0].value = "up";
					break;
				case playerDigging::north:
					props[0].value = "north";
					break;
				case playerDigging::south:
					props[0].value = "south";
					break;
				case playerDigging::west:
					props[0].value = "west";
					break;
				case playerDigging::east:
					props[0].value = "east";
				}
				props[1].name = "waterlogged";
				props[1].value = "false";
				props[2].name = "powered";
				props[2].value = "false";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
				break;
			}
			blockFacing facing;
			switch (face)
			{
			case playerDigging::top:
				facing = blockFacing::up;
				destY++;
				break;
			case playerDigging::bottom:
				facing = blockFacing::down;
				destY--;
				break;
			case playerDigging::east:
				facing = blockFacing::east;
				destX++;
				break;
			case playerDigging::west:
				facing = blockFacing::west;
				destX--;
				break;
			case playerDigging::south:
				facing = blockFacing::south;
				destZ++;
				break;
			case playerDigging::north:
				facing = blockFacing::north;
				destZ--;
			}
			if (!p->world->checkCoordinates(destY))
				//destY out of world
				return;

			BlockState oldBlockState = getBlock(destX, destY, destZ);
			std::string oldBlockName = Registry::getBlock(oldBlockState.id);
			Block oldBlockId = (Block)Registry::getId(Registry::blockRegistry, oldBlockName);

			if (replaceableIndirect(oldBlockId))
			{
				BlockProperty* props = new BlockProperty[3];
				props[0].name = "facing";
				switch (facing)
				{
				case blockFacing::north:
					props[0].value = "north";
					break;
				case blockFacing::south:
					props[0].value = "south";
					break;
				case blockFacing::west:
					props[0].value = "west";
					break;
				case blockFacing::east:
					props[0].value = "east";
					break;
				case blockFacing::up:
					props[0].value = "up";
					break;
				case blockFacing::down:
					props[0].value = "down";
				}
				props[1].name = "waterlogged";
				props[1].value = oldBlockState.id == waterSurceBlockStateId ? "true" : "false";
				props[2].name = "powered";
				props[2].value = "false";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
			}
			break;
		}
				}}

			//blocks that need to have any block or liquid underneath
			{ {
		case Item::minecraft_white_carpet:
		case Item::minecraft_orange_carpet:
		case Item::minecraft_magenta_carpet:
		case Item::minecraft_light_blue_carpet:
		case Item::minecraft_yellow_carpet:
		case Item::minecraft_lime_carpet:
		case Item::minecraft_pink_carpet:
		case Item::minecraft_gray_carpet:
		case Item::minecraft_light_gray_carpet:
		case Item::minecraft_cyan_carpet:
		case Item::minecraft_purple_carpet:
		case Item::minecraft_blue_carpet:
		case Item::minecraft_brown_carpet:
		case Item::minecraft_green_carpet:
		case Item::minecraft_red_carpet:
		case Item::minecraft_black_carpet:
			if (replaceableDirect(targetBlockId))
			{
				int groundY = destY - 1;
				if (!p->world->checkCoordinates(groundY)) return;
				BlockState groundBlockState = getBlock(destX, groundY, destZ);
				if (groundBlockState == 0) break;
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId));
				break;
			}
			switch (face)
			{
			case playerDigging::top:
				destY++;
				break;
			case playerDigging::bottom:
				destY--;
				break;
			case playerDigging::east:
				destX++;
				break;
			case playerDigging::west:
				destX--;
				break;
			case playerDigging::south:
				destZ++;
				break;
			case playerDigging::north:
				destZ--;
			}
			if (!p->world->checkCoordinates(destY))
				//destY out of world
				return;

			BlockState oldBlockState = getBlock(destX, destY, destZ);
			std::string oldBlockName = Registry::getBlock(oldBlockState.id);
			Block oldBlockId = (Block)Registry::getId(Registry::blockRegistry, oldBlockName);

			if (replaceableIndirect(oldBlockId))
			{
				int groundY = destY - 1;
				if (!p->world->checkCoordinates(groundY)) return;
				BlockState groundBlockState = getBlock(destX, groundY, destZ);
				if (groundBlockState == 0) break;
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId));
			}
			break;
				}}

			//slabs ("type" state with "bottom", "top" or "double")
			{
				{
		case Item::minecraft_cut_copper_slab:
		case Item::minecraft_exposed_cut_copper_slab:
		case Item::minecraft_weathered_cut_copper_slab:
		case Item::minecraft_oxidized_cut_copper_slab:
		case Item::minecraft_waxed_cut_copper_slab:
		case Item::minecraft_waxed_exposed_cut_copper_slab:
		case Item::minecraft_waxed_weathered_cut_copper_slab:
		case Item::minecraft_waxed_oxidized_cut_copper_slab:
		case Item::minecraft_oak_slab:
		case Item::minecraft_spruce_slab:
		case Item::minecraft_birch_slab:
		case Item::minecraft_jungle_slab:
		case Item::minecraft_acacia_slab:
		case Item::minecraft_dark_oak_slab:
		case Item::minecraft_crimson_slab:
		case Item::minecraft_warped_slab:
		case Item::minecraft_stone_slab:
		case Item::minecraft_smooth_stone_slab:
		case Item::minecraft_sandstone_slab:
		case Item::minecraft_cut_sandstone_slab:
		case Item::minecraft_petrified_oak_slab:
		case Item::minecraft_cobblestone_slab:
		case Item::minecraft_brick_slab:
		case Item::minecraft_stone_brick_slab:
		case Item::minecraft_nether_brick_slab:
		case Item::minecraft_quartz_slab:
		case Item::minecraft_red_sandstone_slab:
		case Item::minecraft_cut_red_sandstone_slab:
		case Item::minecraft_purpur_slab:
		case Item::minecraft_prismarine_slab:
		case Item::minecraft_prismarine_brick_slab:
		case Item::minecraft_dark_prismarine_slab:
		case Item::minecraft_polished_granite_slab:
		case Item::minecraft_smooth_red_sandstone_slab:
		case Item::minecraft_mossy_stone_brick_slab:
		case Item::minecraft_polished_diorite_slab:
		case Item::minecraft_mossy_cobblestone_slab:
		case Item::minecraft_end_stone_brick_slab:
		case Item::minecraft_smooth_sandstone_slab:
		case Item::minecraft_smooth_quartz_slab:
		case Item::minecraft_granite_slab:
		case Item::minecraft_andesite_slab:
		case Item::minecraft_red_nether_brick_slab:
		case Item::minecraft_polished_andesite_slab:
		case Item::minecraft_diorite_slab:
		case Item::minecraft_cobbled_deepslate_slab:
		case Item::minecraft_polished_deepslate_slab:
		case Item::minecraft_deepslate_brick_slab:
		case Item::minecraft_deepslate_tile_slab:
		case Item::minecraft_blackstone_slab:
		case Item::minecraft_polished_blackstone_slab:
		case Item::minecraft_polished_blackstone_brick_slab:
		{
			//temp
			if (curY == .5f) message::play::send::chatMessage(p, Chat("Are you a robot?"), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));

			//name of the block to be placed
			std::string heldBlockName = Registry::getName(Registry::itemRegistry, itemId);
			//id of the block to be placed
			Block heldBlockId = (Block)Registry::getId(Registry::blockRegistry, heldBlockName);
			slabType type;

			//test and replace slabs directly
			if (replaceableDirect(targetBlockId))
			{
				BlockProperty* props = new BlockProperty[2];
				switch (face)
				{
				case playerDigging::top:
					props[0].value = "bottom";
					break;
				case playerDigging::bottom:
					props[0].value = "top";
					break;
				case playerDigging::south:
					props[0].value = curY < .5f ? "bottom" : "top";
					break;
				case playerDigging::east:
					props[0].value = curY < .5f ? "bottom" : "top";
					break;
				case playerDigging::west:
					props[0].value = curY < .5f ? "bottom" : "top";
					break;
				case playerDigging::north:
					props[0].value = curY < .5f ? "bottom" : "top";
				}
				props[0].name = "type";
				props[1].name = "waterlogged";
				props[1].value = "false";
				stateJson = &Registry::getBlockState(heldBlockName, props);
				delete[] props;
				break;
			}

			switch (face)
			{
			case playerDigging::top:
				//if the clicked block is already a slab, check for update slab (put in the same block)
				if (targetBlockId == heldBlockId && targetBlockState.getState("type") == "bottom")
				{
					targetBlockState.setState("type", "double");
					targetBlockState.setState("waterlogged", "false");
					stateJson = targetBlockState.getJsonState();
					break;
				}
				destY++;
				type = slabType::bottom;
				break;
			case playerDigging::bottom:
				//if the clicked block is already a slab, check for update slab (put in the same block)
				if (targetBlockId == heldBlockId && targetBlockState.getState("type") == "top")
				{
					targetBlockState.setState("type", "double");
					targetBlockState.setState("waterlogged", "false");
					stateJson = targetBlockState.getJsonState();
					break;
				}
				destY--;
				type = slabType::top;
				break;
			case playerDigging::south:
				destZ++;
				type = curY <= 0.5 ? slabType::bottom : slabType::top;
				break;
			case playerDigging::east:
				destX++;
				type = curY <= 0.5 ? slabType::bottom : slabType::top;
				break;
			case playerDigging::west:
				destX--;
				type = curY <= 0.5 ? slabType::bottom : slabType::top;
				break;
			case playerDigging::north:
				destZ--;
				type = curY <= 0.5 ? slabType::bottom : slabType::top;
			}

			if (!stateJson)
			{
				if (!checkCoordinates(destY))
					//block out of world
					return;

				BlockState oldBlockState = getBlock(destX, destY, destZ);
				std::string oldBlockName = Registry::getBlock(oldBlockState.id);
				Block oldBlockId = (Block)Registry::getId(Registry::blockRegistry, oldBlockName);

				if (heldBlockId == oldBlockId)
				{
					//already a slab of the same type there
					if (type == slabType::bottom)
					{
						//put on bottom
						if (oldBlockState.getState("type") == "top")
						{
							oldBlockState.setState("type", "double");
							oldBlockState.setState("waterlogged", "false");
							stateJson = oldBlockState.getJsonState();
						}
						else return;
					}
					else
					{
						//put on top
						if (oldBlockState.getState("type") == "bottom")
						{
							oldBlockState.setState("type", "double");
							oldBlockState.setState("waterlogged", "false");
							stateJson = oldBlockState.getJsonState();
						}
						else return;
					}
				}
				else
				{
					//not already a slab of the same type
					if (replaceableIndirect(oldBlockId))
					{
						BlockProperty* props = new BlockProperty[2];
						props[0].name = "type";
						props[0].value = type == slabType::bottom ? "bottom" : "top";
						props[1].name = "waterlogged";
						props[1].value = oldBlockState.id == waterSurceBlockStateId ? "true" : "false";
						stateJson = &Registry::getBlockState(heldBlockName, props);
						delete[] props;
					}
				}
			}
		}
		break;
				}
			}

			//blocks that have the state "axis" dependent on plcement face (like logs)
			{
				{
		case Item::minecraft_oak_log:
		case Item::minecraft_spruce_log:
		case Item::minecraft_birch_log:
		case Item::minecraft_jungle_log:
		case Item::minecraft_acacia_log:
		case Item::minecraft_dark_oak_log:
		case Item::minecraft_crimson_stem:
		case Item::minecraft_warped_stem:
		case Item::minecraft_stripped_oak_log:
		case Item::minecraft_stripped_spruce_log:
		case Item::minecraft_stripped_birch_log:
		case Item::minecraft_stripped_jungle_log:
		case Item::minecraft_stripped_acacia_log:
		case Item::minecraft_stripped_dark_oak_log:
		case Item::minecraft_stripped_crimson_stem:
		case Item::minecraft_stripped_warped_stem:
		case Item::minecraft_stripped_oak_wood:
		case Item::minecraft_stripped_spruce_wood:
		case Item::minecraft_stripped_birch_wood:
		case Item::minecraft_stripped_jungle_wood:
		case Item::minecraft_stripped_acacia_wood:
		case Item::minecraft_stripped_dark_oak_wood:
		case Item::minecraft_stripped_crimson_hyphae:
		case Item::minecraft_stripped_warped_hyphae:
		case Item::minecraft_oak_wood:
		case Item::minecraft_spruce_wood:
		case Item::minecraft_birch_wood:
		case Item::minecraft_jungle_wood:
		case Item::minecraft_acacia_wood:
		case Item::minecraft_dark_oak_wood:
		case Item::minecraft_crimson_hyphae:
		case Item::minecraft_warped_hyphae:
		case Item::minecraft_deepslate:
		case Item::minecraft_purpur_pillar:
		case Item::minecraft_basalt:
		case Item::minecraft_polished_basalt:
		case Item::minecraft_hay_block:
		case Item::minecraft_quartz_pillar:
		case Item::minecraft_bone_block:
		{
			if (replaceableDirect(targetBlockId))
			{
				BlockProperty* props = new BlockProperty[1];
				switch (face)
				{
				case playerDigging::top:
					props[0].value = "y";
					break;
				case playerDigging::bottom:
					props[0].value = "y";
					break;
				case playerDigging::east:
					props[0].value = "x";
					break;
				case playerDigging::west:
					props[0].value = "x";
					break;
				case playerDigging::south:
					props[0].value = "z";
					break;
				case playerDigging::north:
					props[0].value = "z";
				}
				props[0].name = "axis";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
				break;
			}
			logAxis axis;
			switch (face)
			{
			case playerDigging::top:
				destY++;
				axis = logAxis::y;
				break;
			case playerDigging::bottom:
				destY--;
				axis = logAxis::y;
				break;
			case playerDigging::east:
				destX++;
				axis = logAxis::x;
				break;
			case playerDigging::west:
				destX--;
				axis = logAxis::x;
				break;
			case playerDigging::south:
				destZ++;
				axis = logAxis::z;
				break;
			case playerDigging::north:
				destZ--;
				axis = logAxis::z;
			}
			if (!p->world->checkCoordinates(destY))
				//destY out of world
				return;


			BlockState oldBlockState = getBlock(destX, destY, destZ);
			std::string oldBlockName = Registry::getBlock(oldBlockState.id);
			Block oldBlockId = (Block)Registry::getId(Registry::blockRegistry, oldBlockName);

			if (replaceableIndirect(oldBlockId))
			{
				BlockProperty* props = new BlockProperty[1];
				switch (axis)
				{
				case logAxis::x:
					props[0].value = "x";
					break;
				case logAxis::y:
					props[0].value = "y";
					break;
				case logAxis::z:
					props[0].value = "z";
				}
				props[0].name = "axis";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
			}
		}
		break;
				}
			}

			//leaves
			{ {
		case Item::minecraft_oak_leaves:
		case Item::minecraft_spruce_leaves:
		case Item::minecraft_birch_leaves:
		case Item::minecraft_jungle_leaves:
		case Item::minecraft_acacia_leaves:
		case Item::minecraft_dark_oak_leaves:
		case Item::minecraft_azalea_leaves:
		case Item::minecraft_flowering_azalea_leaves:
			if (replaceableDirect(targetBlockId))
			{
				BlockProperty* props = new BlockProperty[2];
				props[0].name = "distance";
				props[0].value = "7";
				props[1].name = "persistent";
				props[1].value = "true";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
				break;
			}
			switch (face)
			{
			case playerDigging::top:
				destY++;
				break;
			case playerDigging::bottom:
				destY--;
				break;
			case playerDigging::east:
				destX++;
				break;
			case playerDigging::west:
				destX--;
				break;
			case playerDigging::south:
				destZ++;
				break;
			case playerDigging::north:
				destZ--;
			}
			if (!p->world->checkCoordinates(destY))
				//destY out of world
				return;

			BlockState oldBlockState = getBlock(destX, destY, destZ);
			std::string oldBlockName = Registry::getBlock(oldBlockState.id);
			Block oldBlockId = (Block)Registry::getId(Registry::blockRegistry, oldBlockName);

			if (replaceableIndirect(oldBlockId))
			{
				BlockProperty* props = new BlockProperty[2];
				props[0].name = "distance";
				props[0].value = "7";
				props[1].name = "persistent";
				props[1].value = "true";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
			}
			break;
				}}

			//buckets
			{
		case Item::minecraft_bucket:
		{
			break;
		}
		case Item::minecraft_water_bucket:
		{
			if (waterloggable(targetBlockId))
			{
				targetBlockState.setState("waterlogged", "true");
				setBlock(destX, destY, destZ, targetBlockState);
				//send the block to the client
				message::play::send::blockChange(p, loc, targetBlockState.id);
				return;
			}
			if (targetBlockId == Block::minecraft_cauldron)
			{
				message::play::send::chatMessage(p, Chat("No water in cauldrons allowed!", Chat::color::red), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0)); //temp
				//BlockProperty* props = new BlockProperty[1];
				//props[0].name = "level";
				//props[0].value = "3";
				//setBlock(destX, destY, destZ, Registry::getBlockState("minecraft:water_cauldron", props));
				//delete[] props;
				//setblock water_cauldron instead of the cauldron
				return;
			}
			switch (face)
			{
			case playerDigging::top:
				destY++;
				break;
			case playerDigging::bottom:
				destY--;
				break;
			case playerDigging::east:
				destX++;
				break;
			case playerDigging::west:
				destX--;
				break;
			case playerDigging::south:
				destZ++;
				break;
			case playerDigging::north:
				destZ--;
			}
			if (!p->world->checkCoordinates(destY))
				//destY out of world
				return;

			BlockState oldBlockState = getBlock(destX, destY, destZ);
			std::string oldBlockName = Registry::getBlock(oldBlockState.id);
			Block oldBlockId = (Block)Registry::getId(Registry::blockRegistry, oldBlockName);

			if (waterloggable(oldBlockId))
			{
				oldBlockState.setState("waterlogged", "true");
				setBlock(destX, destY, destZ, oldBlockState);
				//send the block to the client
				message::play::send::blockChange(p, Position(destX, destY + p->world->min_y, destZ), oldBlockState.id);
				return;
			}
			if (destroyedByWater(oldBlockId) || replaceableIndirect(oldBlockId)) stateJson = &Registry::getBlockState("minecraft:water");
			break;
		}
		case Item::minecraft_lava_bucket:
		{
			if (targetBlockId == Block::minecraft_cauldron)
			{
				message::play::send::chatMessage(p, Chat("No lava in cauldrons allowed!", Chat::color::red), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0)); //temp
				//BlockProperty* props = new BlockProperty[1];
				//props[0].name = "level";
				//props[0].value = "3";
				//setBlock(destX, destY, destZ, Registry::getBlockState("minecraft:water_cauldron", props));
				//delete[] props;
				//setblock water_cauldron instead of the cauldron
				return;
			}
			switch (face)
			{
			case playerDigging::top:
				destY++;
				break;
			case playerDigging::bottom:
				destY--;
				break;
			case playerDigging::east:
				destX++;
				break;
			case playerDigging::west:
				destX--;
				break;
			case playerDigging::south:
				destZ++;
				break;
			case playerDigging::north:
				destZ--;
			}
			if (!p->world->checkCoordinates(destY))
				//destY out of world
				return;

			BlockState oldBlockState = getBlock(destX, destY, destZ);
			std::string oldBlockName = Registry::getBlock(oldBlockState.id);
			Block oldBlockId = (Block)Registry::getId(Registry::blockRegistry, oldBlockName);

			if (destroyedByWater(oldBlockId) || replaceableIndirect(oldBlockId)) stateJson = &Registry::getBlockState("minecraft:lava");
			break;
		}
		case Item::minecraft_powder_snow_bucket:
		{
			if (targetBlockId == Block::minecraft_cauldron)
			{
				message::play::send::chatMessage(p, Chat("No powder snow in cauldrons allowed!", Chat::color::red), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0)); //temp
				//BlockProperty* props = new BlockProperty[1];
				//props[0].name = "level";
				//props[0].value = "3";
				//setBlock(destX, destY, destZ, Registry::getBlockState("minecraft:water_cauldron", props));
				//delete[] props;
				//setblock water_cauldron instead of the cauldron
				return;
			}
			switch (face)
			{
			case playerDigging::top:
				destY++;
				break;
			case playerDigging::bottom:
				destY--;
				break;
			case playerDigging::east:
				destX++;
				break;
			case playerDigging::west:
				destX--;
				break;
			case playerDigging::south:
				destZ++;
				break;
			case playerDigging::north:
				destZ--;
			}
			if (!p->world->checkCoordinates(destY))
				//destY out of world
				return;

			BlockState oldBlockState = getBlock(destX, destY, destZ);
			std::string oldBlockName = Registry::getBlock(oldBlockState.id);
			Block oldBlockId = (Block)Registry::getId(Registry::blockRegistry, oldBlockName);

			if (replaceableIndirect(oldBlockId)) stateJson = &Registry::getBlockState("minecraft:powder_snow");
			break;
		}
		case Item::minecraft_pufferfish_bucket:
		{
			break;
		}
		case Item::minecraft_salmon_bucket:
		{
			break;
		}
		case Item::minecraft_cod_bucket:
		{
			break;
		}
		case Item::minecraft_tropical_fish_bucket:
		{
			break;
		}
		case Item::minecraft_axolotl_bucket:
		{
			break;
		}
			}

			//trapdoors
			{
				{
		case Item::minecraft_iron_trapdoor:
		case Item::minecraft_oak_trapdoor:
		case Item::minecraft_spruce_trapdoor:
		case Item::minecraft_birch_trapdoor:
		case Item::minecraft_jungle_trapdoor:
		case Item::minecraft_acacia_trapdoor:
		case Item::minecraft_dark_oak_trapdoor:
		case Item::minecraft_crimson_trapdoor:
		case Item::minecraft_warped_trapdoor:
		{
			if (replaceableDirect(targetBlockId))
			{
				BlockProperty* props = new BlockProperty[5];
				props[0].name = "facing";
				props[1].name = "half";
				switch (face)
				{
				case playerDigging::top:
					props[1].value = "bottom";
					props[0].value = facingToString(getHorizontalFacing(p, p->yaw, (Item)itemId));
					break;
				case playerDigging::bottom:
					props[1].value = "top";
					props[0].value = facingToString(getHorizontalFacing(p, p->yaw, (Item)itemId));
					break;
				case playerDigging::east:
					props[1].value = curY < .5f ? "bottom" : "top";
					props[0].value = "east";
					break;
				case playerDigging::west:
					props[1].value = curY < .5f ? "bottom" : "top";
					props[0].value = "west";
					break;
				case playerDigging::south:
					props[1].value = curY < .5f ? "bottom" : "top";
					props[0].value = "south";
					break;
				case playerDigging::north:
					props[1].value = curY < .5f ? "bottom" : "top";
					props[0].value = "north";
				}
				props[2].name = "open";
				props[2].value = "false";
				props[3].name = "powered";
				props[3].value = "false";
				props[4].name = "waterlogged";
				props[4].value = "false";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
				break;
			}
			slabType half;
			blockFacing facing;
			switch (face)
			{
			case playerDigging::top:
				half = slabType::bottom;
				facing = getHorizontalFacing(p, p->yaw, (Item)itemId);
				destY++;
				break;
			case playerDigging::bottom:
				half = slabType::top;
				facing = getHorizontalFacing(p, p->yaw, (Item)itemId);
				destY--;
				break;
			case playerDigging::east:
				half = curY < .5f ? slabType::bottom : slabType::top;
				facing = blockFacing::east;
				destX++;
				break;
			case playerDigging::west:
				half = curY < .5f ? slabType::bottom : slabType::top;
				facing = blockFacing::west;
				destX--;
				break;
			case playerDigging::south:
				half = curY < .5f ? slabType::bottom : slabType::top;
				facing = blockFacing::south;
				destZ++;
				break;
			case playerDigging::north:
				half = curY < .5f ? slabType::bottom : slabType::top;
				facing = blockFacing::north;
				destZ--;
			}
			if (!checkCoordinates(destY))
				//destY out of world
				return;

			BlockState oldBlockState = getBlock(destX, destY, destZ);
			std::string oldBlockName = Registry::getBlock(oldBlockState.id);
			Block oldBlockId = (Block)Registry::getId(Registry::blockRegistry, oldBlockName);

			if (replaceableIndirect(oldBlockId))
			{
				BlockProperty* props = new BlockProperty[5];
				props[0].name = "facing";
				props[0].value = facingToString(facing);
				props[1].name = "half";
				props[1].value = half == slabType::bottom ? "bottom" : "top";
				props[2].name = "open";
				props[2].value = "false";
				props[3].name = "powered";
				props[3].value = "false";
				props[4].name = "waterlogged";
				props[4].value = oldBlockState.id == waterSurceBlockStateId ? "true" : "false";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				delete[] props;
			}
			break;
		}
				}
			}

			//doors
			{
				{
		case Item::minecraft_iron_door:
		case Item::minecraft_oak_door:
		case Item::minecraft_spruce_door:
		case Item::minecraft_birch_door:
		case Item::minecraft_jungle_door:
		case Item::minecraft_acacia_door:
		case Item::minecraft_dark_oak_door:
		case Item::minecraft_crimson_door:
		case Item::minecraft_warped_door:
		{
			//if the block on the top is outside world, skip
			if (replaceableDirect(targetBlockId))
			{
				if (!checkCoordinates(destY - 1) || !checkCoordinates(destY + 1))
					//door cannot have floor or the upper half is outside world
					break;
				BlockState floorState = getBlock(destX, destY - 1, destZ);
				Block floorBlock = stateToBlock(floorState);
				if (!canSupportDoor(floorBlock, floorState))
					//floor block cannot support a door
					break;
				if (!replaceableIndirect(stateToBlock(getBlock(destX, destY + 1, destZ))))
					//canno place top part
					break;

				BlockProperty* props = new BlockProperty[5];
				props[0].name = "facing";
				blockFacing facing = getHorizontalFacing(p, p->yaw, (Item)itemId);
				props[0].value = facingToString(facing);
				props[1].name = "half";
				props[1].value = "lower";
				props[2].name = "hinge";
				props[2].value = hingeToString(chooseDoorHinge(facing, this, destX, destY, destZ, curX, curZ));
				props[3].name = "open";
				props[3].value = "false";
				props[4].name = "powered";
				props[4].value = "false";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				props[1].value = "upper";
				setBlock(destX, destY + 1, destZ, Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props));
				delete[] props;
				break;
			}
			switch (face)
			{
			case playerDigging::top:
				destY++;
				if (!checkCoordinates(destY + 1) || !checkCoordinates(destY)) return;
				break;
			case playerDigging::bottom:
				//probably not needed
				destY--;
				if (!checkCoordinates(destY - 1) || !checkCoordinates(destY)) return;
				break;
			case playerDigging::east:
				if (!checkCoordinates(destY - 1) || !checkCoordinates(destY + 1)) return;
				destX++;
				break;
			case playerDigging::west:
				if (!checkCoordinates(destY - 1) || !checkCoordinates(destY + 1)) return;
				destX--;
				break;
			case playerDigging::south:
				if (!checkCoordinates(destY - 1) || !checkCoordinates(destY + 1)) return;
				destZ++;
				break;
			case playerDigging::north:
				if (!checkCoordinates(destY - 1) || !checkCoordinates(destY + 1)) return;
				destZ--;
			}

			BlockState oldBlockState = getBlock(destX, destY, destZ);
			std::string oldBlockName = Registry::getBlock(oldBlockState.id);
			Block oldBlockId = (Block)Registry::getId(Registry::blockRegistry, oldBlockName);

			if (replaceableIndirect(oldBlockId) && replaceableIndirect(stateToBlock(getBlock(destX, destY + 1, destZ))))
			{
				BlockState floorState = getBlock(destX, destY - 1, destZ);
				Block floorBlock = stateToBlock(floorState);
				if (!canSupportDoor(floorBlock, floorState))
					//floor block cannot support a door
					break;
				BlockProperty* props = new BlockProperty[5];
				props[0].name = "facing";
				blockFacing facing = getHorizontalFacing(p, p->yaw, (Item)itemId);
				props[0].value = facingToString(facing);
				props[1].name = "half";
				props[1].value = "lower";
				props[2].name = "hinge";
				props[2].value = hingeToString(chooseDoorHinge(facing, this, destX, destY, destZ, curX, curZ));
				props[3].name = "open";
				props[3].value = "false";
				props[4].name = "powered";
				props[4].value = "false";
				stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
				props[1].value = "upper";
				setBlock(destX, destY + 1, destZ, Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props));
				delete[] props;
			}
			break;
		}
		break;
				}
			}

			//beds
			{
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
			break;
			}

			//special items
			{
				{
		case Item::minecraft_ender_eye:
			if (targetBlockId != Block::minecraft_end_portal_frame) break;
			std::string hasEye = targetBlockState.getState("eye");
			if (hasEye == "true") break;
			targetBlockState.setState("eye", "true");
			stateJson = targetBlockState.getJsonState();
			message::play::send::blockChange(p, loc, targetBlockState.id);
			//add sound and particles
			//add actual opening of the end portal if all frames are filled with eyes
			break;
				}
			}

			//unhandled
			{
		case Item::minecraft_grass_block:
		case Item::minecraft_podzol:
		case Item::minecraft_mycelium:

		case Item::minecraft_beacon:

		case Item::minecraft_dead_bush:
		case Item::minecraft_seagrass:
		case Item::minecraft_sea_pickle:

		case Item::minecraft_spore_blossom:
		case Item::minecraft_brown_mushroom:
		case Item::minecraft_red_mushroom:
		case Item::minecraft_crimson_fungus:
		case Item::minecraft_warped_fungus:
		case Item::minecraft_crimson_roots:
		case Item::minecraft_warped_roots:
		case Item::minecraft_nether_sprouts:
		case Item::minecraft_weeping_vines:
		case Item::minecraft_twisting_vines:
		case Item::minecraft_sugar_cane:
		case Item::minecraft_kelp:
		case Item::minecraft_moss_carpet:
		case Item::minecraft_hanging_roots:
		case Item::minecraft_big_dripleaf:
		case Item::minecraft_small_dripleaf:
		case Item::minecraft_bamboo:
		case Item::minecraft_torch:
		case Item::minecraft_chorus_plant:
		case Item::minecraft_chorus_flower:
		case Item::minecraft_oak_fence:
		case Item::minecraft_spruce_fence:
		case Item::minecraft_birch_fence:
		case Item::minecraft_jungle_fence:
		case Item::minecraft_acacia_fence:
		case Item::minecraft_dark_oak_fence:
		case Item::minecraft_crimson_fence:
		case Item::minecraft_warped_fence:
		case Item::minecraft_chest:
		case Item::minecraft_farmland:
		case Item::minecraft_ladder:
		case Item::minecraft_snow:
		case Item::minecraft_cactus:
		case Item::minecraft_soul_torch:
		case Item::minecraft_iron_bars:
		case Item::minecraft_chain:
		case Item::minecraft_glass_pane:
		case Item::minecraft_vine:
		case Item::minecraft_glow_lichen:
		case Item::minecraft_lily_pad:
		case Item::minecraft_nether_brick_fence:
		case Item::minecraft_dragon_egg:
		case Item::minecraft_ender_chest:
		case Item::minecraft_command_block:
		case Item::minecraft_cobblestone_wall:
		case Item::minecraft_mossy_cobblestone_wall:
		case Item::minecraft_brick_wall:
		case Item::minecraft_prismarine_wall:
		case Item::minecraft_red_sandstone_wall:
		case Item::minecraft_mossy_stone_brick_wall:
		case Item::minecraft_granite_wall:
		case Item::minecraft_stone_brick_wall:
		case Item::minecraft_nether_brick_wall:
		case Item::minecraft_andesite_wall:
		case Item::minecraft_red_nether_brick_wall:
		case Item::minecraft_sandstone_wall:
		case Item::minecraft_end_stone_brick_wall:
		case Item::minecraft_diorite_wall:
		case Item::minecraft_blackstone_wall:
		case Item::minecraft_polished_blackstone_wall:
		case Item::minecraft_polished_blackstone_brick_wall:
		case Item::minecraft_cobbled_deepslate_wall:
		case Item::minecraft_polished_deepslate_wall:
		case Item::minecraft_deepslate_brick_wall:
		case Item::minecraft_deepslate_tile_wall:
		case Item::minecraft_light:
		case Item::minecraft_structure_void:
		case Item::minecraft_dirt_path:
		case Item::minecraft_sunflower:
		case Item::minecraft_lilac:
		case Item::minecraft_rose_bush:
		case Item::minecraft_peony:
		case Item::minecraft_tall_grass:
		case Item::minecraft_large_fern:
		case Item::minecraft_white_stained_glass_pane:
		case Item::minecraft_orange_stained_glass_pane:
		case Item::minecraft_magenta_stained_glass_pane:
		case Item::minecraft_light_blue_stained_glass_pane:
		case Item::minecraft_yellow_stained_glass_pane:
		case Item::minecraft_lime_stained_glass_pane:
		case Item::minecraft_pink_stained_glass_pane:
		case Item::minecraft_gray_stained_glass_pane:
		case Item::minecraft_light_gray_stained_glass_pane:
		case Item::minecraft_cyan_stained_glass_pane:
		case Item::minecraft_purple_stained_glass_pane:
		case Item::minecraft_blue_stained_glass_pane:
		case Item::minecraft_brown_stained_glass_pane:
		case Item::minecraft_green_stained_glass_pane:
		case Item::minecraft_red_stained_glass_pane:
		case Item::minecraft_black_stained_glass_pane:
		case Item::minecraft_repeating_command_block:
		case Item::minecraft_chain_command_block:
		case Item::minecraft_turtle_egg:
		case Item::minecraft_tube_coral:
		case Item::minecraft_brain_coral:
		case Item::minecraft_bubble_coral:
		case Item::minecraft_fire_coral:
		case Item::minecraft_horn_coral:
		case Item::minecraft_dead_brain_coral:
		case Item::minecraft_dead_bubble_coral:
		case Item::minecraft_dead_fire_coral:
		case Item::minecraft_dead_horn_coral:
		case Item::minecraft_dead_tube_coral:
		case Item::minecraft_tube_coral_fan:
		case Item::minecraft_brain_coral_fan:
		case Item::minecraft_bubble_coral_fan:
		case Item::minecraft_fire_coral_fan:
		case Item::minecraft_horn_coral_fan:
		case Item::minecraft_dead_tube_coral_fan:
		case Item::minecraft_dead_brain_coral_fan:
		case Item::minecraft_dead_bubble_coral_fan:
		case Item::minecraft_dead_fire_coral_fan:
		case Item::minecraft_dead_horn_coral_fan:
		case Item::minecraft_conduit:
		case Item::minecraft_scaffolding:
		case Item::minecraft_redstone:
		case Item::minecraft_redstone_torch:
		case Item::minecraft_repeater:
		case Item::minecraft_comparator:
		case Item::minecraft_hopper:
		case Item::minecraft_lectern:
		case Item::minecraft_lever:
		case Item::minecraft_daylight_detector:
		case Item::minecraft_sculk_sensor:
		case Item::minecraft_tripwire_hook:
		case Item::minecraft_trapped_chest:
		case Item::minecraft_stone_button:
		case Item::minecraft_polished_blackstone_button:
		case Item::minecraft_oak_button:
		case Item::minecraft_spruce_button:
		case Item::minecraft_birch_button:
		case Item::minecraft_jungle_button:
		case Item::minecraft_acacia_button:
		case Item::minecraft_dark_oak_button:
		case Item::minecraft_crimson_button:
		case Item::minecraft_warped_button:
		case Item::minecraft_stone_pressure_plate:
		case Item::minecraft_polished_blackstone_pressure_plate:
		case Item::minecraft_light_weighted_pressure_plate:
		case Item::minecraft_heavy_weighted_pressure_plate:
		case Item::minecraft_oak_pressure_plate:
		case Item::minecraft_spruce_pressure_plate:
		case Item::minecraft_birch_pressure_plate:
		case Item::minecraft_jungle_pressure_plate:
		case Item::minecraft_acacia_pressure_plate:
		case Item::minecraft_dark_oak_pressure_plate:
		case Item::minecraft_crimson_pressure_plate:
		case Item::minecraft_warped_pressure_plate:
		case Item::minecraft_oak_fence_gate:
		case Item::minecraft_spruce_fence_gate:
		case Item::minecraft_birch_fence_gate:
		case Item::minecraft_jungle_fence_gate:
		case Item::minecraft_acacia_fence_gate:
		case Item::minecraft_dark_oak_fence_gate:
		case Item::minecraft_crimson_fence_gate:
		case Item::minecraft_warped_fence_gate:
		case Item::minecraft_powered_rail:
		case Item::minecraft_detector_rail:
		case Item::minecraft_rail:
		case Item::minecraft_activator_rail:
		case Item::minecraft_brown_mushroom_block:
		case Item::minecraft_red_mushroom_block:
		case Item::minecraft_mushroom_stem:
		case Item::minecraft_spawner:
		case Item::minecraft_note_block:
			break;
			}

		case Item::minecraft_redstone_lamp:
		case Item::minecraft_sweet_berries:
		case Item::minecraft_flower_pot:

		case Item::minecraft_minecart:
		case Item::minecraft_chest_minecart:
		case Item::minecraft_furnace_minecart:
		case Item::minecraft_tnt_minecart:
		case Item::minecraft_hopper_minecart:
		case Item::minecraft_carrot_on_a_stick:
		case Item::minecraft_warped_fungus_on_a_stick:
		case Item::minecraft_elytra:

		case Item::minecraft_oak_boat:
		case Item::minecraft_spruce_boat:
		case Item::minecraft_birch_boat:
		case Item::minecraft_jungle_boat:
		case Item::minecraft_acacia_boat:
		case Item::minecraft_dark_oak_boat:

		case Item::minecraft_structure_block:
		case Item::minecraft_jigsaw:
		case Item::minecraft_turtle_helmet:
		case Item::minecraft_scute:
		case Item::minecraft_flint_and_steel:
		case Item::minecraft_apple:
		case Item::minecraft_bow:
		case Item::minecraft_arrow:
		case Item::minecraft_coal:
		case Item::minecraft_charcoal:
		case Item::minecraft_diamond:
		case Item::minecraft_emerald:
		case Item::minecraft_lapis_lazuli:
		case Item::minecraft_quartz:
		case Item::minecraft_amethyst_shard:
		case Item::minecraft_raw_iron:
		case Item::minecraft_iron_ingot:
		case Item::minecraft_raw_copper:
		case Item::minecraft_copper_ingot:
		case Item::minecraft_raw_gold:
		case Item::minecraft_gold_ingot:
		case Item::minecraft_netherite_ingot:
		case Item::minecraft_netherite_scrap:

		case Item::minecraft_wooden_sword:
		case Item::minecraft_wooden_shovel:
		case Item::minecraft_wooden_pickaxe:
		case Item::minecraft_wooden_axe:
		case Item::minecraft_wooden_hoe:
		case Item::minecraft_stone_sword:
		case Item::minecraft_stone_shovel:
		case Item::minecraft_stone_pickaxe:
		case Item::minecraft_stone_axe:
		case Item::minecraft_stone_hoe:
		case Item::minecraft_golden_sword:
		case Item::minecraft_golden_shovel:
		case Item::minecraft_golden_pickaxe:
		case Item::minecraft_golden_axe:
		case Item::minecraft_golden_hoe:
		case Item::minecraft_iron_sword:
		case Item::minecraft_iron_shovel:
		case Item::minecraft_iron_pickaxe:
		case Item::minecraft_iron_axe:
		case Item::minecraft_iron_hoe:
		case Item::minecraft_diamond_sword:
		case Item::minecraft_diamond_shovel:
		case Item::minecraft_diamond_pickaxe:
		case Item::minecraft_diamond_axe:
		case Item::minecraft_diamond_hoe:
		case Item::minecraft_netherite_sword:
		case Item::minecraft_netherite_shovel:
		case Item::minecraft_netherite_pickaxe:
		case Item::minecraft_netherite_axe:
		case Item::minecraft_netherite_hoe:

		case Item::minecraft_stick:
		case Item::minecraft_bowl:
		case Item::minecraft_mushroom_stew:
		case Item::minecraft_string:
		case Item::minecraft_feather:
		case Item::minecraft_gunpowder:
		case Item::minecraft_wheat_seeds:
		case Item::minecraft_wheat:
		case Item::minecraft_bread:
		case Item::minecraft_leather_helmet:
		case Item::minecraft_leather_chestplate:
		case Item::minecraft_leather_leggings:
		case Item::minecraft_leather_boots:
		case Item::minecraft_chainmail_helmet:
		case Item::minecraft_chainmail_chestplate:
		case Item::minecraft_chainmail_leggings:
		case Item::minecraft_chainmail_boots:
		case Item::minecraft_iron_helmet:
		case Item::minecraft_iron_chestplate:
		case Item::minecraft_iron_leggings:
		case Item::minecraft_iron_boots:
		case Item::minecraft_diamond_helmet:
		case Item::minecraft_diamond_chestplate:
		case Item::minecraft_diamond_leggings:
		case Item::minecraft_diamond_boots:
		case Item::minecraft_golden_helmet:
		case Item::minecraft_golden_chestplate:
		case Item::minecraft_golden_leggings:
		case Item::minecraft_golden_boots:
		case Item::minecraft_netherite_helmet:
		case Item::minecraft_netherite_chestplate:
		case Item::minecraft_netherite_leggings:
		case Item::minecraft_netherite_boots:
		case Item::minecraft_flint:
		case Item::minecraft_porkchop:
		case Item::minecraft_cooked_porkchop:
		case Item::minecraft_painting:
		case Item::minecraft_golden_apple:
		case Item::minecraft_enchanted_golden_apple:

		case Item::minecraft_oak_sign:
		case Item::minecraft_spruce_sign:
		case Item::minecraft_birch_sign:
		case Item::minecraft_jungle_sign:
		case Item::minecraft_acacia_sign:
		case Item::minecraft_dark_oak_sign:
		case Item::minecraft_crimson_sign:
		case Item::minecraft_warped_sign:

		case Item::minecraft_snowball:
		case Item::minecraft_leather:
		case Item::minecraft_milk_bucket:

		case Item::minecraft_brick:
		case Item::minecraft_clay_ball:
		case Item::minecraft_paper:
		case Item::minecraft_book:
		case Item::minecraft_slime_ball:
		case Item::minecraft_egg:
		case Item::minecraft_compass:
		case Item::minecraft_bundle:
		case Item::minecraft_fishing_rod:
		case Item::minecraft_clock:
		case Item::minecraft_spyglass:
		case Item::minecraft_glowstone_dust:
		case Item::minecraft_cod:
		case Item::minecraft_salmon:
		case Item::minecraft_tropical_fish:
		case Item::minecraft_pufferfish:
		case Item::minecraft_cooked_cod:
		case Item::minecraft_cooked_salmon:
		case Item::minecraft_ink_sac:
		case Item::minecraft_glow_ink_sac:
		case Item::minecraft_cocoa_beans:
		case Item::minecraft_white_dye:
		case Item::minecraft_orange_dye:
		case Item::minecraft_magenta_dye:
		case Item::minecraft_light_blue_dye:
		case Item::minecraft_yellow_dye:
		case Item::minecraft_lime_dye:
		case Item::minecraft_pink_dye:
		case Item::minecraft_gray_dye:
		case Item::minecraft_light_gray_dye:
		case Item::minecraft_cyan_dye:
		case Item::minecraft_purple_dye:
		case Item::minecraft_blue_dye:
		case Item::minecraft_brown_dye:
		case Item::minecraft_green_dye:
		case Item::minecraft_red_dye:
		case Item::minecraft_black_dye:
		case Item::minecraft_bone_meal:
		case Item::minecraft_bone:
		case Item::minecraft_sugar:
		case Item::minecraft_cake:

		case Item::minecraft_cookie:
		case Item::minecraft_filled_map:
		case Item::minecraft_shears:
		case Item::minecraft_melon_slice:
		case Item::minecraft_dried_kelp:
		case Item::minecraft_pumpkin_seeds:
		case Item::minecraft_melon_seeds:
		case Item::minecraft_beef:
		case Item::minecraft_cooked_beef:
		case Item::minecraft_chicken:
		case Item::minecraft_cooked_chicken:
		case Item::minecraft_rotten_flesh:
		case Item::minecraft_ender_pearl:
		case Item::minecraft_blaze_rod:
		case Item::minecraft_ghast_tear:
		case Item::minecraft_gold_nugget:
		case Item::minecraft_nether_wart:
		case Item::minecraft_potion:
		case Item::minecraft_glass_bottle:
		case Item::minecraft_spider_eye:
		case Item::minecraft_fermented_spider_eye:
		case Item::minecraft_blaze_powder:
		case Item::minecraft_magma_cream:
		case Item::minecraft_brewing_stand:
		case Item::minecraft_glistering_melon_slice:
		case Item::minecraft_axolotl_spawn_egg:
		case Item::minecraft_bat_spawn_egg:
		case Item::minecraft_bee_spawn_egg:
		case Item::minecraft_blaze_spawn_egg:
		case Item::minecraft_cat_spawn_egg:
		case Item::minecraft_cave_spider_spawn_egg:
		case Item::minecraft_chicken_spawn_egg:
		case Item::minecraft_cod_spawn_egg:
		case Item::minecraft_cow_spawn_egg:
		case Item::minecraft_creeper_spawn_egg:
		case Item::minecraft_dolphin_spawn_egg:
		case Item::minecraft_donkey_spawn_egg:
		case Item::minecraft_drowned_spawn_egg:
		case Item::minecraft_elder_guardian_spawn_egg:
		case Item::minecraft_enderman_spawn_egg:
		case Item::minecraft_endermite_spawn_egg:
		case Item::minecraft_evoker_spawn_egg:
		case Item::minecraft_fox_spawn_egg:
		case Item::minecraft_ghast_spawn_egg:
		case Item::minecraft_glow_squid_spawn_egg:
		case Item::minecraft_goat_spawn_egg:
		case Item::minecraft_guardian_spawn_egg:
		case Item::minecraft_hoglin_spawn_egg:
		case Item::minecraft_horse_spawn_egg:
		case Item::minecraft_husk_spawn_egg:
		case Item::minecraft_llama_spawn_egg:
		case Item::minecraft_magma_cube_spawn_egg:
		case Item::minecraft_mooshroom_spawn_egg:
		case Item::minecraft_mule_spawn_egg:
		case Item::minecraft_ocelot_spawn_egg:
		case Item::minecraft_panda_spawn_egg:
		case Item::minecraft_parrot_spawn_egg:
		case Item::minecraft_phantom_spawn_egg:
		case Item::minecraft_pig_spawn_egg:
		case Item::minecraft_piglin_spawn_egg:
		case Item::minecraft_piglin_brute_spawn_egg:
		case Item::minecraft_pillager_spawn_egg:
		case Item::minecraft_polar_bear_spawn_egg:
		case Item::minecraft_pufferfish_spawn_egg:
		case Item::minecraft_rabbit_spawn_egg:
		case Item::minecraft_ravager_spawn_egg:
		case Item::minecraft_salmon_spawn_egg:
		case Item::minecraft_sheep_spawn_egg:
		case Item::minecraft_shulker_spawn_egg:
		case Item::minecraft_silverfish_spawn_egg:
		case Item::minecraft_skeleton_spawn_egg:
		case Item::minecraft_skeleton_horse_spawn_egg:
		case Item::minecraft_slime_spawn_egg:
		case Item::minecraft_spider_spawn_egg:
		case Item::minecraft_squid_spawn_egg:
		case Item::minecraft_stray_spawn_egg:
		case Item::minecraft_strider_spawn_egg:
		case Item::minecraft_trader_llama_spawn_egg:
		case Item::minecraft_tropical_fish_spawn_egg:
		case Item::minecraft_turtle_spawn_egg:
		case Item::minecraft_vex_spawn_egg:
		case Item::minecraft_villager_spawn_egg:
		case Item::minecraft_vindicator_spawn_egg:
		case Item::minecraft_wandering_trader_spawn_egg:
		case Item::minecraft_witch_spawn_egg:
		case Item::minecraft_wither_skeleton_spawn_egg:
		case Item::minecraft_wolf_spawn_egg:
		case Item::minecraft_zoglin_spawn_egg:
		case Item::minecraft_zombie_spawn_egg:
		case Item::minecraft_zombie_horse_spawn_egg:
		case Item::minecraft_zombie_villager_spawn_egg:
		case Item::minecraft_zombified_piglin_spawn_egg:
		case Item::minecraft_experience_bottle:
		case Item::minecraft_fire_charge:
		case Item::minecraft_writable_book:
		case Item::minecraft_written_book:
		case Item::minecraft_item_frame:
		case Item::minecraft_glow_item_frame:
		case Item::minecraft_carrot:
		case Item::minecraft_potato:
		case Item::minecraft_baked_potato:
		case Item::minecraft_poisonous_potato:
		case Item::minecraft_map:
		case Item::minecraft_golden_carrot:
		case Item::minecraft_skeleton_skull:
		case Item::minecraft_wither_skeleton_skull:
		case Item::minecraft_player_head:
		case Item::minecraft_zombie_head:
		case Item::minecraft_creeper_head:
		case Item::minecraft_dragon_head:
		case Item::minecraft_nether_star:
		case Item::minecraft_pumpkin_pie:
		case Item::minecraft_firework_rocket:
		case Item::minecraft_firework_star:
		case Item::minecraft_enchanted_book:
		case Item::minecraft_nether_brick:
		case Item::minecraft_prismarine_shard:
		case Item::minecraft_prismarine_crystals:
		case Item::minecraft_rabbit:
		case Item::minecraft_cooked_rabbit:
		case Item::minecraft_rabbit_stew:
		case Item::minecraft_rabbit_foot:
		case Item::minecraft_rabbit_hide:
		case Item::minecraft_armor_stand:
		case Item::minecraft_iron_horse_armor:
		case Item::minecraft_golden_horse_armor:
		case Item::minecraft_diamond_horse_armor:
		case Item::minecraft_leather_horse_armor:
		case Item::minecraft_lead:
		case Item::minecraft_name_tag:
		case Item::minecraft_command_block_minecart:
		case Item::minecraft_mutton:
		case Item::minecraft_cooked_mutton:

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

		case Item::minecraft_end_crystal:
		case Item::minecraft_chorus_fruit:
		case Item::minecraft_popped_chorus_fruit:
		case Item::minecraft_beetroot:
		case Item::minecraft_beetroot_seeds:
		case Item::minecraft_beetroot_soup:
		case Item::minecraft_dragon_breath:
		case Item::minecraft_splash_potion:
		case Item::minecraft_spectral_arrow:
		case Item::minecraft_tipped_arrow:
		case Item::minecraft_lingering_potion:
		case Item::minecraft_shield:
		case Item::minecraft_totem_of_undying:
		case Item::minecraft_shulker_shell:
		case Item::minecraft_iron_nugget:
		case Item::minecraft_knowledge_book:
		case Item::minecraft_debug_stick:
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
		case Item::minecraft_music_disc_pigstep:
		case Item::minecraft_trident:
		case Item::minecraft_phantom_membrane:
		case Item::minecraft_nautilus_shell:
		case Item::minecraft_heart_of_the_sea:
		case Item::minecraft_crossbow:
		case Item::minecraft_suspicious_stew:
		case Item::minecraft_flower_banner_pattern:
		case Item::minecraft_creeper_banner_pattern:
		case Item::minecraft_skull_banner_pattern:
		case Item::minecraft_mojang_banner_pattern:
		case Item::minecraft_globe_banner_pattern:
		case Item::minecraft_piglin_banner_pattern:
		case Item::minecraft_composter:
		case Item::minecraft_grindstone:
		case Item::minecraft_bell:
		case Item::minecraft_lantern:
		case Item::minecraft_soul_lantern:
		case Item::minecraft_glow_berries:
		case Item::minecraft_campfire:
		case Item::minecraft_soul_campfire:
		case Item::minecraft_honeycomb:
		case Item::minecraft_honey_bottle:
		case Item::minecraft_respawn_anchor:
		case Item::minecraft_candle:
		case Item::minecraft_white_candle:
		case Item::minecraft_orange_candle:
		case Item::minecraft_magenta_candle:
		case Item::minecraft_light_blue_candle:
		case Item::minecraft_yellow_candle:
		case Item::minecraft_lime_candle:
		case Item::minecraft_pink_candle:
		case Item::minecraft_gray_candle:
		case Item::minecraft_light_gray_candle:
		case Item::minecraft_cyan_candle:
		case Item::minecraft_purple_candle:
		case Item::minecraft_blue_candle:
		case Item::minecraft_brown_candle:
		case Item::minecraft_green_candle:
		case Item::minecraft_red_candle:
		case Item::minecraft_black_candle:
		case Item::minecraft_pointed_dripstone:
			break;
		}
	}
	catch (...)
	{
		message::play::send::chatMessage(p, Chat("Debug: error occured in setBlockByItem: switch", Chat::color::red), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
		throw;
	}

	if (stateJson)
	{
		setBlock(destX, destY, destZ, stateJson);
		Position destLoc = Position(destX, destY + p->world->min_y, destZ);
		for (Player* seener : players) if (seener != p && seener->positionInRange(loc)) message::play::send::blockChange(seener, destLoc, (*stateJson)["id"].iValue());
	}
	else message::play::send::chatMessage(p, Chat("Debug: setBlockByItem: no block placed", Chat::color::red), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
}