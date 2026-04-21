//
// Created by Antoine on 2026-03-07.
//

#pragma once
#include <string>
#include <Game/Entities/Switch.h>

enum class ETileType {
	Floor,
	Wall,
	Dirt,
	Obstacle
};

enum class EWallTileType {
	IE_UL, IE_UR, IE_BL, IE_BR,
	IE_L, IE_R, IE_UTop, IE_UBottom, IE_B,
	IE_TopBottom, IE_LeftRight,
	EE_UL, EE_UR, EE_BL, EE_BR,
	EC_UL, EC_UR, EC_BL, EC_BR,
	IC_UL, IC_UR, IC_BL, IC_BR
};

struct FTile {
	ETileType Type;
	std::string TextureID;
	std::string ObjectID;
	bool Walkable = true;
	bool BlocksVision = false;
	int MovementCost = 1;
	bool bIsExit = false;
	bool bIsRoomInterior = false;

	bool CanExit(SwitchStatus Status) {
		return bIsExit && Status == SwitchStatus::On;
	}
};
