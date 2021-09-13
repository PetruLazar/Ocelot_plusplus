#pragma once
#include "typedefs.h"

namespace ChangeGameState
{
	enum Reason : byte
	{
		no_respawn_block_available,
		end_raining,
		begin_raining,
		changeGamemode,
		winGame,
		demoEvent,
		arrowHitPlayer,
		rainLevelChange,
		thunderLevelChange,
		playPufferfishStingSound,
		playElderGuadrianMobAppearance,
		respawnScreen
	};
	enum class winGame
	{
		justSpawn,
		showCredits
	};
	enum class demoEvent
	{
		showWelcome,
		movementControls = 101,
		jumpControl,
		inventoryControl,
		demoOver
	};
	enum class rainLevelChange
	{
		noRain,
		lightRain,
		mediumRain,
		heavyRain,
		superHeavyRain,
		darkRain,
		darkYellowRain,
		yellowRain,
		brightYellowRain,
		ultraBrightYellowRain
	};
	enum class respawnScreen
	{
		enable,
		disable
	};
}

namespace ChatMessage
{
	enum position : byte
	{
		normalMessage,
		systemMessage,
		gameInfo
	};
}

namespace playerInfo
{
	enum action
	{
		addPlayer,
		updateGamemode,
		updateLatency,
		updateDisplayName,
		removePlayer
	};
}

enum class gamemode : byte
{
	survival,
	creative,
	adventure,
	spectator,

	//the 'previous gamemode' default value (-1)
	none = 0xff
};