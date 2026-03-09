//
// Created by Antoine on 2026-03-07.
//

#pragma once
#include <iostream>
#include <string>
#include "../../Core/Object.h"
#include "../Map/Tile.h"
#include "unordered_map"
#include <vector>
#include <memory>
#include <filesystem>
#include <fstream>
#include <sstream>
#include "SFML/Graphics.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"


class GMap : public GObject {
public:
    GMap(int width, int height);
    ~GMap() override = default;

    void LoadFolder(const std::string folderPath);
    FTile CreateTile(const std::string& rawValue);
    void LoadMap(const std::string mapPath);

    void Display(sf::RenderWindow& window);

    int GetWidth() { return Width; };
    int GetHeight() { return Height; };

    bool IsWalkable(int row, int col);
    bool BlocksVision(int row, int col);
    int GetMovementCost(int row, int col);
    std::vector<FTile> GetNeighbours(int row, int col);
    sf::Vector2f WorldToGrid(sf::Vector2f worldPos);
    sf::Vector2f GridToWorld(sf::Vector2f gridPos);

private:
    static const int PixelsPerTile = 32;

    int Width;
    int Height;

    std::vector<std::vector<FTile>> Map;
    std::unordered_map<std::string, sf::Texture> Textures;
    std::unordered_map<std::string, std::unique_ptr<sf::Sprite>> Sprites;

    std::vector<std::vector<FTile>> ObjectMap;

    int GetRand(int max, int chance) const;
};
