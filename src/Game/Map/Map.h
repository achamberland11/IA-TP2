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
#include <set>

enum class EWallType {
	Stone, 
	Clay
};

enum class ECorridorTileType {
	Horizontal, Vertical, Corner
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
	std::set<std::pair<int, int>> UsedConnectionPoints;
};

struct FRoom {
	int TileSize = 32;

	sf::Vector2f Origin;
	sf::Vector2f Size;
	std::vector<sf::Vector2f> Corners;
	std::vector<FRoom> SubRooms;

	bool bIsBreakRoom = false;

	sf::Vector2f Center() const { return Origin + Size / 2.f; }
	sf::Vector2f CenterTile() const { 
		sf::Vector2f C = Center();
		
		return sf::Vector2f(
			std::floor(C.x / TileSize) * TileSize + TileSize / 2.f,
			std::floor(C.y / TileSize) * TileSize + TileSize / 2.f
		);
	}

	sf::Vector2f GetCenterOfPos(sf::Vector2f Pos) const {
		return sf::Vector2f(
			std::floor(Pos.x / TileSize) * TileSize + TileSize / 2.f,
			std::floor(Pos.y / TileSize) * TileSize + TileSize / 2.f
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

struct FCorridor {
	std::vector<std::pair<ECorridorTileType, sf::Vector2f>> Tiles;
	std::optional<sf::Vector2f> Start;
	std::optional<sf::Vector2f> End;
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
	const FTile GetTile(int row, int col) const { return Map[row][col]; }

	sf::Vector2f GetRandomPosition();

	bool HasExit() { return bHasExit; }
	sf::Vector2f GetExitTilePos() { return ExitTilePos; }
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
		{EWallTileType::IE_UL, "row-4-column-6"},
		{EWallTileType::IE_UTop, "row-9-column-3|row-9-column-4|row-9-column-5|row-9-column-6|row-9-column-7|row-9-column-8"},
		{EWallTileType::IE_UBottom, "row-10-column-1|row-10-column-2|row-10-column-3|row-10-column-4|row-10-column-5|row-10-column-6|row-10-column-7|row-10-column-10|row-10-column-11"},
		{EWallTileType::IE_UR, "row-4-column-7"},
		{EWallTileType::IE_L, "row-4-column-3|row-8-column-10|row-8-column-12|row-9-column-10|row-9-column-12"},
		{EWallTileType::IE_R, "row-4-column-1|row-8-column-9|row-8-column-11|row-9-column-9|row-9-column-11"},
		{EWallTileType::IE_B, "row-3-column-2|row-8-column-3|row-8-column-4|row-8-column-5|row-8-column-6|row-8-column-7|row-8-column-8"},
		{EWallTileType::IE_BL, "row-5-column-6"},
		{EWallTileType::IE_BR, "row-5-column-7"},
		{EWallTileType::EE_UL, "row-3-column-1"},
		{EWallTileType::EE_UR, "row-3-column-3"},
		{EWallTileType::EE_BL, "row-9-column-1"},
		{EWallTileType::EE_BR, "row-9-column-2"},
		{EWallTileType::EC_BR, "row-4-column-6"},
		{EWallTileType::EC_BL, "row-4-column-7"},
		{EWallTileType::EC_UR, "row-5-column-6"},
		{EWallTileType::EC_UL, "row-5-column-7"},
		{EWallTileType::IE_TopBottom, "row-6-column-2"},
		{EWallTileType::IE_LeftRight, "row-4-column-4"},
		{EWallTileType::IC_UL, "row-8-column-1"},
		{EWallTileType::IC_UR, "row-8-column-2"},
		{EWallTileType::IC_BL, "row-9-column-1"},
		{EWallTileType::IC_BR, "row-9-column-2"}
	};

	std::map<EWallTileType, std::string> ClayWallTileNames = {
		{EWallTileType::IE_UL, "row-14-column-6"},
		{EWallTileType::IE_UTop, "row-19-column-3|row-19-column-4|row-19-column-5"},
		{EWallTileType::IE_UBottom, "row-20-column-1|row-20-column-2|row-20-column-3|row-20-column-4|row-20-column-5"},
		{EWallTileType::IE_UR, "row-14-column-7"},
		{EWallTileType::IE_L, "row-14-column-3|row-18-column-10|row-18-column-12|row-19-column-12"},
		{EWallTileType::IE_R, "row-14-column-1|row-18-column-9|row-18-column-11|row-19-column-11"},
		{EWallTileType::IE_B, "row-18-column-6|row-18-column-7|row-18-column-8"},
		{EWallTileType::IE_BL, "row-15-column-6"},
		{EWallTileType::IE_BR, "row-15-column-7"}
	};

	std::vector<FRoom> Rooms;
	FRoom* BreakRoom = nullptr;
	std::vector<FCorridor> Corridors;

	sf::Vector2f ExitTilePos;
	bool bHasExit = false;

	//Procedural Generation
	void CarveRoom(int row, int col, int width, int height);
	void ConnectRoomsBacktrack(std::vector<FGenRoom>& Rooms, int current, std::vector<bool>& visited);
	bool CanCarveCorridor(int r1, int c1, int r2, int c2);
	void CarveCorridor(int r1, int c1, int r2, int c2);
	sf::Vector2f GetEdgePoint(FGenRoom& A, const FGenRoom& B);
	void PlaceRoomBorders(int row, int col, int width, int height);
	void PlaceCorridorBorders(const FCorridor& Corridor);
	std::string GetRandomizedImage(std::string imagesName);
	void PlaceCorridorJunctionImages(int row, int col);
	bool IsTooCloseToCorridor(int r, int c, bool bEndPoint);
	void PlaceExit();
	void PlaceObstacles();

	sf::Font DebugFont;
};
