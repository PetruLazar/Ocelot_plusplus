#pragma once
#include "typedefs.h"

namespace ChangeGameState
{
	enum Reason : Byte
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
	enum position : Byte
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

namespace playerDigging
{
	enum status
	{
		startedDigging,
		cancelledDigging,
		finishedDigging,
		dropItemStack,
		dropItem,
		shootArrow,
		//finishEating = shootArrow,
		swapItemInHand
	};
}

enum class gamemode : Byte
{
	survival,
	creative,
	adventure,
	spectator,

	//the 'previous gamemode' default value (-1)
	none = 0xff
};