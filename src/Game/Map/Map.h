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

enum class EWallType {
	Stone, 
	Clay
};

struct FNavNode {
	int row, col;
	std::vector<int> Neighbors;
};

struct FGenRoom {
	int row, col;
	int width, height;
	EWallType WallType;
	bool bVisited = false;
	std::vector<int> ConnectedTo;
};

struct FRoom {
	sf::Vector2f Origin;
	sf::Vector2f Size;
	std::vector<sf::Vector2f> Corners;
	std::vector<FRoom> SubRooms;

	bool bIsBreakRoom = false;

	sf::Vector2f Center() const { return Origin + Size / 2.f; }
	sf::Vector2f CenterTile() const { 
		sf::Vector2f C = Center();

		int TileSize = 32;

		return sf::Vector2f(
			std::floor(C.x / TileSize) * TileSize + TileSize / 2.f,
			std::floor(C.y / TileSize) * TileSize + TileSize / 2.f
		);
	}

	//To know in which room the player is currently in.
	bool Contains(sf::Vector2f Pos) const {
		if (SubRooms.empty())
			return Pos.x >= Origin.x && Pos.x <= Origin.x + Size.x &&
			Pos.y >= Origin.y && Pos.y <= Origin.y + Size.y;
		for (const FRoom& SubRoom : SubRooms)
			if (SubRoom.Contains(Pos))
				return true;

		return false;
	}
};

class GMap : public GObject {
public:
	GMap(int width, int height);
	~GMap() override = default;

	void LoadFolder(const std::string folderPath);
	FTile CreateTile(const std::string& rawValue, int row, int col);
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

	void DetectRooms();
	void MergeRooms();
	void FindBreakRoom();
	FRoom* GetBreakRoom() const { return BreakRoom; }
	FRoom Merge(const FRoom& RoomA, const FRoom& RoomB);
	bool Intersects(const FRoom& RoomA, const FRoom& RoomB);
	const std::vector<FRoom>& GetRooms() const { return Rooms; }

	sf::Vector2f GetRandomPosition();

	bool HasExit() { return bHasExit; }
	sf::Vector2f GetExitTilePos() { return ExitTile; }
	void ChangeExitVisibility(bool bIsOn);

	void SetTileToObstacle(int row, int col) {
		Map[row][col].Walkable = false;
		Map[row][col].Type = ETileType::Obstacle;
		BuildNavGraph();
	}

	//Procedural Generation
	void GenerateRooms();
	sf::Vector2f GetPlayerSpawnPos() { return Rooms[0].Corners[0] + sf::Vector2f(PixelsPerTile / 2.f, PixelsPerTile / 2.f); }

	static const int PixelsPerTile = 32;

private:
	int Width;
	int Height;

	std::vector<std::vector<FTile>> Map;
	std::unordered_map<std::string, sf::Texture> Textures;
	std::unordered_map<std::string, std::unique_ptr<sf::Sprite>> Sprites;

	int GetRand(int max, int chance) const;

	void BuildNavGraph();

	std::map<EWallTileType, std::string> StoneWallTileNames = {
		{EWallTileType::E_UL, "row-4-column-6"},
		{EWallTileType::E_UTop, "row-9-column-4"},
		{EWallTileType::E_UBottom, "row-10-column-4"},
		{EWallTileType::E_UR, "row-4-column-7"},
		{EWallTileType::E_L, "row-4-column-3"},
		{EWallTileType::E_R, "row-4-column-1"},
		{EWallTileType::E_B, "row-3-column-2"},
		{EWallTileType::E_BL, "row-5-column-6"},
		{EWallTileType::E_BR, "row-5-column-7"}
	};

	std::map<EWallTileType, std::string> ClayWallTileNames = {
		{EWallTileType::E_UL, "row-14-column-6"},
		{EWallTileType::E_UTop, "row-19-column-3"},
		{EWallTileType::E_UBottom, "row-20-column-2"},
		{EWallTileType::E_UR, "row-14-column-7"},
		{EWallTileType::E_L, "row-14-column-3"},
		{EWallTileType::E_R, "row-14-column-1"},
		{EWallTileType::E_B, "row-18-column-7"},
		{EWallTileType::E_BL, "row-15-column-6"},
		{EWallTileType::E_BR, "row-15-column-7"}
	};

	std::vector<FRoom> Rooms;
	FRoom* BreakRoom = nullptr;

	sf::Vector2f ExitTile;
	bool bHasExit = false;

	//Procedural Generation
	void CarveRoom(int row, int col, int width, int height);
	void CarveCorridor(int r1, int c1, int r2, int c2);
	void PlaceRoomBorders(int row, int col, int width, int height);
};
