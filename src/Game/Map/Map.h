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

struct FNavNode {
    int Row, Col;
    std::vector<int> Neighbors;
};

struct FRoom {
    sf::Vector2f Origin;
    sf::Vector2f Size;

    sf::Vector2f TopLeft() { return Origin; }
    sf::Vector2f TopRight() { return Origin + sf::Vector2f(Size.x, 0); }
    sf::Vector2f BottomLeft() { return Origin + sf::Vector2f(0, Size.y); }
    sf::Vector2f BottomRight() { return Origin + Size; }
    sf::Vector2f Center() { return Origin + Size / 2.f; }

    //To know in which room the player is currently in.
    bool Contains(sf::Vector2f Pos) const {
        return Pos.x >= Origin.x && Pos.x <= Origin.x + Size.x &&
            Pos.y >= Origin.y && Pos.y <= Origin.y + Size.y;
    }
};

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
    sf::Vector2f WorldToGrid(sf::Vector2f worldPos);
    sf::Vector2f GridToWorld(sf::Vector2f gridPos);

    std::vector<FNavNode> NavGraph;

    int GetNavIndex(int row, int col) const { return row * Width + col; }

    void AddRoom(FRoom Room) { Rooms.push_back(Room); }
    const std::vector<FRoom>& GetRooms() const { return Rooms; }

private:
    static const int PixelsPerTile = 32;

    int Width;
    int Height;

    std::vector<std::vector<FTile>> Map;
    std::unordered_map<std::string, sf::Texture> Textures;
    std::unordered_map<std::string, std::unique_ptr<sf::Sprite>> Sprites;

    int GetRand(int max, int chance) const;

    void BuildNavGraph();

    std::vector<FRoom> Rooms;
};
