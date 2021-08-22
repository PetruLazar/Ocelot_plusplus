#pragma once

enum class GameEvent
{
	minecraft_block_attach = 0,
	minecraft_block_change = 1,
	minecraft_block_close = 2,
	minecraft_block_destroy = 3,
	minecraft_block_detach = 4,
	minecraft_block_open = 5,
	minecraft_block_place = 6,
	minecraft_block_press = 7,
	minecraft_block_switch = 8,
	minecraft_block_unpress = 9,
	minecraft_block_unswitch = 10,
	minecraft_container_close = 11,
	minecraft_container_open = 12,
	minecraft_dispense_fail = 13,
	minecraft_drinking_finish = 14,
	minecraft_eat = 15,
	minecraft_elytra_free_fall = 16,
	minecraft_entity_damaged = 17,
	minecraft_entity_killed = 18,
	minecraft_entity_place = 19,
	minecraft_equip = 20,
	minecraft_explode = 21,
	minecraft_fishing_rod_cast = 22,
	minecraft_fishing_rod_reel_in = 23,
	minecraft_flap = 24,
	minecraft_fluid_pickup = 25,
	minecraft_fluid_place = 26,
	minecraft_hit_ground = 27,
	minecraft_mob_interact = 28,
	minecraft_lightning_strike = 29,
	minecraft_minecart_moving = 30,
	minecraft_piston_contract = 31,
	minecraft_piston_extend = 32,
	minecraft_prime_fuse = 33,
	minecraft_projectile_land = 34,
	minecraft_projectile_shoot = 35,
	minecraft_ravager_roar = 36,
	minecraft_ring_bell = 37,
	minecraft_shear = 38,
	minecraft_shulker_close = 39,
	minecraft_shulker_open = 40,
	minecraft_splash = 41,
	minecraft_step = 42,
	minecraft_swim = 43,
	minecraft_wolf_shaking = 44
};