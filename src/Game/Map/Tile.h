//
// Created by Antoine on 2026-03-07.
//

#pragma once
#include <string>

enum class ETileType {
    Floor,
    Wall,
    Dirt,
    Obstacle,
};

struct FTile {
    ETileType Type;
    std::string TextureID;
    bool Walkable = true;
    bool BlocksVision = false;
    int MovementCost = 1;
};
