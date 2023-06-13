#pragma once

#include "../../block.h"

namespace Blocks
{
	class FacingProperty : virtual public BlockState
	{
	public:
		enum class Facing : int
		{
			north,
			east,
			south,
			west,
			up,
			down
		};
		enum class Facing2D : int
		{
			north,
			south,
			west,
			east
		};
		enum class FacingHopper : int
		{
			down,
			north,
			south,
			west,
			east
		};

		static Facing facingFromOrientatino(float playerYaw, float playerPitch)
		{
			if (playerPitch > 45)
				return Facing::down;
			if (playerPitch < -45)
				return Facing::up;

			int yawInt = (int)playerYaw;
			playerYaw -= (yawInt - yawInt % 360);
			if (playerYaw < 0)
				playerYaw += 360;

			if (playerYaw <= 45 || playerYaw > 315)
				return Facing::south;
			else if (playerYaw <= 135)
				return Facing::west;
			else if (playerYaw <= 225)
				return Facing::north;
			return Facing::east;
		}
		static Facing2D facing2DfromOrientation(float playerYaw, float playerPitch)
		{
			int yawInt = (int)playerYaw;
			playerYaw -= (yawInt - yawInt % 360);
			if (playerYaw < 0)
				playerYaw += 360;

			if (playerYaw <= 45 || playerYaw > 315)
				return Facing2D::south;
			else if (playerYaw <= 135)
				return Facing2D::west;
			else if (playerYaw <= 225)
				return Facing2D::north;
			return Facing2D::east;
		}

		FacingProperty(Facing facing) : value((int)facing) {}
		FacingProperty(Facing2D facing) : value((int)facing) {}
		FacingProperty(FacingHopper facing) : value((int)facing) {}
		virtual const Blocks::FacingProperty* const facing() const override { return nullptr; }
		int get() const { return value; }

	protected:
		int value;
	};
}