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

struct FTile {
	ETileType Type;
	std::string TextureID;
	std::string ObjectID;
	bool Walkable = true;
	bool BlocksVision = false;
	int MovementCost = 1;
	bool bIsExit = false;

	bool CanExit(SwitchStatus Status) {
		return bIsExit && Status == SwitchStatus::On;
	}
};
