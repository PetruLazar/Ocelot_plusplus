#include "../world.h"
#include "../types/item.h"
#include "../player/message.h"

enum class slabType : Byte
{
	bottom,
	top,
	dbl
};

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

	try
	{
		switch ((Item)itemId)
		{
			//unplaceable items
			{
		case Item::minecraft_air:
			return;
			}

			//block with no states depending on placement
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
			//do not replace any block, only specific blocks
			//if (!getBlock(destX,destY,destZ).id) return;
			stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId));
			break;
			}

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
			message::play::send::chatMessage(p, Chat("No stairs allowed!", Chat::color::red), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
			return;
			}

			//slabs
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
		{
			//name of the block to be placed
			std::string heldBlockName = Registry::getName(Registry::itemRegistry, itemId);
			//id of the block to be placed
			Block heldBlockId = (Block)Registry::getId(Registry::blockRegistry, heldBlockName);
			slabType type;

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
					//test for waterlogged
					BlockProperty* props = new BlockProperty[2];
					props[0].name = "type";
					if (type == slabType::bottom) props[0].value = "bottom";
					else props[0].value = "top";
					props[1].name = "waterlogged";
					props[1].value = "false";
					stateJson = &Registry::getBlockState(heldBlockName, props);
				}
			}
		}
		break;
			}

			//full block that depend on the face clicked (like logs)
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
			BlockProperty* props = new BlockProperty[1];
			props[0].name = "axis";
			switch (face)
			{
			case playerDigging::top:
				destY++;
				props[0].value = "y";
				break;
			case playerDigging::bottom:
				destY--;
				props[0].value = "y";
				break;
			case playerDigging::east:
				destX++;
				props[0].value = "x";
				break;
			case playerDigging::west:
				destX--;
				props[0].value = "x";
				break;
			case playerDigging::south:
				destZ++;
				props[0].value = "z";
				break;
			case playerDigging::north:
				destZ--;
				props[0].value = "z";
			}
			stateJson = &Registry::getBlockState(Registry::getName(Registry::itemRegistry, itemId), props);
			delete[] props;
			if (!p->world->checkCoordinates(destY))
				//destY out of world
				return;
		}
		break;
			}

			//leaves
			{
		case Item::minecraft_oak_leaves:
		case Item::minecraft_spruce_leaves:
		case Item::minecraft_birch_leaves:
		case Item::minecraft_jungle_leaves:
		case Item::minecraft_acacia_leaves:
		case Item::minecraft_dark_oak_leaves:
		case Item::minecraft_azalea_leaves:
		case Item::minecraft_flowering_azalea_leaves:
			break;
			}

			//unhandled
			{
		case Item::minecraft_grass_block:
		case Item::minecraft_podzol:
		case Item::minecraft_grass:
		case Item::minecraft_fern:
		case Item::minecraft_azalea:
		case Item::minecraft_flowering_azalea:
		case Item::minecraft_dead_bush:
		case Item::minecraft_seagrass:
		case Item::minecraft_sea_pickle:
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
		case Item::minecraft_cornflower:
		case Item::minecraft_lily_of_the_valley:
		case Item::minecraft_wither_rose:
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
		case Item::minecraft_moss_block:
		case Item::minecraft_hanging_roots:
		case Item::minecraft_big_dripleaf:
		case Item::minecraft_small_dripleaf:
		case Item::minecraft_bamboo:
		case Item::minecraft_torch:
		case Item::minecraft_end_rod:
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
		case Item::minecraft_furnace:
		case Item::minecraft_ladder:
		case Item::minecraft_snow:
		case Item::minecraft_cactus:
		case Item::minecraft_carved_pumpkin:
		case Item::minecraft_jack_o_lantern:
		case Item::minecraft_soul_torch:
		case Item::minecraft_iron_bars:
		case Item::minecraft_chain:
		case Item::minecraft_glass_pane:
		case Item::minecraft_vine:
		case Item::minecraft_glow_lichen:
		case Item::minecraft_mycelium:
		case Item::minecraft_lily_pad:
		case Item::minecraft_nether_brick_fence:
		case Item::minecraft_enchanting_table:
		case Item::minecraft_end_portal_frame:
		case Item::minecraft_dragon_egg:
		case Item::minecraft_ender_chest:
		case Item::minecraft_command_block:
		case Item::minecraft_beacon:
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
		case Item::minecraft_anvil:
		case Item::minecraft_chipped_anvil:
		case Item::minecraft_damaged_anvil:
		case Item::minecraft_light:
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
		case Item::minecraft_piston:
		case Item::minecraft_sticky_piston:
		case Item::minecraft_observer:
		case Item::minecraft_hopper:
		case Item::minecraft_dispenser:
		case Item::minecraft_dropper:
		case Item::minecraft_lectern:
		case Item::minecraft_lever:
		case Item::minecraft_lightning_rod:
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
		case Item::minecraft_iron_door:
		case Item::minecraft_oak_door:
		case Item::minecraft_spruce_door:
		case Item::minecraft_birch_door:
		case Item::minecraft_jungle_door:
		case Item::minecraft_acacia_door:
		case Item::minecraft_dark_oak_door:
		case Item::minecraft_crimson_door:
		case Item::minecraft_warped_door:
		case Item::minecraft_iron_trapdoor:
		case Item::minecraft_oak_trapdoor:
		case Item::minecraft_spruce_trapdoor:
		case Item::minecraft_birch_trapdoor:
		case Item::minecraft_jungle_trapdoor:
		case Item::minecraft_acacia_trapdoor:
		case Item::minecraft_dark_oak_trapdoor:
		case Item::minecraft_crimson_trapdoor:
		case Item::minecraft_warped_trapdoor:
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
		case Item::minecraft_oak_sapling:
		case Item::minecraft_spruce_sapling:
		case Item::minecraft_birch_sapling:
		case Item::minecraft_jungle_sapling:
		case Item::minecraft_acacia_sapling:
		case Item::minecraft_dark_oak_sapling:
		case Item::minecraft_brown_mushroom_block:
		case Item::minecraft_red_mushroom_block:
		case Item::minecraft_mushroom_stem:
		case Item::minecraft_spawner:
		case Item::minecraft_note_block:
			break;
			}

		case Item::minecraft_redstone_lamp:

		case Item::minecraft_saddle:
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
		case Item::minecraft_bucket:
		case Item::minecraft_water_bucket:
		case Item::minecraft_lava_bucket:
		case Item::minecraft_powder_snow_bucket:
		case Item::minecraft_snowball:
		case Item::minecraft_leather:
		case Item::minecraft_milk_bucket:
		case Item::minecraft_pufferfish_bucket:
		case Item::minecraft_salmon_bucket:
		case Item::minecraft_cod_bucket:
		case Item::minecraft_tropical_fish_bucket:
		case Item::minecraft_axolotl_bucket:
		case Item::minecraft_brick:
		case Item::minecraft_clay_ball:
		case Item::minecraft_dried_kelp_block:
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
		case Item::minecraft_cauldron:
		case Item::minecraft_ender_eye:
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
		case Item::minecraft_flower_pot:
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
		case Item::minecraft_loom:
		case Item::minecraft_flower_banner_pattern:
		case Item::minecraft_creeper_banner_pattern:
		case Item::minecraft_skull_banner_pattern:
		case Item::minecraft_mojang_banner_pattern:
		case Item::minecraft_globe_banner_pattern:
		case Item::minecraft_piglin_banner_pattern:
		case Item::minecraft_composter:
		case Item::minecraft_barrel:
		case Item::minecraft_smoker:
		case Item::minecraft_blast_furnace:
		case Item::minecraft_cartography_table:
		case Item::minecraft_fletching_table:
		case Item::minecraft_grindstone:
		case Item::minecraft_smithing_table:
		case Item::minecraft_stonecutter:
		case Item::minecraft_bell:
		case Item::minecraft_lantern:
		case Item::minecraft_soul_lantern:
		case Item::minecraft_sweet_berries:
		case Item::minecraft_glow_berries:
		case Item::minecraft_campfire:
		case Item::minecraft_soul_campfire:
		case Item::minecraft_shroomlight:
		case Item::minecraft_honeycomb:
		case Item::minecraft_bee_nest:
		case Item::minecraft_beehive:
		case Item::minecraft_honey_bottle:
		case Item::minecraft_honeycomb_block:
		case Item::minecraft_lodestone:
		case Item::minecraft_crying_obsidian:
		case Item::minecraft_blackstone:
		case Item::minecraft_blackstone_slab:
		case Item::minecraft_blackstone_stairs:
		case Item::minecraft_gilded_blackstone:
		case Item::minecraft_polished_blackstone:
		case Item::minecraft_polished_blackstone_slab:
		case Item::minecraft_polished_blackstone_stairs:
		case Item::minecraft_chiseled_polished_blackstone:
		case Item::minecraft_polished_blackstone_bricks:
		case Item::minecraft_polished_blackstone_brick_slab:
		case Item::minecraft_polished_blackstone_brick_stairs:
		case Item::minecraft_cracked_polished_blackstone_bricks:
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
		case Item::minecraft_small_amethyst_bud:
		case Item::minecraft_medium_amethyst_bud:
		case Item::minecraft_large_amethyst_bud:
		case Item::minecraft_amethyst_cluster:
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
	}
	else message::play::send::chatMessage(p, Chat("Debug: setBlockByItem: no block placed", Chat::color::red), ChatMessage::systemMessage, mcUUID(0, 0, 0, 0));
}