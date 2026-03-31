//
// Created by Antoine on 2026-03-07.
//

#include "Map.h"

GMap::GMap(int width, int height)
{
	Width = width / PixelsPerTile;
	Height = height / PixelsPerTile;

	// Load Tileset
	LoadFolder("Assets/Images/Tileset/Dirt");
	LoadFolder("Assets/Images/Tileset/Stone");
	LoadFolder("Assets/Images/Tileset/Clay");
	LoadFolder("Assets/Images/Tileset/Mix");
	LoadFolder("Assets/Images/Tileset/Floor");
	LoadFolder("Assets/Images/Objects");
}

void GMap::LoadFolder(const std::string folderPath)
{
	for (const auto& entry : std::filesystem::directory_iterator(folderPath))
	{
		if (entry.path().extension() == ".png")
		{
			std::string tileName = entry.path().stem().string();

			if (!Textures[tileName].loadFromFile(entry.path().string()))
				std::cerr << "Error loading : " << entry.path() << std::endl;
		}
	}
}

FTile GMap::CreateTile(const std::string& rawValue, int row, int col)
{
	FTile tile{};

	tile.TextureID = rawValue;

	size_t sep = rawValue.find('|');
	std::string base = (sep != std::string::npos) ? rawValue.substr(0, sep) : rawValue;
	tile.ObjectID = (sep != std::string::npos) ? rawValue.substr(sep + 1) : "";

	tile.TextureID = base;

	if (rawValue.find("row-10-column-8") == 0) {
		bHasExit = true;
		ExitTile = sf::Vector2f(row, col);

		tile.Type = ETileType::Floor;
		tile.Walkable = true;
		tile.BlocksVision = false;
		tile.MovementCost = 1;

		return tile;
	}

	if (rawValue == "Dirt")
	{
		tile.TextureID += std::to_string(GetRand(22, 85));
		tile.Type = ETileType::Dirt;
		tile.Walkable = false;
		tile.BlocksVision = false;
		tile.MovementCost = 10;
	}
	else if (rawValue.find("F_") == 0)
	{
		if (rawValue.find("F_M") == 0)
			tile.TextureID += std::to_string(GetRand(8, 65));
		if (rawValue.find("F_J") == 0)
			tile.TextureID += std::to_string(GetRand(5, 10));

		tile.Type = ETileType::Floor;
		tile.Walkable = true;
		tile.BlocksVision = false;
		tile.MovementCost = 1;
	}
	else
	{
		tile.Type = ETileType::Wall;
		tile.Walkable = false;
		tile.BlocksVision = true;
		tile.MovementCost = 1;
	}

	if (!tile.ObjectID.empty())
		tile.Type = ETileType::Obstacle;

	return tile;
}

void GMap::LoadMap(const std::string mapPath)
{
	Map.assign(Height, std::vector<FTile>(Width));

	std::ifstream file(mapPath);
	if (!file)
		throw std::runtime_error("Can't open file");

	std::string line;
	for (int row = 0; row < Height && std::getline(file, line); row++)
	{
		if (!line.empty() && line.back() == '\r')
			line.pop_back();

		std::stringstream ss(line);
		std::string value;

		for (int col = 0; col < Width && std::getline(ss, value, ','); col++)
		{
			if (!value.empty() && value.back() == '\r')
				value.pop_back();

			Map[row][col] = CreateTile(value, row, col);
		}
	}

	for (auto& [tile, texture] : Textures)
	{
		Sprites[tile] = std::make_unique<sf::Sprite>(texture);

		sf::Vector2u textureSize = texture.getSize();
		float scaleX = static_cast<float>(PixelsPerTile) / textureSize.x;
		float scaleY = static_cast<float>(PixelsPerTile) / textureSize.y;
		Sprites[tile]->setScale(sf::Vector2f(scaleX, scaleY));
	}

	DetectRooms();
	MergeRooms();
	FindBreakRoom();
	BuildNavGraph();
}

void GMap::Display(sf::RenderWindow& window)
{
	for (int row = 0; row < Height; row++)
	{
		for (int col = 0; col < Width; col++)
		{
			sf::Vector2f position(
				static_cast<float>(col * PixelsPerTile),
				static_cast<float>(row * PixelsPerTile)
			);

			//Add a sand layer behind to cover wall gaps.
			auto floor = Sprites.find("F_F");
			if (floor != Sprites.end())
			{
				floor->second->setPosition(position);
				window.draw(*floor->second);
			}

			const std::string& tileName = Map[row][col].TextureID;

			auto it = Sprites.find(tileName);

			if (it == Sprites.end())
				continue;

			it->second->setPosition(position);

			window.draw(*it->second);

			const std::string& objectName = Map[row][col].ObjectID;

			if (!objectName.empty())
			{
				auto obj = Sprites.find(objectName);
				if (obj != Sprites.end())
				{
					obj->second->setPosition(position);
					window.draw(*obj->second);
				}
			}
		}
	}
}

bool GMap::IsWalkable(int row, int col)
{
	if (row < 0 || row >= Height || col < 0 || col >= Width)
		return false;

	return Map[row][col].Walkable && Map[row][col].Type != ETileType::Obstacle;
}

bool GMap::BlocksVision(int row, int col)
{
	return Map[row][col].BlocksVision;
}

int GMap::GetMovementCost(int row, int col)
{
	return Map[row][col].MovementCost;
}

sf::Vector2f GMap::WorldToGrid(sf::Vector2f worldPos)
{
	return sf::Vector2f(
		std::floor(worldPos.x / PixelsPerTile), // col
		std::floor(worldPos.y / PixelsPerTile) // row
	);
}

sf::Vector2f GMap::GridToWorld(sf::Vector2f gridPos)
{
	return sf::Vector2f(
		gridPos.x * PixelsPerTile + PixelsPerTile / 2.f,
		gridPos.y * PixelsPerTile + PixelsPerTile / 2.f
	);
}

void GMap::DetectRooms()
{
	for (int r = 0; r < Height; r++)
		for (int c = 0; c < Width; c++) {
			if (Map[r][c].TextureID != "F_UL")
				continue;

			int w = 1;

			while (c + w < Width && Map[r][c + w - 1].TextureID != "F_UR" && Map[r][c + w - 1].TextureID != "F_R")
				w++;

			int h = 1;

			while (r + h < Height && Map[r + h - 1][c].TextureID != "F_BL" && Map[r + h - 1][c].TextureID != "F_B")
				h++;

			FRoom Room;
			Room.Origin = sf::Vector2f(c * PixelsPerTile, r * PixelsPerTile);
			Room.Size = sf::Vector2f(w * PixelsPerTile, h * PixelsPerTile);

			Room.Corners.push_back(Room.Origin);
			Room.Corners.push_back(Room.Origin + sf::Vector2f(Room.Size.x, 0));
			Room.Corners.push_back(Room.Origin + Room.Size);
			Room.Corners.push_back(Room.Origin + sf::Vector2f(0, Room.Size.y));
			Room.SubRooms.push_back(Room);

			Rooms.push_back(Room);
		}
}

void GMap::MergeRooms()
{
	bool merged = true;

	while (merged) {
		merged = false;

		for (int i = 0; i < Rooms.size(); i++) {
			for (int j = i + 1; j < Rooms.size(); j++)
				if (Intersects(Rooms[i], Rooms[j])) {
					Rooms[i] = Merge(Rooms[i], Rooms[j]);

					sf::Vector2f Center = Rooms[i].Center();

					std::sort(Rooms[i].Corners.begin(), Rooms[i].Corners.end(),
						[&Center](const sf::Vector2f& a, const sf::Vector2f& b) {
							return std::atan2(a.y - Center.y, a.x - Center.x) <
								   std::atan2(b.y - Center.y, b.x - Center.x);
						});

					Rooms.erase(Rooms.begin() + j);
					merged = true;
					break;
				}

			if (merged)
				break;
		}
	}
}

void GMap::FindBreakRoom()
{
	if (Rooms.empty())
		return;

	float maxArea = 0.f;
	int bestIndex = -1;

	for (int i = 0; i < Rooms.size(); i++) {
		float area = Rooms[i].Size.x * Rooms[i].Size.y;

		if (area > maxArea) {
			maxArea = area;
			bestIndex = i;
		}
	}

	if (bestIndex != -1)
		Rooms[bestIndex].bIsBreakRoom = true;
}

FRoom GMap::Merge(const FRoom& RoomA, const FRoom& RoomB)
{
	sf::Vector2f MinCorner(
		std::min(RoomA.Origin.x, RoomB.Origin.x),
		std::min(RoomA.Origin.y, RoomB.Origin.y)
	);

	sf::Vector2f MaxCorner(
		std::max(RoomA.Origin.x + RoomA.Size.x, RoomB.Origin.x + RoomB.Size.x),
		std::max(RoomA.Origin.y + RoomA.Size.y, RoomB.Origin.y + RoomB.Size.y)
	);

	FRoom MergedRoom;
	MergedRoom.Origin = MinCorner;
	MergedRoom.Size = MaxCorner - MinCorner;

	MergedRoom.SubRooms = RoomA.SubRooms;
	for (const FRoom& SubRoom : RoomB.SubRooms)
		MergedRoom.SubRooms.push_back(SubRoom);

	MergedRoom.Corners = RoomA.Corners;
	for (const sf::Vector2f& Corner : RoomB.Corners)
		MergedRoom.Corners.push_back(Corner);

	return MergedRoom;
}

bool GMap::Intersects(const FRoom& RoomA, const FRoom& RoomB)
{
	return RoomA.Origin.x < RoomB.Origin.x + RoomB.Size.x &&
		RoomA.Origin.x + RoomA.Size.x > RoomB.Origin.x &&
		RoomA.Origin.y < RoomB.Origin.y + RoomB.Size.y &&
		RoomA.Origin.y + RoomA.Size.y > RoomB.Origin.y;
}

sf::Vector2f GMap::GetRandomPosition()
{
	if (Rooms.empty())
		return sf::Vector2f(0, 0);

	int roomIndex = rand() % Rooms.size();

	const FRoom& Room = Rooms[roomIndex];

	for (int attempt = 0; attempt < 100; attempt++) {
		int col = Room.Origin.x / PixelsPerTile + rand() % (int)(Room.Size.x / PixelsPerTile);
		int row = Room.Origin.y / PixelsPerTile + rand() % (int)(Room.Size.y / PixelsPerTile);

		if (IsWalkable(row, col) && Room.Contains(GridToWorld(sf::Vector2f(col, row))) && Map[row][col].TextureID == "F_M") {
			Map[row][col].Walkable = false;
			Map[row][col].Type = ETileType::Obstacle;
			BuildNavGraph();
			return GridToWorld(sf::Vector2f(col, row));
		}
	}

	return Room.Center();
}

void GMap::ChangeExitVisibility(bool bIsOn)
{
	int row = ExitTile.x;
	int col = ExitTile.y;

	FTile& Tile = Map[row][col];

	if (bIsOn) {
		Tile.ObjectID = "row-10-column-9";
		bHasExit = true;
	}
	else {
		Tile.ObjectID = "row-10-column-8";
		bHasExit = false;
	}

	BuildNavGraph();
}

int GMap::GetRand(int max, int chance) const
{
	if (rand() % 100 <= chance)
		return 0;

	return rand() % max + 1;
}

void GMap::BuildNavGraph()
{
	NavGraph.resize(Height * Width);

	const int neighborRowOffsets[] = { -1, 1, 0, 0 };
	const int neighborColOffsets[] = { 0, 0, -1, 1 };

	for (int r = 0; r < Height; r++)
		for (int c = 0; c < Width; c++) {
			NavGraph[GetNavIndex(r, c)].Row = r;
			NavGraph[GetNavIndex(r, c)].Col = c;

			if (!IsWalkable(r, c))
				continue;

			for (int i = 0; i < 4; i++) {
				int neighborRow = r + neighborRowOffsets[i];
				int neighborCol = c + neighborColOffsets[i];

				if (IsWalkable(neighborRow, neighborCol))
					NavGraph[GetNavIndex(r, c)].Neighbors.push_back(GetNavIndex(neighborRow, neighborCol));

			}
		}
}
