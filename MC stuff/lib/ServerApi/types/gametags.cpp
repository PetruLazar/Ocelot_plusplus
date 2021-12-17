#include "gametags.h"

#include "block.h"
#include "item.h"
#include "fluid.h"
#include "entity.h"
#include "gameevent.h"
#include "registry.h"

#include <filesystem>
using namespace std::filesystem;

std::vector<TagGroup> TagGroup::defaultTags;

const std::string TagGroup::blockTags = "minecraft:block",
TagGroup::itemTags = "minecraft:item",
TagGroup::fluidTags = "minecraft:fluid",
TagGroup::entityTypeTags = "minecraft:entity_type",
TagGroup::gameEvents = "minecraft:game_event";

const TagGroup::Tag* TagGroup::getTag(const std::string& name)
{
	for (const Tag& tag : tags) if (tag.name == name) return &tag;
	return nullptr;
}
void TagGroup::loadTag(const std::string& tagName)
{
	//if the tag is already loaded, exit
	if (getTag(tagType, tagName)) return;

	//open file and parse it
	std::fstream tagFile("data/tags/" + std::string((tagType.c_str() + sizeof("minecraft"))) + "s/" + tagName + ".json", std::ios::in);
	if (!tagFile.is_open())
		throw;
	json& jsonTag = *json::parse(tagFile);

	//create the tag
	Tag tag;
	tag.name = "minecraft:" + tagName;

	//registry that the values will be from
	//std::string registry = tagType;
	//registry.pop_back();

	//take each entry of "values"
	json& values = jsonTag["values"];
	ull size = values.getSize();
	tags.reserve(size);
	for (ull i = 0; i < size; i++)
	{
		std::string value = values[(int)i].value();
		if (value[0] == '#')
		{
			//value si reference to another tag
			loadTag(value.c_str() + sizeof("minecraft:"));
			const Tag* refTag = getTag(tagType, (value.c_str() + 1));
			if (!&refTag)
				throw;
			for (const varInt& v : refTag->entries) tag.entries.emplace_back(v);
		}
		else
		{
			//value is literal, add it to the tag
			tag.entries.emplace_back(Registry::getId(tagType, value));
		}
	}

	//add the tag to the TagGroup
	tags.emplace_back(tag);

	//memory cleanup
	delete& jsonTag;
}

const TagGroup::Tag* TagGroup::getTag(const std::string& category, const std::string& tagName)
{
	for (const TagGroup& tagGroup : TagGroup::defaultTags) if (tagGroup.tagType == category)
	{
		for (const Tag& tag : tagGroup.tags) if (tag.name == tagName) return &tag;
		return nullptr;
	}
	return nullptr;
}
void TagGroup::loadVanillaTags()
{
	directory_iterator tagsDir("data/tags");

	//take all folders in data/tags directory
	for (const directory_entry& tagGroup : tagsDir)
	{
		//ignore files that are not directories
		if (!tagGroup.is_directory()) continue;

		//create the tag group
		TagGroup& group = defaultTags.emplace_back();
		group.tagType = "minecraft:" + tagGroup.path().stem().string();
		group.tagType.pop_back();

		recursive_directory_iterator tagGroupDir(tagGroup);

		//take each tag file in the tag group
		for (const directory_entry& tagFile : tagGroupDir)
		{
			//ignore files that are not regular files
			if (!tagFile.is_regular_file()) continue;

			std::string tagName = relative(tagFile.path(), tagGroup.path()).replace_extension().string();
			std::replace(tagName.begin(), tagName.end(), '\\', '/');
			group.loadTag(tagName);
		}
	}
}

bool TagGroup::checkTagEntry(const mcString& tagType, const mcString& tagName, varInt entry)
{
	for (const TagGroup& tagGroup : defaultTags)
		if (tagGroup.tagType == tagType)
			for (const Tag& tag : tagGroup.tags)
				if (tag.name == tagName)
					for (varInt entry_ : tag.entries)
						if (entry_ == entry)
							return true;
	return false;
}

/*previous tag implementation
const varInt Tags::defaltTagsLengthCount = 5;
Tags Tags::defaultTags[5] = {
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
		Tag("minecraft:coals",2,new varInt[2]{
			(int)Item::minecraft_coal,
			(int)Item::minecraft_charcoal
		}),
		Tag("minecraft:banners",16,new varInt[16]{
			(int)Item::minecraft_white_banner,
			(int)Item::minecraft_orange_banner,
			(int)Item::minecraft_magenta_banner,
			(int)Item::minecraft_light_blue_banner,
			(int)Item::minecraft_yellow_banner,
			(int)Item::minecraft_lime_banner,
			(int)Item::minecraft_pink_banner,
			(int)Item::minecraft_gray_banner,
			(int)Item::minecraft_light_gray_banner,
			(int)Item::minecraft_cyan_banner,
			(int)Item::minecraft_purple_banner,
			(int)Item::minecraft_blue_banner,
			(int)Item::minecraft_brown_banner,
			(int)Item::minecraft_green_banner,
			(int)Item::minecraft_red_banner,
			(int)Item::minecraft_black_banner
		}),
		Tag("minecraft:soul_fire_base_blocks",2,new varInt[2]{
			(int)Item::minecraft_soul_sand,
			(int)Item::minecraft_soul_soil
		}),
		Tag("minecraft:stone_crafting_materials",3,new varInt[3]{
			(int)Item::minecraft_cobblestone,
			(int)Item::minecraft_blackstone,
			(int)Item::minecraft_cobbled_deepslate
		}),
		Tag("minecraft:wooden_fences",8,new varInt[8]{
			(int)Item::minecraft_oak_fence,
			(int)Item::minecraft_acacia_fence,
			(int)Item::minecraft_dark_oak_fence,
			(int)Item::minecraft_spruce_fence,
			(int)Item::minecraft_birch_fence,
			(int)Item::minecraft_jungle_fence,
			(int)Item::minecraft_crimson_fence,
			(int)Item::minecraft_warped_fence
		}),
		Tag("minecraft:piglin_repellents",3,new varInt[3]{
			(int)Item::minecraft_soul_torch,
			(int)Item::minecraft_soul_lantern,
			(int)Item::minecraft_soul_campfire
		}),
		Tag("minecraft:beacon_payment_items",5,new varInt[5]{
			(int)Item::minecraft_netherite_ingot,
			(int)Item::minecraft_emerald,
			(int)Item::minecraft_diamond,
			(int)Item::minecraft_gold_ingot,
			(int)Item::minecraft_iron_ingot
		}),
		Tag("minecraft:wooden_slabs",8,new varInt[8]{
			(int)Item::minecraft_oak_slab,
			(int)Item::minecraft_spruce_slab,
			(int)Item::minecraft_birch_slab,
			(int)Item::minecraft_jungle_slab,
			(int)Item::minecraft_acacia_slab,
			(int)Item::minecraft_dark_oak_slab,
			(int)Item::minecraft_crimson_slab,
			(int)Item::minecraft_warped_slab
		}),
		Tag("minecraft:coal_ores",2,new varInt[2]{
			(int)Item::minecraft_coal_ore,
			(int)Item::minecraft_deepslate_coal_ore
		}),
		Tag("minecraft:occludes_vibration_signals",16,new varInt[16]{
			(int)Item::minecraft_white_wool,
			(int)Item::minecraft_orange_wool,
			(int)Item::minecraft_magenta_wool,
			(int)Item::minecraft_light_blue_wool,
			(int)Item::minecraft_yellow_wool,
			(int)Item::minecraft_lime_wool,
			(int)Item::minecraft_pink_wool,
			(int)Item::minecraft_gray_wool,
			(int)Item::minecraft_light_gray_wool,
			(int)Item::minecraft_cyan_wool,
			(int)Item::minecraft_purple_wool,
			(int)Item::minecraft_blue_wool,
			(int)Item::minecraft_brown_wool,
			(int)Item::minecraft_green_wool,
			(int)Item::minecraft_red_wool,
			(int)Item::minecraft_black_wool
		}),
		Tag("minecraft:small_flowers",13,new varInt[13]{
			(int)Item::minecraft_dandelion,
			(int)Item::minecraft_poppy,
			(int)Item::minecraft_blue_orchid,
			(int)Item::minecraft_allium,
			(int)Item::minecraft_azure_bluet,
			(int)Item::minecraft_red_tulip,
			(int)Item::minecraft_orange_tulip,
			(int)Item::minecraft_white_tulip,
			(int)Item::minecraft_pink_tulip,
			(int)Item::minecraft_oxeye_daisy,
			(int)Item::minecraft_cornflower,
			(int)Item::minecraft_lily_of_the_valley,
			(int)Item::minecraft_wither_rose
		}),
		Tag("minecraft:wooden_trapdoors",8,new varInt[8]{
			(int)Item::minecraft_acacia_trapdoor,
			(int)Item::minecraft_birch_trapdoor,
			(int)Item::minecraft_dark_oak_trapdoor,
			(int)Item::minecraft_jungle_trapdoor,
			(int)Item::minecraft_oak_trapdoor,
			(int)Item::minecraft_spruce_trapdoor,
			(int)Item::minecraft_crimson_trapdoor,
			(int)Item::minecraft_warped_trapdoor
		}),
		Tag("minecraft:jungle_logs",4,new varInt[4]{
			(int)Item::minecraft_jungle_log,
			(int)Item::minecraft_jungle_wood,
			(int)Item::minecraft_stripped_jungle_log,
			(int)Item::minecraft_stripped_jungle_wood
		}),
		Tag("minecraft:lectern_books",2,new varInt[2]{
			(int)Item::minecraft_written_book,
			(int)Item::minecraft_writable_book
		}),
		Tag("minecraft:wooden_stairs",8,new varInt[8]{
			(int)Item::minecraft_oak_stairs,
			(int)Item::minecraft_spruce_stairs,
			(int)Item::minecraft_birch_stairs,
			(int)Item::minecraft_jungle_stairs,
			(int)Item::minecraft_acacia_stairs,
			(int)Item::minecraft_dark_oak_stairs,
			(int)Item::minecraft_crimson_stairs,
			(int)Item::minecraft_warped_stairs
		}),
		Tag("minecraft:spruce_logs",4,new varInt[4]{
			(int)Item::minecraft_spruce_log,
			(int)Item::minecraft_spruce_wood,
			(int)Item::minecraft_stripped_spruce_log,
			(int)Item::minecraft_stripped_spruce_wood
		}),
		Tag("minecraft:signs",8,new varInt[8]{
			(int)Item::minecraft_oak_sign,
			(int)Item::minecraft_spruce_sign,
			(int)Item::minecraft_birch_sign,
			(int)Item::minecraft_acacia_sign,
			(int)Item::minecraft_jungle_sign,
			(int)Item::minecraft_dark_oak_sign,
			(int)Item::minecraft_crimson_sign,
			(int)Item::minecraft_warped_sign
		}),
		Tag("minecraft:axolotl_tempt_items",1,new varInt[1]{
			(int)Item::minecraft_tropical_fish_bucket
		}),
		Tag("minecraft:carpets",16,new varInt[16]{
			(int)Item::minecraft_white_carpet,
			(int)Item::minecraft_orange_carpet,
			(int)Item::minecraft_magenta_carpet,
			(int)Item::minecraft_light_blue_carpet,
			(int)Item::minecraft_yellow_carpet,
			(int)Item::minecraft_lime_carpet,
			(int)Item::minecraft_pink_carpet,
			(int)Item::minecraft_gray_carpet,
			(int)Item::minecraft_light_gray_carpet,
			(int)Item::minecraft_cyan_carpet,
			(int)Item::minecraft_purple_carpet,
			(int)Item::minecraft_blue_carpet,
			(int)Item::minecraft_brown_carpet,
			(int)Item::minecraft_green_carpet,
			(int)Item::minecraft_red_carpet,
			(int)Item::minecraft_black_carpet
		}),
		Tag("minecraft:wool",16,new varInt[16]{
			(int)Item::minecraft_white_wool,
			(int)Item::minecraft_orange_wool,
			(int)Item::minecraft_magenta_wool,
			(int)Item::minecraft_light_blue_wool,
			(int)Item::minecraft_yellow_wool,
			(int)Item::minecraft_lime_wool,
			(int)Item::minecraft_pink_wool,
			(int)Item::minecraft_gray_wool,
			(int)Item::minecraft_light_gray_wool,
			(int)Item::minecraft_cyan_wool,
			(int)Item::minecraft_purple_wool,
			(int)Item::minecraft_blue_wool,
			(int)Item::minecraft_brown_wool,
			(int)Item::minecraft_green_wool,
			(int)Item::minecraft_red_wool,
			(int)Item::minecraft_black_wool
		}),
		Tag("minecraft:wooden_buttons",8,new varInt[8]{
			(int)Item::minecraft_oak_button,
			(int)Item::minecraft_spruce_button,
			(int)Item::minecraft_birch_button,
			(int)Item::minecraft_jungle_button,
			(int)Item::minecraft_acacia_button,
			(int)Item::minecraft_dark_oak_button,
			(int)Item::minecraft_crimson_button,
			(int)Item::minecraft_warped_button
		}),
		Tag("minecraft:stairs",48,new varInt[48]{
			(int)Item::minecraft_cobblestone_stairs,
			(int)Item::minecraft_sandstone_stairs,
			(int)Item::minecraft_nether_brick_stairs,
			(int)Item::minecraft_stone_brick_stairs,
			(int)Item::minecraft_brick_stairs,
			(int)Item::minecraft_purpur_stairs,
			(int)Item::minecraft_quartz_stairs,
			(int)Item::minecraft_red_sandstone_stairs,
			(int)Item::minecraft_prismarine_brick_stairs,
			(int)Item::minecraft_prismarine_stairs,
			(int)Item::minecraft_dark_prismarine_stairs,
			(int)Item::minecraft_polished_granite_stairs,
			(int)Item::minecraft_smooth_red_sandstone_stairs,
			(int)Item::minecraft_mossy_stone_brick_stairs,
			(int)Item::minecraft_polished_diorite_stairs,
			(int)Item::minecraft_mossy_cobblestone_stairs,
			(int)Item::minecraft_end_stone_brick_stairs,
			(int)Item::minecraft_stone_stairs,
			(int)Item::minecraft_smooth_sandstone_stairs,
			(int)Item::minecraft_smooth_quartz_stairs,
			(int)Item::minecraft_granite_stairs,
			(int)Item::minecraft_andesite_stairs,
			(int)Item::minecraft_red_nether_brick_stairs,
			(int)Item::minecraft_polished_andesite_stairs,
			(int)Item::minecraft_diorite_stairs,
			(int)Item::minecraft_blackstone_stairs,
			(int)Item::minecraft_polished_blackstone_brick_stairs,
			(int)Item::minecraft_polished_blackstone_stairs,
			(int)Item::minecraft_cobbled_deepslate_stairs,
			(int)Item::minecraft_polished_deepslate_stairs,
			(int)Item::minecraft_deepslate_tile_stairs,
			(int)Item::minecraft_deepslate_brick_stairs,
			(int)Item::minecraft_oxidized_cut_copper_stairs,
			(int)Item::minecraft_weathered_cut_copper_stairs,
			(int)Item::minecraft_exposed_cut_copper_stairs,
			(int)Item::minecraft_cut_copper_stairs,
			(int)Item::minecraft_waxed_weathered_cut_copper_stairs,
			(int)Item::minecraft_waxed_exposed_cut_copper_stairs,
			(int)Item::minecraft_waxed_cut_copper_stairs,
			(int)Item::minecraft_waxed_oxidized_cut_copper_stairs,
			(int)Item::minecraft_oak_stairs,
			(int)Item::minecraft_spruce_stairs,
			(int)Item::minecraft_birch_stairs,
			(int)Item::minecraft_jungle_stairs,
			(int)Item::minecraft_acacia_stairs,
			(int)Item::minecraft_dark_oak_stairs,
			(int)Item::minecraft_crimson_stairs,
			(int)Item::minecraft_warped_stairs
		}),
		Tag("minecraft:fishes",6,new varInt[6]{
			(int)Item::minecraft_cod,
			(int)Item::minecraft_cooked_cod,
			(int)Item::minecraft_salmon,
			(int)Item::minecraft_cooked_salmon,
			(int)Item::minecraft_pufferfish,
			(int)Item::minecraft_tropical_fish
		}),
		Tag("minecraft:logs",32,new varInt[32]{
			(int)Item::minecraft_crimson_stem,
			(int)Item::minecraft_stripped_crimson_stem,
			(int)Item::minecraft_crimson_hyphae,
			(int)Item::minecraft_stripped_crimson_hyphae,
			(int)Item::minecraft_warped_stem,
			(int)Item::minecraft_stripped_warped_stem,
			(int)Item::minecraft_warped_hyphae,
			(int)Item::minecraft_stripped_warped_hyphae,
			(int)Item::minecraft_oak_log,
			(int)Item::minecraft_oak_wood,
			(int)Item::minecraft_stripped_oak_log,
			(int)Item::minecraft_stripped_oak_wood,
			(int)Item::minecraft_dark_oak_log,
			(int)Item::minecraft_dark_oak_wood,
			(int)Item::minecraft_stripped_dark_oak_log,
			(int)Item::minecraft_stripped_dark_oak_wood,
			(int)Item::minecraft_birch_log,
			(int)Item::minecraft_birch_wood,
			(int)Item::minecraft_stripped_birch_log,
			(int)Item::minecraft_stripped_birch_wood,
			(int)Item::minecraft_jungle_log,
			(int)Item::minecraft_jungle_wood,
			(int)Item::minecraft_stripped_jungle_log,
			(int)Item::minecraft_stripped_jungle_wood,
			(int)Item::minecraft_spruce_log,
			(int)Item::minecraft_spruce_wood,
			(int)Item::minecraft_stripped_spruce_log,
			(int)Item::minecraft_stripped_spruce_wood,
			(int)Item::minecraft_acacia_log,
			(int)Item::minecraft_acacia_wood,
			(int)Item::minecraft_stripped_acacia_log,
			(int)Item::minecraft_stripped_acacia_wood
		}),
		Tag("minecraft:stone_bricks",4,new varInt[4]{
			(int)Item::minecraft_stone_bricks,
			(int)Item::minecraft_mossy_stone_bricks,
			(int)Item::minecraft_cracked_stone_bricks,
			(int)Item::minecraft_chiseled_stone_bricks
		}),
		Tag("minecraft:creeper_drop_music_discs",12,new varInt[12]{
			(int)Item::minecraft_music_disc_13,
			(int)Item::minecraft_music_disc_cat,
			(int)Item::minecraft_music_disc_blocks,
			(int)Item::minecraft_music_disc_chirp,
			(int)Item::minecraft_music_disc_far,
			(int)Item::minecraft_music_disc_mall,
			(int)Item::minecraft_music_disc_mellohi,
			(int)Item::minecraft_music_disc_stal,
			(int)Item::minecraft_music_disc_strad,
			(int)Item::minecraft_music_disc_ward,
			(int)Item::minecraft_music_disc_11,
			(int)Item::minecraft_music_disc_wait
		}),
		Tag("minecraft:arrows",3,new varInt[3]{
			(int)Item::minecraft_arrow,
			(int)Item::minecraft_tipped_arrow,
			(int)Item::minecraft_spectral_arrow
		}),
		Tag("minecraft:slabs", 0, new varInt[]{
			(int)Item::minecraft_stone_slab,
			(int)Item::minecraft_smooth_stone_slab,
			(int)Item::minecraft_stone_brick_slab,
			(int)Item::minecraft_sandstone_slab,
			(int)Item::minecraft_purpur_slab,
			(int)Item::minecraft_quartz_slab,
			(int)Item::minecraft_red_sandstone_slab,
			(int)Item::minecraft_brick_slab,
			(int)Item::minecraft_cobblestone_slab,
			(int)Item::minecraft_nether_brick_slab,
			(int)Item::minecraft_petrified_oak_slab,
			(int)Item::minecraft_prismarine_slab,
			(int)Item::minecraft_prismarine_brick_slab,
			(int)Item::minecraft_dark_prismarine_slab,
			(int)Item::minecraft_polished_granite_slab,
			(int)Item::minecraft_smooth_red_sandstone_slab,
			(int)Item::minecraft_mossy_stone_brick_slab,
			(int)Item::minecraft_polished_diorite_slab,
			(int)Item::minecraft_mossy_cobblestone_slab,
			(int)Item::minecraft_end_stone_brick_slab,
			(int)Item::minecraft_smooth_sandstone_slab,
			(int)Item::minecraft_smooth_quartz_slab,
			(int)Item::minecraft_granite_slab,
			(int)Item::minecraft_andesite_slab,
			(int)Item::minecraft_red_nether_brick_slab,
			(int)Item::minecraft_polished_andesite_slab,
			(int)Item::minecraft_diorite_slab,
			(int)Item::minecraft_cut_sandstone_slab,
			(int)Item::minecraft_cut_red_sandstone_slab,
			(int)Item::minecraft_blackstone_slab,
			(int)Item::minecraft_polished_blackstone_brick_slab,
			(int)Item::minecraft_polished_blackstone_slab,
			(int)Item::minecraft_cobbled_deepslate_slab,
			(int)Item::minecraft_polished_deepslate_slab,
			(int)Item::minecraft_deepslate_tile_slab,
			(int)Item::minecraft_deepslate_brick_slab,
			(int)Item::minecraft_waxed_weathered_cut_copper_slab,
			(int)Item::minecraft_waxed_exposed_cut_copper_slab,
			(int)Item::minecraft_waxed_cut_copper_slab,
			(int)Item::minecraft_oxidized_cut_copper_slab,
			(int)Item::minecraft_weathered_cut_copper_slab,
			(int)Item::minecraft_exposed_cut_copper_slab,
			(int)Item::minecraft_cut_copper_slab,
			(int)Item::minecraft_waxed_oxidized_cut_copper_slab,
			(int)Item::minecraft_oak_slab,
			(int)Item::minecraft_spruce_slab,
			(int)Item::minecraft_birch_slab,
			(int)Item::minecraft_jungle_slab,
			(int)Item::minecraft_acacia_slab,
			(int)Item::minecraft_dark_oak_slab,
			(int)Item::minecraft_crimson_slab,
			(int)Item::minecraft_warped_slab
		}),
		Tag("minecraft:wooden_doors",0, new varInt[]{
			(int)Item::minecraft_oak_door,
			(int)Item::minecraft_spruce_door,
			(int)Item::minecraft_birch_door,
			(int)Item::minecraft_jungle_door,
			(int)Item::minecraft_acacia_door,
			(int)Item::minecraft_dark_oak_door,
			(int)Item::minecraft_crimson_door,
			(int)Item::minecraft_warped_door
		}),
		Tag("minecraft:warped_stems",4,new varInt[4]{
			(int)Item::minecraft_warped_stem,
			(int)Item::minecraft_stripped_warped_stem,
			(int)Item::minecraft_warped_hyphae,
			(int)Item::minecraft_stripped_warped_hyphae
		}),
		Tag("minecraft:emerald_ores",0, new varInt[]{
			(int)Item::minecraft_emerald_ore,
			(int)Item::minecraft_deepslate_emerald_ore
		}),
		Tag("minecraft:trapdoors",0, new varInt[]{
			(int)Item::minecraft_iron_trapdoor,
			(int)Item::minecraft_acacia_trapdoor,
			(int)Item::minecraft_birch_trapdoor,
			(int)Item::minecraft_dark_oak_trapdoor,
			(int)Item::minecraft_jungle_trapdoor,
			(int)Item::minecraft_oak_trapdoor,
			(int)Item::minecraft_spruce_trapdoor,
			(int)Item::minecraft_crimson_trapdoor,
			(int)Item::minecraft_warped_trapdoor
		}),
		Tag("minecraft:redstone_ores",0, new varInt[]{
			(int)Item::minecraft_redstone_ore,
			(int)Item::minecraft_deepslate_redstone_ore
		}),
		Tag("minecraft:crimson_stems",4,new varInt[4]{
			(int)Item::minecraft_crimson_stem,
			(int)Item::minecraft_stripped_crimson_stem,
			(int)Item::minecraft_crimson_hyphae,
			(int)Item::minecraft_stripped_crimson_hyphae
		}),
		Tag("minecraft:ignored_by_piglin_babies",0,new varInt[]{
			(int)Item::minecraft_leather
		}),
		Tag("minecraft:buttons",0,new varInt[]{
			(int)Item::minecraft_stone_button,
			(int)Item::minecraft_polished_blackstone_button,
			(int)Item::minecraft_oak_button,
			(int)Item::minecraft_spruce_button,
			(int)Item::minecraft_birch_button,
			(int)Item::minecraft_jungle_button,
			(int)Item::minecraft_acacia_button,
			(int)Item::minecraft_dark_oak_button,
			(int)Item::minecraft_crimson_button,
			(int)Item::minecraft_warped_button
		}),
		Tag("minecraft:flowers",0,new varInt[]{
			(int)Item::minecraft_flowering_azalea_leaves,
			(int)Item::minecraft_flowering_azalea,
			(int)Item::minecraft_sunflower,
			(int)Item::minecraft_lilac,
			(int)Item::minecraft_peony,
			(int)Item::minecraft_rose_bush,
			(int)Item::minecraft_dandelion,
			(int)Item::minecraft_poppy,
			(int)Item::minecraft_blue_orchid,
			(int)Item::minecraft_allium,
			(int)Item::minecraft_azure_bluet,
			(int)Item::minecraft_red_tulip,
			(int)Item::minecraft_orange_tulip,
			(int)Item::minecraft_white_tulip,
			(int)Item::minecraft_pink_tulip,
			(int)Item::minecraft_oxeye_daisy,
			(int)Item::minecraft_cornflower,
			(int)Item::minecraft_lily_of_the_valley,
			(int)Item::minecraft_wither_rose
		}),
		Tag("minecraft:stone_tool_materials",0,new varInt[]{
			(int)Item::minecraft_cobblestone,
			(int)Item::minecraft_blackstone,
			(int)Item::minecraft_cobbled_deepslate
		}),
		Tag("minecraft:planks",0,new varInt[]{
			(int)Item::minecraft_oak_planks,
			(int)Item::minecraft_spruce_planks,
			(int)Item::minecraft_birch_planks,
			(int)Item::minecraft_jungle_planks,
			(int)Item::minecraft_acacia_planks,
			(int)Item::minecraft_dark_oak_planks,
			(int)Item::minecraft_crimson_planks,
			(int)Item::minecraft_warped_planks
		}),
		Tag("minecraft:fox_food",0,new varInt[]{
			(int)Item::minecraft_sweet_berries,
			(int)Item::minecraft_glow_berries
		}),
		Tag("minecraft:boats",0,new varInt[]{
			(int)Item::minecraft_oak_boat,
			(int)Item::minecraft_spruce_boat,
			(int)Item::minecraft_birch_boat,
			(int)Item::minecraft_jungle_boat,
			(int)Item::minecraft_acacia_boat,
			(int)Item::minecraft_dark_oak_boat
		}),
		Tag("minecraft:dark_oak_logs",4,new varInt[4]{
			(int)Item::minecraft_dark_oak_log,
			(int)Item::minecraft_dark_oak_wood,
			(int)Item::minecraft_stripped_dark_oak_log,
			(int)Item::minecraft_stripped_dark_oak_wood,
		}),
		Tag("minecraft:rails",4,new varInt[4]{
			(int)Item::minecraft_rail,
			(int)Item::minecraft_powered_rail,
			(int)Item::minecraft_detector_rail,
			(int)Item::minecraft_activator_rail
		}),
		Tag("minecraft:diamond_ores",0,new varInt[]{
			(int)Item::minecraft_diamond_ore,
			(int)Item::minecraft_deepslate_diamond_ore
		}),
		Tag("minecraft:non_flammable_wood",0,new varInt[]{
			(int)Item::minecraft_warped_stem,
			(int)Item::minecraft_stripped_warped_stem,
			(int)Item::minecraft_warped_hyphae,
			(int)Item::minecraft_stripped_warped_hyphae,
			(int)Item::minecraft_crimson_stem,
			(int)Item::minecraft_stripped_crimson_stem,
			(int)Item::minecraft_crimson_hyphae,
			(int)Item::minecraft_stripped_crimson_hyphae,
			(int)Item::minecraft_crimson_planks,
			(int)Item::minecraft_warped_planks,
			(int)Item::minecraft_crimson_slab,
			(int)Item::minecraft_warped_slab,
			(int)Item::minecraft_crimson_pressure_plate,
			(int)Item::minecraft_warped_pressure_plate,
			(int)Item::minecraft_crimson_fence,
			(int)Item::minecraft_warped_fence,
			(int)Item::minecraft_crimson_trapdoor,
			(int)Item::minecraft_warped_trapdoor,
			(int)Item::minecraft_crimson_fence_gate,
			(int)Item::minecraft_warped_fence_gate,
			(int)Item::minecraft_crimson_stairs,
			(int)Item::minecraft_warped_stairs,
			(int)Item::minecraft_crimson_button,
			(int)Item::minecraft_warped_button,
			(int)Item::minecraft_crimson_door,
			(int)Item::minecraft_warped_door,
			(int)Item::minecraft_crimson_sign,
			(int)Item::minecraft_warped_sign
		}),
		Tag("minecraft:leaves",0,new varInt[]{
			(int)Item::minecraft_jungle_leaves,
			(int)Item::minecraft_oak_leaves,
			(int)Item::minecraft_spruce_leaves,
			(int)Item::minecraft_dark_oak_leaves,
			(int)Item::minecraft_acacia_leaves,
			(int)Item::minecraft_birch_leaves,
			(int)Item::minecraft_azalea_leaves,
			(int)Item::minecraft_flowering_azalea_leaves
		}),
		Tag("minecraft:walls",0,new varInt[]{
			(int)Item::minecraft_cobblestone_wall,
			(int)Item::minecraft_mossy_cobblestone_wall,
			(int)Item::minecraft_brick_wall,
			(int)Item::minecraft_prismarine_wall,
			(int)Item::minecraft_red_sandstone_wall,
			(int)Item::minecraft_mossy_stone_brick_wall,
			(int)Item::minecraft_granite_wall,
			(int)Item::minecraft_stone_brick_wall,
			(int)Item::minecraft_nether_brick_wall,
			(int)Item::minecraft_andesite_wall,
			(int)Item::minecraft_red_nether_brick_wall,
			(int)Item::minecraft_sandstone_wall,
			(int)Item::minecraft_end_stone_brick_wall,
			(int)Item::minecraft_diorite_wall,
			(int)Item::minecraft_blackstone_wall,
			(int)Item::minecraft_polished_blackstone_brick_wall,
			(int)Item::minecraft_polished_blackstone_wall,
			(int)Item::minecraft_cobbled_deepslate_wall,
			(int)Item::minecraft_polished_deepslate_wall,
			(int)Item::minecraft_deepslate_tile_wall,
			(int)Item::minecraft_deepslate_brick_wall
		}),
		Tag("minecraft:piglin_food",0,new varInt[]{
			(int)Item::minecraft_porkchop,
			(int)Item::minecraft_cooked_porkchop
		}),
		Tag("minecraft:wooden_pressure_plates",0,new varInt[]{
			(int)Item::minecraft_oak_pressure_plate,
			(int)Item::minecraft_spruce_pressure_plate,
			(int)Item::minecraft_birch_pressure_plate,
			(int)Item::minecraft_jungle_pressure_plate,
			(int)Item::minecraft_acacia_pressure_plate,
			(int)Item::minecraft_dark_oak_pressure_plate,
			(int)Item::minecraft_crimson_pressure_plate,
			(int)Item::minecraft_warped_pressure_plate
		}),
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
		Tag("minecraft:piglin_loved",0,new varInt[]{
			(int)Item::minecraft_gold_block,
			(int)Item::minecraft_gilded_blackstone,
			(int)Item::minecraft_light_weighted_pressure_plate,
			(int)Item::minecraft_gold_ingot,
			(int)Item::minecraft_bell,
			(int)Item::minecraft_clock,
			(int)Item::minecraft_golden_carrot,
			(int)Item::minecraft_glistering_melon_slice,
			(int)Item::minecraft_golden_apple,
			(int)Item::minecraft_enchanted_golden_apple,
			(int)Item::minecraft_golden_helmet,
			(int)Item::minecraft_golden_chestplate,
			(int)Item::minecraft_golden_leggings,
			(int)Item::minecraft_golden_boots,
			(int)Item::minecraft_golden_horse_armor,
			(int)Item::minecraft_golden_sword,
			(int)Item::minecraft_golden_pickaxe,
			(int)Item::minecraft_golden_shovel,
			(int)Item::minecraft_golden_axe,
			(int)Item::minecraft_golden_hoe,
			(int)Item::minecraft_raw_gold,
			(int)Item::minecraft_raw_gold_block,
			(int)Item::minecraft_gold_ore,
			(int)Item::minecraft_nether_gold_ore,
			(int)Item::minecraft_deepslate_gold_ore
		}),
		Tag("minecraft:candles",0,new varInt[]{
			(int)Item::minecraft_candle,
			(int)Item::minecraft_white_candle,
			(int)Item::minecraft_orange_candle,
			(int)Item::minecraft_magenta_candle,
			(int)Item::minecraft_light_blue_candle,
			(int)Item::minecraft_yellow_candle,
			(int)Item::minecraft_lime_candle,
			(int)Item::minecraft_pink_candle,
			(int)Item::minecraft_gray_candle,
			(int)Item::minecraft_light_gray_candle,
			(int)Item::minecraft_cyan_candle,
			(int)Item::minecraft_purple_candle,
			(int)Item::minecraft_blue_candle,
			(int)Item::minecraft_brown_candle,
			(int)Item::minecraft_green_candle,
			(int)Item::minecraft_red_candle,
			(int)Item::minecraft_black_candle
		}),
		Tag("minecraft:music_discs",0,new varInt[]{
			(int)Item::minecraft_music_disc_pigstep,
			(int)Item::minecraft_music_disc_13,
			(int)Item::minecraft_music_disc_cat,
			(int)Item::minecraft_music_disc_blocks,
			(int)Item::minecraft_music_disc_chirp,
			(int)Item::minecraft_music_disc_far,
			(int)Item::minecraft_music_disc_mall,
			(int)Item::minecraft_music_disc_mellohi,
			(int)Item::minecraft_music_disc_stal,
			(int)Item::minecraft_music_disc_strad,
			(int)Item::minecraft_music_disc_ward,
			(int)Item::minecraft_music_disc_11,
			(int)Item::minecraft_music_disc_wait
		}),
		Tag("minecraft:birch_logs",4, new varInt[4]{
			(int)Item::minecraft_birch_log,
			(int)Item::minecraft_birch_wood,
			(int)Item::minecraft_stripped_birch_log,
			(int)Item::minecraft_stripped_birch_wood,
			}),
		Tag("minecraft:tall_flowers",0,new varInt[]{
			(int)Item::minecraft_sunflower,
			(int)Item::minecraft_lilac,
			(int)Item::minecraft_peony,
			(int)Item::minecraft_rose_bush
		}),
		Tag("minecraft:lapis_ores",0,new varInt[]{
			(int)Item::minecraft_lapis_ore,
			(int)Item::minecraft_deepslate_lapis_ore
		}),
		Tag("minecraft:sand",0,new varInt[]{
			(int)Item::minecraft_sand,
			(int)Item::minecraft_red_sand
		}),
		Tag("minecraft:copper_ores",0,new varInt[]{
			(int)Item::minecraft_copper_ore,
			(int)Item::minecraft_deepslate_copper_ore
		}),
		Tag("minecraft:gold_ores",0,new varInt[]{
			(int)Item::minecraft_gold_ore,
			(int)Item::minecraft_nether_gold_ore,
			(int)Item::minecraft_deepslate_gold_ore
		}),
		Tag("minecraft:freeze_immune_wearables",0,new varInt[]{
			(int)Item::minecraft_leather_boots,
			(int)Item::minecraft_leather_leggings,
			(int)Item::minecraft_leather_chestplate,
			(int)Item::minecraft_leather_helmet,
			(int)Item::minecraft_leather_horse_armor
		}),
		Tag("minecraft:logs_that_burn", 24, new varInt[24]{
			(int)Item::minecraft_oak_log,
			(int)Item::minecraft_oak_wood,
			(int)Item::minecraft_stripped_oak_log,
			(int)Item::minecraft_stripped_oak_wood,
			(int)Item::minecraft_dark_oak_log,
			(int)Item::minecraft_dark_oak_wood,
			(int)Item::minecraft_stripped_dark_oak_log,
			(int)Item::minecraft_stripped_dark_oak_wood,
			(int)Item::minecraft_birch_log,
			(int)Item::minecraft_birch_wood,
			(int)Item::minecraft_stripped_birch_log,
			(int)Item::minecraft_stripped_birch_wood,
			(int)Item::minecraft_jungle_log,
			(int)Item::minecraft_jungle_wood,
			(int)Item::minecraft_stripped_jungle_log,
			(int)Item::minecraft_stripped_jungle_wood,
			(int)Item::minecraft_spruce_log,
			(int)Item::minecraft_spruce_wood,
			(int)Item::minecraft_stripped_spruce_log,
			(int)Item::minecraft_stripped_spruce_wood,
			(int)Item::minecraft_acacia_log,
			(int)Item::minecraft_acacia_wood,
			(int)Item::minecraft_stripped_acacia_log,
			(int)Item::minecraft_stripped_acacia_wood
		}),
		Tag("minecraft:fences",0,new varInt[]{
			(int)Item::minecraft_nether_brick_fence,
			(int)Item::minecraft_oak_fence,
			(int)Item::minecraft_acacia_fence,
			(int)Item::minecraft_dark_oak_fence,
			(int)Item::minecraft_spruce_fence,
			(int)Item::minecraft_birch_fence,
			(int)Item::minecraft_jungle_fence,
			(int)Item::minecraft_crimson_fence,
			(int)Item::minecraft_warped_fence
		}),
		Tag("minecraft:saplings",0,new varInt[]{
			(int)Item::minecraft_oak_sapling,
			(int)Item::minecraft_spruce_sapling,
			(int)Item::minecraft_birch_sapling,
			(int)Item::minecraft_jungle_sapling,
			(int)Item::minecraft_acacia_sapling,
			(int)Item::minecraft_dark_oak_sapling,
			(int)Item::minecraft_azalea,
			(int)Item::minecraft_flowering_azalea
		}),
		Tag("minecraft:beds",0,new varInt[]{
			(int)Item::minecraft_red_bed,
			(int)Item::minecraft_black_bed,
			(int)Item::minecraft_blue_bed,
			(int)Item::minecraft_brown_bed,
			(int)Item::minecraft_cyan_bed,
			(int)Item::minecraft_gray_bed,
			(int)Item::minecraft_green_bed,
			(int)Item::minecraft_light_blue_bed,
			(int)Item::minecraft_light_gray_bed,
			(int)Item::minecraft_lime_bed,
			(int)Item::minecraft_magenta_bed,
			(int)Item::minecraft_orange_bed,
			(int)Item::minecraft_pink_bed,
			(int)Item::minecraft_purple_bed,
			(int)Item::minecraft_white_bed,
			(int)Item::minecraft_yellow_bed
		}),
		Tag("minecraft:iron_ores",0,new varInt[]{
			(int)Item::minecraft_iron_ore,
			(int)Item::minecraft_deepslate_iron_ore
		}),
		Tag("minecraft:oak_logs",4, new varInt[4]{
			(int)Item::minecraft_oak_log,
			(int)Item::minecraft_oak_wood,
			(int)Item::minecraft_stripped_oak_log,
			(int)Item::minecraft_stripped_oak_wood,
			}),
		Tag("minecraft:doors",0,new varInt[]{
			(int)Item::minecraft_iron_door,
			(int)Item::minecraft_oak_door,
			(int)Item::minecraft_spruce_door,
			(int)Item::minecraft_birch_door,
			(int)Item::minecraft_jungle_door,
			(int)Item::minecraft_acacia_door,
			(int)Item::minecraft_dark_oak_door,
			(int)Item::minecraft_crimson_door,
			(int)Item::minecraft_warped_door
		}),
		Tag("minecraft:cluster_max_harvestables",0,new varInt[]{
			(int)Item::minecraft_diamond_pickaxe,
			(int)Item::minecraft_golden_pickaxe,
			(int)Item::minecraft_iron_pickaxe,
			(int)Item::minecraft_netherite_pickaxe,
			(int)Item::minecraft_stone_pickaxe,
			(int)Item::minecraft_wooden_pickaxe
		})
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
		Tag("minecraft:axolotl_always_hostiles",3,new varInt[3]{
			(int)EntityType::minecraft_drowned,
			(int)EntityType::minecraft_guardian,
			(int)EntityType::minecraft_elder_guardian,
		}),
		Tag("minecraft:freeze_hurts_extra_types",3,new varInt[3]{
			(int)EntityType::minecraft_strider,
			(int)EntityType::minecraft_blaze,
			(int)EntityType::minecraft_magma_cube,
		}),
		Tag("minecraft:freeze_immune_entity_types",4,new varInt[4]{
			(int)EntityType::minecraft_stray,
			(int)EntityType::minecraft_polar_bear,
			(int)EntityType::minecraft_snow_golem,
			(int)EntityType::minecraft_wither,
		}),
		Tag("minecraft:beehive_inhabitors",1,new varInt[1]{
			(int)EntityType::minecraft_bee,
		}),
		Tag("minecraft:impact_projectiles",9,new varInt[9]{
			(int)EntityType::minecraft_arrow,
			(int)EntityType::minecraft_spectral_arrow,
			(int)EntityType::minecraft_snowball,
			(int)EntityType::minecraft_fireball,
			(int)EntityType::minecraft_small_fireball,
			(int)EntityType::minecraft_egg,
			(int)EntityType::minecraft_trident,
			(int)EntityType::minecraft_dragon_fireball,
			(int)EntityType::minecraft_wither_skull,
		}),
		Tag("minecraft:skeletons",3,new varInt[3]{
			(int)EntityType::minecraft_skeleton,
			(int)EntityType::minecraft_stray,
			(int)EntityType::minecraft_wither_skeleton,
		}),
		Tag("minecraft:raiders",6,new varInt[6]{
			(int)EntityType::minecraft_evoker,
			(int)EntityType::minecraft_illusioner,
			(int)EntityType::minecraft_pillager,
			(int)EntityType::minecraft_ravager,
			(int)EntityType::minecraft_vindicator,
			(int)EntityType::minecraft_witch,
		}),
		Tag("minecraft:powder_snow_walkable_mobs",4,new varInt[4]{
			(int)EntityType::minecraft_rabbit,
			(int)EntityType::minecraft_endermite,
			(int)EntityType::minecraft_silverfish,
			(int)EntityType::minecraft_fox,
		}),
		Tag("minecraft:axolotl_hunt_targets",6,new varInt[6]{
			(int)EntityType::minecraft_tropical_fish,
			(int)EntityType::minecraft_pufferfish,
			(int)EntityType::minecraft_salmon,
			(int)EntityType::minecraft_cod,
			(int)EntityType::minecraft_squid,
			(int)EntityType::minecraft_glow_squid,
		}),
		Tag("minecraft:arrows",2,new varInt[2]{
			(int)EntityType::minecraft_arrow,
			(int)EntityType::minecraft_spectral_arrow,
		})
	}),
	Tags(2, "minecraft:game_event", new Tag[2]{
		Tag("minecraft:vibrations",45,new varInt[45]{
			(int)GameEvent::minecraft_block_attach,
			(int)GameEvent::minecraft_block_change,
			(int)GameEvent::minecraft_block_close,
			(int)GameEvent::minecraft_block_destroy,
			(int)GameEvent::minecraft_block_detach,
			(int)GameEvent::minecraft_block_open,
			(int)GameEvent::minecraft_block_place,
			(int)GameEvent::minecraft_block_press,
			(int)GameEvent::minecraft_block_switch,
			(int)GameEvent::minecraft_block_unpress,
			(int)GameEvent::minecraft_block_unswitch,
			(int)GameEvent::minecraft_container_close,
			(int)GameEvent::minecraft_container_open,
			(int)GameEvent::minecraft_dispense_fail,
			(int)GameEvent::minecraft_drinking_finish,
			(int)GameEvent::minecraft_eat,
			(int)GameEvent::minecraft_elytra_free_fall,
			(int)GameEvent::minecraft_entity_damaged,
			(int)GameEvent::minecraft_entity_killed,
			(int)GameEvent::minecraft_entity_place,
			(int)GameEvent::minecraft_equip,
			(int)GameEvent::minecraft_explode,
			(int)GameEvent::minecraft_fishing_rod_cast,
			(int)GameEvent::minecraft_fishing_rod_reel_in,
			(int)GameEvent::minecraft_flap,
			(int)GameEvent::minecraft_fluid_pickup,
			(int)GameEvent::minecraft_fluid_place,
			(int)GameEvent::minecraft_hit_ground,
			(int)GameEvent::minecraft_mob_interact,
			(int)GameEvent::minecraft_lightning_strike,
			(int)GameEvent::minecraft_minecart_moving,
			(int)GameEvent::minecraft_piston_contract,
			(int)GameEvent::minecraft_piston_extend,
			(int)GameEvent::minecraft_prime_fuse,
			(int)GameEvent::minecraft_projectile_land,
			(int)GameEvent::minecraft_projectile_shoot,
			(int)GameEvent::minecraft_ravager_roar,
			(int)GameEvent::minecraft_ring_bell,
			(int)GameEvent::minecraft_shear,
			(int)GameEvent::minecraft_shulker_close,
			(int)GameEvent::minecraft_shulker_open,
			(int)GameEvent::minecraft_splash,
			(int)GameEvent::minecraft_step,
			(int)GameEvent::minecraft_swim,
			(int)GameEvent::minecraft_wolf_shaking
		}),
		Tag("minecraft:ignore_vibrations_sneaking",4,new varInt[4]{
			(int)GameEvent::minecraft_hit_ground,
			(int)GameEvent::minecraft_projectile_shoot,
			(int)GameEvent::minecraft_step,
			(int)GameEvent::minecraft_swim
		})
	})
};*/