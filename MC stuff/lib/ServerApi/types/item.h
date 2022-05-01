#pragma once

#include "typedefs.h"

enum class Item
{
	minecraft_air = 0,
	minecraft_stone = 1,
	minecraft_granite = 2,
	minecraft_polished_granite = 3,
	minecraft_diorite = 4,
	minecraft_polished_diorite = 5,
	minecraft_andesite = 6,
	minecraft_polished_andesite = 7,
	minecraft_deepslate = 8,
	minecraft_cobbled_deepslate = 9,
	minecraft_polished_deepslate = 10,
	minecraft_calcite = 11,
	minecraft_tuff = 12,
	minecraft_dripstone_block = 13,
	minecraft_grass_block = 14,
	minecraft_dirt = 15,
	minecraft_coarse_dirt = 16,
	minecraft_podzol = 17,
	minecraft_rooted_dirt = 18,
	minecraft_crimson_nylium = 19,
	minecraft_warped_nylium = 20,
	minecraft_cobblestone = 21,
	minecraft_oak_planks = 22,
	minecraft_spruce_planks = 23,
	minecraft_birch_planks = 24,
	minecraft_jungle_planks = 25,
	minecraft_acacia_planks = 26,
	minecraft_dark_oak_planks = 27,
	minecraft_crimson_planks = 28,
	minecraft_warped_planks = 29,
	minecraft_oak_sapling = 30,
	minecraft_spruce_sapling = 31,
	minecraft_birch_sapling = 32,
	minecraft_jungle_sapling = 33,
	minecraft_acacia_sapling = 34,
	minecraft_dark_oak_sapling = 35,
	minecraft_bedrock = 36,
	minecraft_sand = 37,
	minecraft_red_sand = 38,
	minecraft_gravel = 39,
	minecraft_coal_ore = 40,
	minecraft_deepslate_coal_ore = 41,
	minecraft_iron_ore = 42,
	minecraft_deepslate_iron_ore = 43,
	minecraft_copper_ore = 44,
	minecraft_deepslate_copper_ore = 45,
	minecraft_gold_ore = 46,
	minecraft_deepslate_gold_ore = 47,
	minecraft_redstone_ore = 48,
	minecraft_deepslate_redstone_ore = 49,
	minecraft_emerald_ore = 50,
	minecraft_deepslate_emerald_ore = 51,
	minecraft_lapis_ore = 52,
	minecraft_deepslate_lapis_ore = 53,
	minecraft_diamond_ore = 54,
	minecraft_deepslate_diamond_ore = 55,
	minecraft_nether_gold_ore = 56,
	minecraft_nether_quartz_ore = 57,
	minecraft_ancient_debris = 58,
	minecraft_coal_block = 59,
	minecraft_raw_iron_block = 60,
	minecraft_raw_copper_block = 61,
	minecraft_raw_gold_block = 62,
	minecraft_amethyst_block = 63,
	minecraft_budding_amethyst = 64,
	minecraft_iron_block = 65,
	minecraft_copper_block = 66,
	minecraft_gold_block = 67,
	minecraft_diamond_block = 68,
	minecraft_netherite_block = 69,
	minecraft_exposed_copper = 70,
	minecraft_weathered_copper = 71,
	minecraft_oxidized_copper = 72,
	minecraft_cut_copper = 73,
	minecraft_exposed_cut_copper = 74,
	minecraft_weathered_cut_copper = 75,
	minecraft_oxidized_cut_copper = 76,
	minecraft_cut_copper_stairs = 77,
	minecraft_exposed_cut_copper_stairs = 78,
	minecraft_weathered_cut_copper_stairs = 79,
	minecraft_oxidized_cut_copper_stairs = 80,
	minecraft_cut_copper_slab = 81,
	minecraft_exposed_cut_copper_slab = 82,
	minecraft_weathered_cut_copper_slab = 83,
	minecraft_oxidized_cut_copper_slab = 84,
	minecraft_waxed_copper_block = 85,
	minecraft_waxed_exposed_copper = 86,
	minecraft_waxed_weathered_copper = 87,
	minecraft_waxed_oxidized_copper = 88,
	minecraft_waxed_cut_copper = 89,
	minecraft_waxed_exposed_cut_copper = 90,
	minecraft_waxed_weathered_cut_copper = 91,
	minecraft_waxed_oxidized_cut_copper = 92,
	minecraft_waxed_cut_copper_stairs = 93,
	minecraft_waxed_exposed_cut_copper_stairs = 94,
	minecraft_waxed_weathered_cut_copper_stairs = 95,
	minecraft_waxed_oxidized_cut_copper_stairs = 96,
	minecraft_waxed_cut_copper_slab = 97,
	minecraft_waxed_exposed_cut_copper_slab = 98,
	minecraft_waxed_weathered_cut_copper_slab = 99,
	minecraft_waxed_oxidized_cut_copper_slab = 100,
	minecraft_oak_log = 101,
	minecraft_spruce_log = 102,
	minecraft_birch_log = 103,
	minecraft_jungle_log = 104,
	minecraft_acacia_log = 105,
	minecraft_dark_oak_log = 106,
	minecraft_crimson_stem = 107,
	minecraft_warped_stem = 108,
	minecraft_stripped_oak_log = 109,
	minecraft_stripped_spruce_log = 110,
	minecraft_stripped_birch_log = 111,
	minecraft_stripped_jungle_log = 112,
	minecraft_stripped_acacia_log = 113,
	minecraft_stripped_dark_oak_log = 114,
	minecraft_stripped_crimson_stem = 115,
	minecraft_stripped_warped_stem = 116,
	minecraft_stripped_oak_wood = 117,
	minecraft_stripped_spruce_wood = 118,
	minecraft_stripped_birch_wood = 119,
	minecraft_stripped_jungle_wood = 120,
	minecraft_stripped_acacia_wood = 121,
	minecraft_stripped_dark_oak_wood = 122,
	minecraft_stripped_crimson_hyphae = 123,
	minecraft_stripped_warped_hyphae = 124,
	minecraft_oak_wood = 125,
	minecraft_spruce_wood = 126,
	minecraft_birch_wood = 127,
	minecraft_jungle_wood = 128,
	minecraft_acacia_wood = 129,
	minecraft_dark_oak_wood = 130,
	minecraft_crimson_hyphae = 131,
	minecraft_warped_hyphae = 132,
	minecraft_oak_leaves = 133,
	minecraft_spruce_leaves = 134,
	minecraft_birch_leaves = 135,
	minecraft_jungle_leaves = 136,
	minecraft_acacia_leaves = 137,
	minecraft_dark_oak_leaves = 138,
	minecraft_azalea_leaves = 139,
	minecraft_flowering_azalea_leaves = 140,
	minecraft_sponge = 141,
	minecraft_wet_sponge = 142,
	minecraft_glass = 143,
	minecraft_tinted_glass = 144,
	minecraft_lapis_block = 145,
	minecraft_sandstone = 146,
	minecraft_chiseled_sandstone = 147,
	minecraft_cut_sandstone = 148,
	minecraft_cobweb = 149,
	minecraft_grass = 150,
	minecraft_fern = 151,
	minecraft_azalea = 152,
	minecraft_flowering_azalea = 153,
	minecraft_dead_bush = 154,
	minecraft_seagrass = 155,
	minecraft_sea_pickle = 156,
	minecraft_white_wool = 157,
	minecraft_orange_wool = 158,
	minecraft_magenta_wool = 159,
	minecraft_light_blue_wool = 160,
	minecraft_yellow_wool = 161,
	minecraft_lime_wool = 162,
	minecraft_pink_wool = 163,
	minecraft_gray_wool = 164,
	minecraft_light_gray_wool = 165,
	minecraft_cyan_wool = 166,
	minecraft_purple_wool = 167,
	minecraft_blue_wool = 168,
	minecraft_brown_wool = 169,
	minecraft_green_wool = 170,
	minecraft_red_wool = 171,
	minecraft_black_wool = 172,
	minecraft_dandelion = 173,
	minecraft_poppy = 174,
	minecraft_blue_orchid = 175,
	minecraft_allium = 176,
	minecraft_azure_bluet = 177,
	minecraft_red_tulip = 178,
	minecraft_orange_tulip = 179,
	minecraft_white_tulip = 180,
	minecraft_pink_tulip = 181,
	minecraft_oxeye_daisy = 182,
	minecraft_cornflower = 183,
	minecraft_lily_of_the_valley = 184,
	minecraft_wither_rose = 185,
	minecraft_spore_blossom = 186,
	minecraft_brown_mushroom = 187,
	minecraft_red_mushroom = 188,
	minecraft_crimson_fungus = 189,
	minecraft_warped_fungus = 190,
	minecraft_crimson_roots = 191,
	minecraft_warped_roots = 192,
	minecraft_nether_sprouts = 193,
	minecraft_weeping_vines = 194,
	minecraft_twisting_vines = 195,
	minecraft_sugar_cane = 196,
	minecraft_kelp = 197,
	minecraft_moss_carpet = 198,
	minecraft_moss_block = 199,
	minecraft_hanging_roots = 200,
	minecraft_big_dripleaf = 201,
	minecraft_small_dripleaf = 202,
	minecraft_bamboo = 203,
	minecraft_oak_slab = 204,
	minecraft_spruce_slab = 205,
	minecraft_birch_slab = 206,
	minecraft_jungle_slab = 207,
	minecraft_acacia_slab = 208,
	minecraft_dark_oak_slab = 209,
	minecraft_crimson_slab = 210,
	minecraft_warped_slab = 211,
	minecraft_stone_slab = 212,
	minecraft_smooth_stone_slab = 213,
	minecraft_sandstone_slab = 214,
	minecraft_cut_sandstone_slab = 215,
	minecraft_petrified_oak_slab = 216,
	minecraft_cobblestone_slab = 217,
	minecraft_brick_slab = 218,
	minecraft_stone_brick_slab = 219,
	minecraft_nether_brick_slab = 220,
	minecraft_quartz_slab = 221,
	minecraft_red_sandstone_slab = 222,
	minecraft_cut_red_sandstone_slab = 223,
	minecraft_purpur_slab = 224,
	minecraft_prismarine_slab = 225,
	minecraft_prismarine_brick_slab = 226,
	minecraft_dark_prismarine_slab = 227,
	minecraft_smooth_quartz = 228,
	minecraft_smooth_red_sandstone = 229,
	minecraft_smooth_sandstone = 230,
	minecraft_smooth_stone = 231,
	minecraft_bricks = 232,
	minecraft_bookshelf = 233,
	minecraft_mossy_cobblestone = 234,
	minecraft_obsidian = 235,
	minecraft_torch = 236,
	minecraft_end_rod = 237,
	minecraft_chorus_plant = 238,
	minecraft_chorus_flower = 239,
	minecraft_purpur_block = 240,
	minecraft_purpur_pillar = 241,
	minecraft_purpur_stairs = 242,
	minecraft_spawner = 243,
	minecraft_oak_stairs = 244,
	minecraft_chest = 245,
	minecraft_crafting_table = 246,
	minecraft_farmland = 247,
	minecraft_furnace = 248,
	minecraft_ladder = 249,
	minecraft_cobblestone_stairs = 250,
	minecraft_snow = 251,
	minecraft_ice = 252,
	minecraft_snow_block = 253,
	minecraft_cactus = 254,
	minecraft_clay = 255,
	minecraft_jukebox = 256,
	minecraft_oak_fence = 257,
	minecraft_spruce_fence = 258,
	minecraft_birch_fence = 259,
	minecraft_jungle_fence = 260,
	minecraft_acacia_fence = 261,
	minecraft_dark_oak_fence = 262,
	minecraft_crimson_fence = 263,
	minecraft_warped_fence = 264,
	minecraft_pumpkin = 265,
	minecraft_carved_pumpkin = 266,
	minecraft_jack_o_lantern = 267,
	minecraft_netherrack = 268,
	minecraft_soul_sand = 269,
	minecraft_soul_soil = 270,
	minecraft_basalt = 271,
	minecraft_polished_basalt = 272,
	minecraft_smooth_basalt = 273,
	minecraft_soul_torch = 274,
	minecraft_glowstone = 275,
	minecraft_infested_stone = 276,
	minecraft_infested_cobblestone = 277,
	minecraft_infested_stone_bricks = 278,
	minecraft_infested_mossy_stone_bricks = 279,
	minecraft_infested_cracked_stone_bricks = 280,
	minecraft_infested_chiseled_stone_bricks = 281,
	minecraft_infested_deepslate = 282,
	minecraft_stone_bricks = 283,
	minecraft_mossy_stone_bricks = 284,
	minecraft_cracked_stone_bricks = 285,
	minecraft_chiseled_stone_bricks = 286,
	minecraft_deepslate_bricks = 287,
	minecraft_cracked_deepslate_bricks = 288,
	minecraft_deepslate_tiles = 289,
	minecraft_cracked_deepslate_tiles = 290,
	minecraft_chiseled_deepslate = 291,
	minecraft_brown_mushroom_block = 292,
	minecraft_red_mushroom_block = 293,
	minecraft_mushroom_stem = 294,
	minecraft_iron_bars = 295,
	minecraft_chain = 296,
	minecraft_glass_pane = 297,
	minecraft_melon = 298,
	minecraft_vine = 299,
	minecraft_glow_lichen = 300,
	minecraft_brick_stairs = 301,
	minecraft_stone_brick_stairs = 302,
	minecraft_mycelium = 303,
	minecraft_lily_pad = 304,
	minecraft_nether_bricks = 305,
	minecraft_cracked_nether_bricks = 306,
	minecraft_chiseled_nether_bricks = 307,
	minecraft_nether_brick_fence = 308,
	minecraft_nether_brick_stairs = 309,
	minecraft_enchanting_table = 310,
	minecraft_end_portal_frame = 311,
	minecraft_end_stone = 312,
	minecraft_end_stone_bricks = 313,
	minecraft_dragon_egg = 314,
	minecraft_sandstone_stairs = 315,
	minecraft_ender_chest = 316,
	minecraft_emerald_block = 317,
	minecraft_spruce_stairs = 318,
	minecraft_birch_stairs = 319,
	minecraft_jungle_stairs = 320,
	minecraft_crimson_stairs = 321,
	minecraft_warped_stairs = 322,
	minecraft_command_block = 323,
	minecraft_beacon = 324,
	minecraft_cobblestone_wall = 325,
	minecraft_mossy_cobblestone_wall = 326,
	minecraft_brick_wall = 327,
	minecraft_prismarine_wall = 328,
	minecraft_red_sandstone_wall = 329,
	minecraft_mossy_stone_brick_wall = 330,
	minecraft_granite_wall = 331,
	minecraft_stone_brick_wall = 332,
	minecraft_nether_brick_wall = 333,
	minecraft_andesite_wall = 334,
	minecraft_red_nether_brick_wall = 335,
	minecraft_sandstone_wall = 336,
	minecraft_end_stone_brick_wall = 337,
	minecraft_diorite_wall = 338,
	minecraft_blackstone_wall = 339,
	minecraft_polished_blackstone_wall = 340,
	minecraft_polished_blackstone_brick_wall = 341,
	minecraft_cobbled_deepslate_wall = 342,
	minecraft_polished_deepslate_wall = 343,
	minecraft_deepslate_brick_wall = 344,
	minecraft_deepslate_tile_wall = 345,
	minecraft_anvil = 346,
	minecraft_chipped_anvil = 347,
	minecraft_damaged_anvil = 348,
	minecraft_chiseled_quartz_block = 349,
	minecraft_quartz_block = 350,
	minecraft_quartz_bricks = 351,
	minecraft_quartz_pillar = 352,
	minecraft_quartz_stairs = 353,
	minecraft_white_terracotta = 354,
	minecraft_orange_terracotta = 355,
	minecraft_magenta_terracotta = 356,
	minecraft_light_blue_terracotta = 357,
	minecraft_yellow_terracotta = 358,
	minecraft_lime_terracotta = 359,
	minecraft_pink_terracotta = 360,
	minecraft_gray_terracotta = 361,
	minecraft_light_gray_terracotta = 362,
	minecraft_cyan_terracotta = 363,
	minecraft_purple_terracotta = 364,
	minecraft_blue_terracotta = 365,
	minecraft_brown_terracotta = 366,
	minecraft_green_terracotta = 367,
	minecraft_red_terracotta = 368,
	minecraft_black_terracotta = 369,
	minecraft_barrier = 370,
	minecraft_light = 371,
	minecraft_hay_block = 372,
	minecraft_white_carpet = 373,
	minecraft_orange_carpet = 374,
	minecraft_magenta_carpet = 375,
	minecraft_light_blue_carpet = 376,
	minecraft_yellow_carpet = 377,
	minecraft_lime_carpet = 378,
	minecraft_pink_carpet = 379,
	minecraft_gray_carpet = 380,
	minecraft_light_gray_carpet = 381,
	minecraft_cyan_carpet = 382,
	minecraft_purple_carpet = 383,
	minecraft_blue_carpet = 384,
	minecraft_brown_carpet = 385,
	minecraft_green_carpet = 386,
	minecraft_red_carpet = 387,
	minecraft_black_carpet = 388,
	minecraft_terracotta = 389,
	minecraft_packed_ice = 390,
	minecraft_acacia_stairs = 391,
	minecraft_dark_oak_stairs = 392,
	minecraft_dirt_path = 393,
	minecraft_sunflower = 394,
	minecraft_lilac = 395,
	minecraft_rose_bush = 396,
	minecraft_peony = 397,
	minecraft_tall_grass = 398,
	minecraft_large_fern = 399,
	minecraft_white_stained_glass = 400,
	minecraft_orange_stained_glass = 401,
	minecraft_magenta_stained_glass = 402,
	minecraft_light_blue_stained_glass = 403,
	minecraft_yellow_stained_glass = 404,
	minecraft_lime_stained_glass = 405,
	minecraft_pink_stained_glass = 406,
	minecraft_gray_stained_glass = 407,
	minecraft_light_gray_stained_glass = 408,
	minecraft_cyan_stained_glass = 409,
	minecraft_purple_stained_glass = 410,
	minecraft_blue_stained_glass = 411,
	minecraft_brown_stained_glass = 412,
	minecraft_green_stained_glass = 413,
	minecraft_red_stained_glass = 414,
	minecraft_black_stained_glass = 415,
	minecraft_white_stained_glass_pane = 416,
	minecraft_orange_stained_glass_pane = 417,
	minecraft_magenta_stained_glass_pane = 418,
	minecraft_light_blue_stained_glass_pane = 419,
	minecraft_yellow_stained_glass_pane = 420,
	minecraft_lime_stained_glass_pane = 421,
	minecraft_pink_stained_glass_pane = 422,
	minecraft_gray_stained_glass_pane = 423,
	minecraft_light_gray_stained_glass_pane = 424,
	minecraft_cyan_stained_glass_pane = 425,
	minecraft_purple_stained_glass_pane = 426,
	minecraft_blue_stained_glass_pane = 427,
	minecraft_brown_stained_glass_pane = 428,
	minecraft_green_stained_glass_pane = 429,
	minecraft_red_stained_glass_pane = 430,
	minecraft_black_stained_glass_pane = 431,
	minecraft_prismarine = 432,
	minecraft_prismarine_bricks = 433,
	minecraft_dark_prismarine = 434,
	minecraft_prismarine_stairs = 435,
	minecraft_prismarine_brick_stairs = 436,
	minecraft_dark_prismarine_stairs = 437,
	minecraft_sea_lantern = 438,
	minecraft_red_sandstone = 439,
	minecraft_chiseled_red_sandstone = 440,
	minecraft_cut_red_sandstone = 441,
	minecraft_red_sandstone_stairs = 442,
	minecraft_repeating_command_block = 443,
	minecraft_chain_command_block = 444,
	minecraft_magma_block = 445,
	minecraft_nether_wart_block = 446,
	minecraft_warped_wart_block = 447,
	minecraft_red_nether_bricks = 448,
	minecraft_bone_block = 449,
	minecraft_structure_void = 450,
	minecraft_shulker_box = 451,
	minecraft_white_shulker_box = 452,
	minecraft_orange_shulker_box = 453,
	minecraft_magenta_shulker_box = 454,
	minecraft_light_blue_shulker_box = 455,
	minecraft_yellow_shulker_box = 456,
	minecraft_lime_shulker_box = 457,
	minecraft_pink_shulker_box = 458,
	minecraft_gray_shulker_box = 459,
	minecraft_light_gray_shulker_box = 460,
	minecraft_cyan_shulker_box = 461,
	minecraft_purple_shulker_box = 462,
	minecraft_blue_shulker_box = 463,
	minecraft_brown_shulker_box = 464,
	minecraft_green_shulker_box = 465,
	minecraft_red_shulker_box = 466,
	minecraft_black_shulker_box = 467,
	minecraft_white_glazed_terracotta = 468,
	minecraft_orange_glazed_terracotta = 469,
	minecraft_magenta_glazed_terracotta = 470,
	minecraft_light_blue_glazed_terracotta = 471,
	minecraft_yellow_glazed_terracotta = 472,
	minecraft_lime_glazed_terracotta = 473,
	minecraft_pink_glazed_terracotta = 474,
	minecraft_gray_glazed_terracotta = 475,
	minecraft_light_gray_glazed_terracotta = 476,
	minecraft_cyan_glazed_terracotta = 477,
	minecraft_purple_glazed_terracotta = 478,
	minecraft_blue_glazed_terracotta = 479,
	minecraft_brown_glazed_terracotta = 480,
	minecraft_green_glazed_terracotta = 481,
	minecraft_red_glazed_terracotta = 482,
	minecraft_black_glazed_terracotta = 483,
	minecraft_white_concrete = 484,
	minecraft_orange_concrete = 485,
	minecraft_magenta_concrete = 486,
	minecraft_light_blue_concrete = 487,
	minecraft_yellow_concrete = 488,
	minecraft_lime_concrete = 489,
	minecraft_pink_concrete = 490,
	minecraft_gray_concrete = 491,
	minecraft_light_gray_concrete = 492,
	minecraft_cyan_concrete = 493,
	minecraft_purple_concrete = 494,
	minecraft_blue_concrete = 495,
	minecraft_brown_concrete = 496,
	minecraft_green_concrete = 497,
	minecraft_red_concrete = 498,
	minecraft_black_concrete = 499,
	minecraft_white_concrete_powder = 500,
	minecraft_orange_concrete_powder = 501,
	minecraft_magenta_concrete_powder = 502,
	minecraft_light_blue_concrete_powder = 503,
	minecraft_yellow_concrete_powder = 504,
	minecraft_lime_concrete_powder = 505,
	minecraft_pink_concrete_powder = 506,
	minecraft_gray_concrete_powder = 507,
	minecraft_light_gray_concrete_powder = 508,
	minecraft_cyan_concrete_powder = 509,
	minecraft_purple_concrete_powder = 510,
	minecraft_blue_concrete_powder = 511,
	minecraft_brown_concrete_powder = 512,
	minecraft_green_concrete_powder = 513,
	minecraft_red_concrete_powder = 514,
	minecraft_black_concrete_powder = 515,
	minecraft_turtle_egg = 516,
	minecraft_dead_tube_coral_block = 517,
	minecraft_dead_brain_coral_block = 518,
	minecraft_dead_bubble_coral_block = 519,
	minecraft_dead_fire_coral_block = 520,
	minecraft_dead_horn_coral_block = 521,
	minecraft_tube_coral_block = 522,
	minecraft_brain_coral_block = 523,
	minecraft_bubble_coral_block = 524,
	minecraft_fire_coral_block = 525,
	minecraft_horn_coral_block = 526,
	minecraft_tube_coral = 527,
	minecraft_brain_coral = 528,
	minecraft_bubble_coral = 529,
	minecraft_fire_coral = 530,
	minecraft_horn_coral = 531,
	minecraft_dead_brain_coral = 532,
	minecraft_dead_bubble_coral = 533,
	minecraft_dead_fire_coral = 534,
	minecraft_dead_horn_coral = 535,
	minecraft_dead_tube_coral = 536,
	minecraft_tube_coral_fan = 537,
	minecraft_brain_coral_fan = 538,
	minecraft_bubble_coral_fan = 539,
	minecraft_fire_coral_fan = 540,
	minecraft_horn_coral_fan = 541,
	minecraft_dead_tube_coral_fan = 542,
	minecraft_dead_brain_coral_fan = 543,
	minecraft_dead_bubble_coral_fan = 544,
	minecraft_dead_fire_coral_fan = 545,
	minecraft_dead_horn_coral_fan = 546,
	minecraft_blue_ice = 547,
	minecraft_conduit = 548,
	minecraft_polished_granite_stairs = 549,
	minecraft_smooth_red_sandstone_stairs = 550,
	minecraft_mossy_stone_brick_stairs = 551,
	minecraft_polished_diorite_stairs = 552,
	minecraft_mossy_cobblestone_stairs = 553,
	minecraft_end_stone_brick_stairs = 554,
	minecraft_stone_stairs = 555,
	minecraft_smooth_sandstone_stairs = 556,
	minecraft_smooth_quartz_stairs = 557,
	minecraft_granite_stairs = 558,
	minecraft_andesite_stairs = 559,
	minecraft_red_nether_brick_stairs = 560,
	minecraft_polished_andesite_stairs = 561,
	minecraft_diorite_stairs = 562,
	minecraft_cobbled_deepslate_stairs = 563,
	minecraft_polished_deepslate_stairs = 564,
	minecraft_deepslate_brick_stairs = 565,
	minecraft_deepslate_tile_stairs = 566,
	minecraft_polished_granite_slab = 567,
	minecraft_smooth_red_sandstone_slab = 568,
	minecraft_mossy_stone_brick_slab = 569,
	minecraft_polished_diorite_slab = 570,
	minecraft_mossy_cobblestone_slab = 571,
	minecraft_end_stone_brick_slab = 572,
	minecraft_smooth_sandstone_slab = 573,
	minecraft_smooth_quartz_slab = 574,
	minecraft_granite_slab = 575,
	minecraft_andesite_slab = 576,
	minecraft_red_nether_brick_slab = 577,
	minecraft_polished_andesite_slab = 578,
	minecraft_diorite_slab = 579,
	minecraft_cobbled_deepslate_slab = 580,
	minecraft_polished_deepslate_slab = 581,
	minecraft_deepslate_brick_slab = 582,
	minecraft_deepslate_tile_slab = 583,
	minecraft_scaffolding = 584,
	minecraft_redstone = 585,
	minecraft_redstone_torch = 586,
	minecraft_redstone_block = 587,
	minecraft_repeater = 588,
	minecraft_comparator = 589,
	minecraft_piston = 590,
	minecraft_sticky_piston = 591,
	minecraft_slime_block = 592,
	minecraft_honey_block = 593,
	minecraft_observer = 594,
	minecraft_hopper = 595,
	minecraft_dispenser = 596,
	minecraft_dropper = 597,
	minecraft_lectern = 598,
	minecraft_target = 599,
	minecraft_lever = 600,
	minecraft_lightning_rod = 601,
	minecraft_daylight_detector = 602,
	minecraft_sculk_sensor = 603,
	minecraft_tripwire_hook = 604,
	minecraft_trapped_chest = 605,
	minecraft_tnt = 606,
	minecraft_redstone_lamp = 607,
	minecraft_note_block = 608,
	minecraft_stone_button = 609,
	minecraft_polished_blackstone_button = 610,
	minecraft_oak_button = 611,
	minecraft_spruce_button = 612,
	minecraft_birch_button = 613,
	minecraft_jungle_button = 614,
	minecraft_acacia_button = 615,
	minecraft_dark_oak_button = 616,
	minecraft_crimson_button = 617,
	minecraft_warped_button = 618,
	minecraft_stone_pressure_plate = 619,
	minecraft_polished_blackstone_pressure_plate = 620,
	minecraft_light_weighted_pressure_plate = 621,
	minecraft_heavy_weighted_pressure_plate = 622,
	minecraft_oak_pressure_plate = 623,
	minecraft_spruce_pressure_plate = 624,
	minecraft_birch_pressure_plate = 625,
	minecraft_jungle_pressure_plate = 626,
	minecraft_acacia_pressure_plate = 627,
	minecraft_dark_oak_pressure_plate = 628,
	minecraft_crimson_pressure_plate = 629,
	minecraft_warped_pressure_plate = 630,
	minecraft_iron_door = 631,
	minecraft_oak_door = 632,
	minecraft_spruce_door = 633,
	minecraft_birch_door = 634,
	minecraft_jungle_door = 635,
	minecraft_acacia_door = 636,
	minecraft_dark_oak_door = 637,
	minecraft_crimson_door = 638,
	minecraft_warped_door = 639,
	minecraft_iron_trapdoor = 640,
	minecraft_oak_trapdoor = 641,
	minecraft_spruce_trapdoor = 642,
	minecraft_birch_trapdoor = 643,
	minecraft_jungle_trapdoor = 644,
	minecraft_acacia_trapdoor = 645,
	minecraft_dark_oak_trapdoor = 646,
	minecraft_crimson_trapdoor = 647,
	minecraft_warped_trapdoor = 648,
	minecraft_oak_fence_gate = 649,
	minecraft_spruce_fence_gate = 650,
	minecraft_birch_fence_gate = 651,
	minecraft_jungle_fence_gate = 652,
	minecraft_acacia_fence_gate = 653,
	minecraft_dark_oak_fence_gate = 654,
	minecraft_crimson_fence_gate = 655,
	minecraft_warped_fence_gate = 656,
	minecraft_powered_rail = 657,
	minecraft_detector_rail = 658,
	minecraft_rail = 659,
	minecraft_activator_rail = 660,
	minecraft_saddle = 661,
	minecraft_minecart = 662,
	minecraft_chest_minecart = 663,
	minecraft_furnace_minecart = 664,
	minecraft_tnt_minecart = 665,
	minecraft_hopper_minecart = 666,
	minecraft_carrot_on_a_stick = 667,
	minecraft_warped_fungus_on_a_stick = 668,
	minecraft_elytra = 669,
	minecraft_oak_boat = 670,
	minecraft_spruce_boat = 671,
	minecraft_birch_boat = 672,
	minecraft_jungle_boat = 673,
	minecraft_acacia_boat = 674,
	minecraft_dark_oak_boat = 675,
	minecraft_structure_block = 676,
	minecraft_jigsaw = 677,
	minecraft_turtle_helmet = 678,
	minecraft_scute = 679,
	minecraft_flint_and_steel = 680,
	minecraft_apple = 681,
	minecraft_bow = 682,
	minecraft_arrow = 683,
	minecraft_coal = 684,
	minecraft_charcoal = 685,
	minecraft_diamond = 686,
	minecraft_emerald = 687,
	minecraft_lapis_lazuli = 688,
	minecraft_quartz = 689,
	minecraft_amethyst_shard = 690,
	minecraft_raw_iron = 691,
	minecraft_iron_ingot = 692,
	minecraft_raw_copper = 693,
	minecraft_copper_ingot = 694,
	minecraft_raw_gold = 695,
	minecraft_gold_ingot = 696,
	minecraft_netherite_ingot = 697,
	minecraft_netherite_scrap = 698,
	minecraft_wooden_sword = 699,
	minecraft_wooden_shovel = 700,
	minecraft_wooden_pickaxe = 701,
	minecraft_wooden_axe = 702,
	minecraft_wooden_hoe = 703,
	minecraft_stone_sword = 704,
	minecraft_stone_shovel = 705,
	minecraft_stone_pickaxe = 706,
	minecraft_stone_axe = 707,
	minecraft_stone_hoe = 708,
	minecraft_golden_sword = 709,
	minecraft_golden_shovel = 710,
	minecraft_golden_pickaxe = 711,
	minecraft_golden_axe = 712,
	minecraft_golden_hoe = 713,
	minecraft_iron_sword = 714,
	minecraft_iron_shovel = 715,
	minecraft_iron_pickaxe = 716,
	minecraft_iron_axe = 717,
	minecraft_iron_hoe = 718,
	minecraft_diamond_sword = 719,
	minecraft_diamond_shovel = 720,
	minecraft_diamond_pickaxe = 721,
	minecraft_diamond_axe = 722,
	minecraft_diamond_hoe = 723,
	minecraft_netherite_sword = 724,
	minecraft_netherite_shovel = 725,
	minecraft_netherite_pickaxe = 726,
	minecraft_netherite_axe = 727,
	minecraft_netherite_hoe = 728,
	minecraft_stick = 729,
	minecraft_bowl = 730,
	minecraft_mushroom_stew = 731,
	minecraft_string = 732,
	minecraft_feather = 733,
	minecraft_gunpowder = 734,
	minecraft_wheat_seeds = 735,
	minecraft_wheat = 736,
	minecraft_bread = 737,
	minecraft_leather_helmet = 738,
	minecraft_leather_chestplate = 739,
	minecraft_leather_leggings = 740,
	minecraft_leather_boots = 741,
	minecraft_chainmail_helmet = 742,
	minecraft_chainmail_chestplate = 743,
	minecraft_chainmail_leggings = 744,
	minecraft_chainmail_boots = 745,
	minecraft_iron_helmet = 746,
	minecraft_iron_chestplate = 747,
	minecraft_iron_leggings = 748,
	minecraft_iron_boots = 749,
	minecraft_diamond_helmet = 750,
	minecraft_diamond_chestplate = 751,
	minecraft_diamond_leggings = 752,
	minecraft_diamond_boots = 753,
	minecraft_golden_helmet = 754,
	minecraft_golden_chestplate = 755,
	minecraft_golden_leggings = 756,
	minecraft_golden_boots = 757,
	minecraft_netherite_helmet = 758,
	minecraft_netherite_chestplate = 759,
	minecraft_netherite_leggings = 760,
	minecraft_netherite_boots = 761,
	minecraft_flint = 762,
	minecraft_porkchop = 763,
	minecraft_cooked_porkchop = 764,
	minecraft_painting = 765,
	minecraft_golden_apple = 766,
	minecraft_enchanted_golden_apple = 767,
	minecraft_oak_sign = 768,
	minecraft_spruce_sign = 769,
	minecraft_birch_sign = 770,
	minecraft_jungle_sign = 771,
	minecraft_acacia_sign = 772,
	minecraft_dark_oak_sign = 773,
	minecraft_crimson_sign = 774,
	minecraft_warped_sign = 775,
	minecraft_bucket = 776,
	minecraft_water_bucket = 777,
	minecraft_lava_bucket = 778,
	minecraft_powder_snow_bucket = 779,
	minecraft_snowball = 780,
	minecraft_leather = 781,
	minecraft_milk_bucket = 782,
	minecraft_pufferfish_bucket = 783,
	minecraft_salmon_bucket = 784,
	minecraft_cod_bucket = 785,
	minecraft_tropical_fish_bucket = 786,
	minecraft_axolotl_bucket = 787,
	minecraft_brick = 788,
	minecraft_clay_ball = 789,
	minecraft_dried_kelp_block = 790,
	minecraft_paper = 791,
	minecraft_book = 792,
	minecraft_slime_ball = 793,
	minecraft_egg = 794,
	minecraft_compass = 795,
	minecraft_bundle = 796,
	minecraft_fishing_rod = 797,
	minecraft_clock = 798,
	minecraft_spyglass = 799,
	minecraft_glowstone_dust = 800,
	minecraft_cod = 801,
	minecraft_salmon = 802,
	minecraft_tropical_fish = 803,
	minecraft_pufferfish = 804,
	minecraft_cooked_cod = 805,
	minecraft_cooked_salmon = 806,
	minecraft_ink_sac = 807,
	minecraft_glow_ink_sac = 808,
	minecraft_cocoa_beans = 809,
	minecraft_white_dye = 810,
	minecraft_orange_dye = 811,
	minecraft_magenta_dye = 812,
	minecraft_light_blue_dye = 813,
	minecraft_yellow_dye = 814,
	minecraft_lime_dye = 815,
	minecraft_pink_dye = 816,
	minecraft_gray_dye = 817,
	minecraft_light_gray_dye = 818,
	minecraft_cyan_dye = 819,
	minecraft_purple_dye = 820,
	minecraft_blue_dye = 821,
	minecraft_brown_dye = 822,
	minecraft_green_dye = 823,
	minecraft_red_dye = 824,
	minecraft_black_dye = 825,
	minecraft_bone_meal = 826,
	minecraft_bone = 827,
	minecraft_sugar = 828,
	minecraft_cake = 829,
	minecraft_white_bed = 830,
	minecraft_orange_bed = 831,
	minecraft_magenta_bed = 832,
	minecraft_light_blue_bed = 833,
	minecraft_yellow_bed = 834,
	minecraft_lime_bed = 835,
	minecraft_pink_bed = 836,
	minecraft_gray_bed = 837,
	minecraft_light_gray_bed = 838,
	minecraft_cyan_bed = 839,
	minecraft_purple_bed = 840,
	minecraft_blue_bed = 841,
	minecraft_brown_bed = 842,
	minecraft_green_bed = 843,
	minecraft_red_bed = 844,
	minecraft_black_bed = 845,
	minecraft_cookie = 846,
	minecraft_filled_map = 847,
	minecraft_shears = 848,
	minecraft_melon_slice = 849,
	minecraft_dried_kelp = 850,
	minecraft_pumpkin_seeds = 851,
	minecraft_melon_seeds = 852,
	minecraft_beef = 853,
	minecraft_cooked_beef = 854,
	minecraft_chicken = 855,
	minecraft_cooked_chicken = 856,
	minecraft_rotten_flesh = 857,
	minecraft_ender_pearl = 858,
	minecraft_blaze_rod = 859,
	minecraft_ghast_tear = 860,
	minecraft_gold_nugget = 861,
	minecraft_nether_wart = 862,
	minecraft_potion = 863,
	minecraft_glass_bottle = 864,
	minecraft_spider_eye = 865,
	minecraft_fermented_spider_eye = 866,
	minecraft_blaze_powder = 867,
	minecraft_magma_cream = 868,
	minecraft_brewing_stand = 869,
	minecraft_cauldron = 870,
	minecraft_ender_eye = 871,
	minecraft_glistering_melon_slice = 872,
	minecraft_axolotl_spawn_egg = 873,
	minecraft_bat_spawn_egg = 874,
	minecraft_bee_spawn_egg = 875,
	minecraft_blaze_spawn_egg = 876,
	minecraft_cat_spawn_egg = 877,
	minecraft_cave_spider_spawn_egg = 878,
	minecraft_chicken_spawn_egg = 879,
	minecraft_cod_spawn_egg = 880,
	minecraft_cow_spawn_egg = 881,
	minecraft_creeper_spawn_egg = 882,
	minecraft_dolphin_spawn_egg = 883,
	minecraft_donkey_spawn_egg = 884,
	minecraft_drowned_spawn_egg = 885,
	minecraft_elder_guardian_spawn_egg = 886,
	minecraft_enderman_spawn_egg = 887,
	minecraft_endermite_spawn_egg = 888,
	minecraft_evoker_spawn_egg = 889,
	minecraft_fox_spawn_egg = 890,
	minecraft_ghast_spawn_egg = 891,
	minecraft_glow_squid_spawn_egg = 892,
	minecraft_goat_spawn_egg = 893,
	minecraft_guardian_spawn_egg = 894,
	minecraft_hoglin_spawn_egg = 895,
	minecraft_horse_spawn_egg = 896,
	minecraft_husk_spawn_egg = 897,
	minecraft_llama_spawn_egg = 898,
	minecraft_magma_cube_spawn_egg = 899,
	minecraft_mooshroom_spawn_egg = 900,
	minecraft_mule_spawn_egg = 901,
	minecraft_ocelot_spawn_egg = 902,
	minecraft_panda_spawn_egg = 903,
	minecraft_parrot_spawn_egg = 904,
	minecraft_phantom_spawn_egg = 905,
	minecraft_pig_spawn_egg = 906,
	minecraft_piglin_spawn_egg = 907,
	minecraft_piglin_brute_spawn_egg = 908,
	minecraft_pillager_spawn_egg = 909,
	minecraft_polar_bear_spawn_egg = 910,
	minecraft_pufferfish_spawn_egg = 911,
	minecraft_rabbit_spawn_egg = 912,
	minecraft_ravager_spawn_egg = 913,
	minecraft_salmon_spawn_egg = 914,
	minecraft_sheep_spawn_egg = 915,
	minecraft_shulker_spawn_egg = 916,
	minecraft_silverfish_spawn_egg = 917,
	minecraft_skeleton_spawn_egg = 918,
	minecraft_skeleton_horse_spawn_egg = 919,
	minecraft_slime_spawn_egg = 920,
	minecraft_spider_spawn_egg = 921,
	minecraft_squid_spawn_egg = 922,
	minecraft_stray_spawn_egg = 923,
	minecraft_strider_spawn_egg = 924,
	minecraft_trader_llama_spawn_egg = 925,
	minecraft_tropical_fish_spawn_egg = 926,
	minecraft_turtle_spawn_egg = 927,
	minecraft_vex_spawn_egg = 928,
	minecraft_villager_spawn_egg = 929,
	minecraft_vindicator_spawn_egg = 930,
	minecraft_wandering_trader_spawn_egg = 931,
	minecraft_witch_spawn_egg = 932,
	minecraft_wither_skeleton_spawn_egg = 933,
	minecraft_wolf_spawn_egg = 934,
	minecraft_zoglin_spawn_egg = 935,
	minecraft_zombie_spawn_egg = 936,
	minecraft_zombie_horse_spawn_egg = 937,
	minecraft_zombie_villager_spawn_egg = 938,
	minecraft_zombified_piglin_spawn_egg = 939,
	minecraft_experience_bottle = 940,
	minecraft_fire_charge = 941,
	minecraft_writable_book = 942,
	minecraft_written_book = 943,
	minecraft_item_frame = 944,
	minecraft_glow_item_frame = 945,
	minecraft_flower_pot = 946,
	minecraft_carrot = 947,
	minecraft_potato = 948,
	minecraft_baked_potato = 949,
	minecraft_poisonous_potato = 950,
	minecraft_map = 951,
	minecraft_golden_carrot = 952,
	minecraft_skeleton_skull = 953,
	minecraft_wither_skeleton_skull = 954,
	minecraft_player_head = 955,
	minecraft_zombie_head = 956,
	minecraft_creeper_head = 957,
	minecraft_dragon_head = 958,
	minecraft_nether_star = 959,
	minecraft_pumpkin_pie = 960,
	minecraft_firework_rocket = 961,
	minecraft_firework_star = 962,
	minecraft_enchanted_book = 963,
	minecraft_nether_brick = 964,
	minecraft_prismarine_shard = 965,
	minecraft_prismarine_crystals = 966,
	minecraft_rabbit = 967,
	minecraft_cooked_rabbit = 968,
	minecraft_rabbit_stew = 969,
	minecraft_rabbit_foot = 970,
	minecraft_rabbit_hide = 971,
	minecraft_armor_stand = 972,
	minecraft_iron_horse_armor = 973,
	minecraft_golden_horse_armor = 974,
	minecraft_diamond_horse_armor = 975,
	minecraft_leather_horse_armor = 976,
	minecraft_lead = 977,
	minecraft_name_tag = 978,
	minecraft_command_block_minecart = 979,
	minecraft_mutton = 980,
	minecraft_cooked_mutton = 981,
	minecraft_white_banner = 982,
	minecraft_orange_banner = 983,
	minecraft_magenta_banner = 984,
	minecraft_light_blue_banner = 985,
	minecraft_yellow_banner = 986,
	minecraft_lime_banner = 987,
	minecraft_pink_banner = 988,
	minecraft_gray_banner = 989,
	minecraft_light_gray_banner = 990,
	minecraft_cyan_banner = 991,
	minecraft_purple_banner = 992,
	minecraft_blue_banner = 993,
	minecraft_brown_banner = 994,
	minecraft_green_banner = 995,
	minecraft_red_banner = 996,
	minecraft_black_banner = 997,
	minecraft_end_crystal = 998,
	minecraft_chorus_fruit = 999,
	minecraft_popped_chorus_fruit = 1000,
	minecraft_beetroot = 1001,
	minecraft_beetroot_seeds = 1002,
	minecraft_beetroot_soup = 1003,
	minecraft_dragon_breath = 1004,
	minecraft_splash_potion = 1005,
	minecraft_spectral_arrow = 1006,
	minecraft_tipped_arrow = 1007,
	minecraft_lingering_potion = 1008,
	minecraft_shield = 1009,
	minecraft_totem_of_undying = 1010,
	minecraft_shulker_shell = 1011,
	minecraft_iron_nugget = 1012,
	minecraft_knowledge_book = 1013,
	minecraft_debug_stick = 1014,
	minecraft_music_disc_13 = 1015,
	minecraft_music_disc_cat = 1016,
	minecraft_music_disc_blocks = 1017,
	minecraft_music_disc_chirp = 1018,
	minecraft_music_disc_far = 1019,
	minecraft_music_disc_mall = 1020,
	minecraft_music_disc_mellohi = 1021,
	minecraft_music_disc_stal = 1022,
	minecraft_music_disc_strad = 1023,
	minecraft_music_disc_ward = 1024,
	minecraft_music_disc_11 = 1025,
	minecraft_music_disc_wait = 1026,
	minecraft_music_disc_otherside = 1027,
	minecraft_music_disc_pigstep = 1028,
	minecraft_trident = 1029,
	minecraft_phantom_membrane = 1030,
	minecraft_nautilus_shell = 1031,
	minecraft_heart_of_the_sea = 1032,
	minecraft_crossbow = 1033,
	minecraft_suspicious_stew = 1034,
	minecraft_loom = 1035,
	minecraft_flower_banner_pattern = 1036,
	minecraft_creeper_banner_pattern = 1037,
	minecraft_skull_banner_pattern = 1038,
	minecraft_mojang_banner_pattern = 1039,
	minecraft_globe_banner_pattern = 1040,
	minecraft_piglin_banner_pattern = 1041,
	minecraft_composter = 1042,
	minecraft_barrel = 1043,
	minecraft_smoker = 1044,
	minecraft_blast_furnace = 1045,
	minecraft_cartography_table = 1046,
	minecraft_fletching_table = 1047,
	minecraft_grindstone = 1048,
	minecraft_smithing_table = 1049,
	minecraft_stonecutter = 1050,
	minecraft_bell = 1051,
	minecraft_lantern = 1052,
	minecraft_soul_lantern = 1053,
	minecraft_sweet_berries = 1054,
	minecraft_glow_berries = 1055,
	minecraft_campfire = 1056,
	minecraft_soul_campfire = 1057,
	minecraft_shroomlight = 1058,
	minecraft_honeycomb = 1059,
	minecraft_bee_nest = 1060,
	minecraft_beehive = 1061,
	minecraft_honey_bottle = 1062,
	minecraft_honeycomb_block = 1063,
	minecraft_lodestone = 1064,
	minecraft_crying_obsidian = 1065,
	minecraft_blackstone = 1066,
	minecraft_blackstone_slab = 1067,
	minecraft_blackstone_stairs = 1068,
	minecraft_gilded_blackstone = 1069,
	minecraft_polished_blackstone = 1070,
	minecraft_polished_blackstone_slab = 1071,
	minecraft_polished_blackstone_stairs = 1072,
	minecraft_chiseled_polished_blackstone = 1073,
	minecraft_polished_blackstone_bricks = 1074,
	minecraft_polished_blackstone_brick_slab = 1075,
	minecraft_polished_blackstone_brick_stairs = 1076,
	minecraft_cracked_polished_blackstone_bricks = 1077,
	minecraft_respawn_anchor = 1078,
	minecraft_candle = 1079,
	minecraft_white_candle = 1080,
	minecraft_orange_candle = 1081,
	minecraft_magenta_candle = 1082,
	minecraft_light_blue_candle = 1083,
	minecraft_yellow_candle = 1084,
	minecraft_lime_candle = 1085,
	minecraft_pink_candle = 1086,
	minecraft_gray_candle = 1087,
	minecraft_light_gray_candle = 1088,
	minecraft_cyan_candle = 1089,
	minecraft_purple_candle = 1090,
	minecraft_blue_candle = 1091,
	minecraft_brown_candle = 1092,
	minecraft_green_candle = 1093,
	minecraft_red_candle = 1094,
	minecraft_black_candle = 1095,
	minecraft_small_amethyst_bud = 1096,
	minecraft_medium_amethyst_bud = 1097,
	minecraft_large_amethyst_bud = 1098,
	minecraft_amethyst_cluster = 1099,
	minecraft_pointed_dripstone = 1100,
};

namespace items
{
	Byte getStackableSize(int i);
}