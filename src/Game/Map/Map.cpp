//
// Created by Antoine on 2026-03-07.
//

#include "Map.h"
#include <stack>

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
	DebugFont.openFromFile("Assets/Fonts/Roboto/Roboto.ttf");
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

	//Exit
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
	/*Map.assign(Height, std::vector<FTile>(Width));

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
	}*/

	GenerateRooms();
	DetectRooms();
	MergeRooms();
	FindBreakRoom();
	PlaceExit();
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
	{
		Rooms[bestIndex].bIsBreakRoom = true;
		BreakRoom = &Rooms[bestIndex];
	}
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

	return Room.CenterTile();
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

void GMap::GenerateRooms()
{
	Map.assign(Height, std::vector<FTile>(Width));

	for (int row = 0; row < Height; row++)
		for (int col = 0; col < Width; col++)
			Map[row][col] = CreateTile("Dirt", row, col);

	std::vector<FGenRoom> GenRooms;

	int attempts = 0;

	while (GenRooms.size() < 15 && attempts < 200) {
		int w = 4 + rand() % 5;
		int h = 4 + rand() % 5;

		int rowRange = Height - h - 4;
		int colRange = Width - w - 3;

		if (rowRange <= 0 || colRange <= 0) {
			attempts++;
			continue;
		}

		int r = 3 + rand() % rowRange;
		int c = 2 + rand() % colRange;

		bool overlaps = false;

		for (auto& R : GenRooms) {
			if (c < R.col + R.width + 3 && c + w + 3 > R.col &&
				r < R.row + R.height + 3 && r + h + 3 > R.row) {
				overlaps = true;
				break;
			}

			if (GetRand(1, 75) == 0)
				R.WallType = EWallType::Stone;
			else
				R.WallType = EWallType::Clay;
		}

		if (!overlaps)
			GenRooms.push_back({ r, c, w, h });

		attempts++;
	}

	for (auto& R : GenRooms) {
		CarveRoom(R.row, R.col, R.width, R.height);
		PlaceRoomBorders(R.row, R.col, R.width, R.height);
	}

	std::vector<bool> visited(GenRooms.size(), false);
	ConnectRoomsBacktrack(GenRooms, 0, visited);

	for (int i = 0; i < (int)GenRooms.size(); i++) {
		if (visited[i])
			continue;

		float bestDist = FLT_MAX;
		int bestNeighbor = 0;

		for (int j = 0; j < (int)GenRooms.size(); j++) {
			if (!visited[j])
				continue;

			float dr = GenRooms[i].row - GenRooms[j].row;
			float dc = GenRooms[i].col - GenRooms[j].col;
			float dist = dr * dr + dc * dc;

			if (dist < bestDist) {
				bestDist = dist;
				bestNeighbor = j;
			}
		}

		auto p1 = GetEdgePoint(GenRooms[bestNeighbor], GenRooms[i]);
		auto p2 = GetEdgePoint(GenRooms[i], GenRooms[bestNeighbor]);

		CarveCorridor(p1.x, p1.y, p2.x, p2.y);
		visited[i] = true;
	}

	for (auto& C : Corridors) {
		PlaceCorridorBorders(C);

		for (auto Tile : C.Tiles)
			Map[Tile.second.x][Tile.second.y] = CreateTile("F_J", Tile.second.x, Tile.second.y);
	}

	for (auto& [tile, texture] : Textures)
	{
		Sprites[tile] = std::make_unique<sf::Sprite>(texture);

		sf::Vector2u textureSize = texture.getSize();
		float scaleX = static_cast<float>(PixelsPerTile) / textureSize.x;
		float scaleY = static_cast<float>(PixelsPerTile) / textureSize.y;
		Sprites[tile]->setScale(sf::Vector2f(scaleX, scaleY));
	}
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
			NavGraph[GetNavIndex(r, c)].row = r;
			NavGraph[GetNavIndex(r, c)].col = c;

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

void GMap::CarveRoom(int row, int col, int width, int height)
{
	std::string tileName;

	for (int r = row; r < row + height; r++)
		for (int c = col; c < col + width; c++) {
			if (r == row) {
				if (c == col)
					tileName = "F_UL";
				else if (c == col + width - 1)
					tileName = "F_UR";
				else
					tileName = "F_U";
			}
			else if (r == row + height - 1) {
				if (c == col)
					tileName = "F_BL";
				else if (c == col + width - 1)
					tileName = "F_BR";
				else
					tileName = "F_B";
			}
			else if (r > row && r < row + height - 1) {
				if (c == col)
					tileName = "F_L";
				else if (c == col + width - 1)
					tileName = "F_R";
				else
					tileName = "F_M";
			}

			Map[r][c] = CreateTile(tileName, r, c);
			Map[r][c].bIsRoomInterior = true;
		}
}

void GMap::ConnectRoomsBacktrack(std::vector<FGenRoom>& Rooms, int current, std::vector<bool>& visited)
{
	visited[current] = true;

	std::vector<std::pair<float, int>> candidates;

	for (int i = 0; i < (int)Rooms.size(); i++) {
		if (visited[i])
			continue;

		float dr = Rooms[current].row - Rooms[i].row;
		float dc = Rooms[current].col - Rooms[i].col;

		candidates.push_back({ dr * dr + dc * dc, i });
	}

	std::sort(candidates.begin(), candidates.end());

	int shuffleCount = std::min((int)candidates.size(), 3);

	for (int i = 0; i < shuffleCount; i++) {
		int j = i + rand() % (shuffleCount - i);
		std::swap(candidates[i], candidates[j]);
	}

	for (auto& [dist, next] : candidates) {
		if (visited[next])
			continue;

		auto savedUsedA = Rooms[current].UsedConnectionPoints;
		auto savedUsedB = Rooms[next].UsedConnectionPoints;

		auto p1 = GetEdgePoint(Rooms[current], Rooms[next]);
		auto p2 = GetEdgePoint(Rooms[next], Rooms[current]);

		int r1 = p1.x, c1 = p1.y;
		int r2 = p2.x, c2 = p2.y;

		auto isCorner = [&](int r, int c, const FGenRoom& Room) {
			auto topRow = r == Room.row;
			auto bottomRow = r == Room.row + Room.height - 1;
			auto leftCol = c == Room.col;
			auto rightCol = c == Room.col + Room.width - 1;

			return (topRow || bottomRow) && (leftCol || rightCol);
			};

		if (isCorner(r1, c1, Rooms[current]) || isCorner(r2, c2, Rooms[next])) {
			Rooms[current].UsedConnectionPoints = savedUsedA;
			Rooms[next].UsedConnectionPoints = savedUsedB;
			continue;
		}

		if (CanCarveCorridor(r1, c1, r2, c2)) {
			CarveCorridor(r1, c1, r2, c2);
			ConnectRoomsBacktrack(Rooms, next, visited);
		}
		else {
			Rooms[current].UsedConnectionPoints = savedUsedA;
			Rooms[next].UsedConnectionPoints = savedUsedB;
		}
	}
}

bool GMap::CanCarveCorridor(int r1, int c1, int r2, int c2)
{
	int r = r1;
	int c = c1;

	int colStep = (c2 > c1) ? 1 : -1;

	while (c != c2)
	{
		bool isEndpoint = (r == r1 && c == c1) || (r == r2 && c == c2);

		if (IsTooCloseToCorridor(r, c, isEndpoint))
			return false;

		c += colStep;
	}

	int rowStep = (r2 > r1) ? 1 : -1;

	while (r != r2)
	{
		bool isEndpoint = (r == r1 && c == c1) || (r == r2 && c == c2);

		if (IsTooCloseToCorridor(r, c2, isEndpoint))
			return false;

		r += rowStep;
	}

	return true;
}

void GMap::CarveCorridor(int r1, int c1, int r2, int c2)
{
	FCorridor Corridor;

	int colStep = (c2 > c1) ? 1 : -1;

	for (int c = c1; c != c2; c += colStep) {
		if (Map[r1][c].Type != ETileType::Floor) {
			if (Map[r1][c].bIsRoomInterior)
				continue;

			Map[r1][c] = CreateTile("F_J", r1, c);

			bool bNearFloor = Map[r1 - 2][c].TextureID.find("F_J") != std::string::npos ||
				Map[r1 - 1][c].TextureID.find("F_J") != std::string::npos ||
				Map[r1 + 1][c].TextureID.find("F_J") != std::string::npos;

			Corridor.Tiles.push_back({ bNearFloor ? ECorridorTileType::Corner : ECorridorTileType::Horizontal,
				sf::Vector2f(r1, c) });
		}
	}

	if (Map[r1][c2].Type != ETileType::Floor) {
		Map[r1][c2] = CreateTile("F_J", r1, c2);
		Corridor.Tiles.push_back({ ECorridorTileType::Corner, sf::Vector2f(r1, c2) });
	}

	int rowStep = (r2 > r1) ? 1 : -1;

	for (int r = r1; r != r2; r += rowStep) {
		if (r == r1)
			continue;

		if (Map[r][c2].bIsRoomInterior)
			continue;

		Map[r][c2] = CreateTile("F_J", r, c2);

		bool bNearFloor = Map[r][c2 - 1].TextureID.find("F_J") != std::string::npos ||
			Map[r][c2 + 1].TextureID.find("F_J") != std::string::npos;

		Corridor.Tiles.push_back({ bNearFloor ? ECorridorTileType::Corner : ECorridorTileType::Vertical,
			sf::Vector2f(r, c2) });
	}

	if (!Corridor.Tiles.empty()) {
		if (Corridor.Tiles.begin()->first != ECorridorTileType::Corner)
			Corridor.Start = Corridor.Tiles.begin()->second;

		if (std::prev(Corridor.Tiles.end())->first != ECorridorTileType::Corner)
			Corridor.End = std::prev(Corridor.Tiles.end())->second;

		Corridors.push_back(Corridor);
	}
}

sf::Vector2f GMap::GetEdgePoint(FGenRoom& A, const FGenRoom& B)
{
	int dr = (B.row + B.height / 2) - (A.row + A.height / 2);
	int dc = (B.col + B.width / 2) - (A.col + A.width / 2);

	std::vector<std::pair<int, int>> candidates;

	if (abs(dr) >= abs(dc))
	{
		int r = (dr > 0) ? A.row + A.height - 1 : A.row;

		for (int c = A.col + 1; c < A.col + A.width - 1; c++)
			if (A.UsedConnectionPoints.count({ r, c }) == 0)
				candidates.push_back({ r, c });
	}
	else
	{
		int c = (dc > 0) ? A.col + A.width - 1 : A.col;

		for (int r = A.row + 1; r < A.row + A.height - 1; r++)
			if (A.UsedConnectionPoints.count({ r, c }) == 0)
				candidates.push_back({ r, c });
	}

	if (candidates.empty())
		return sf::Vector2f(A.row + A.height / 2.f, A.col + A.width / 2.f);

	auto [r, c] = candidates[rand() % candidates.size()];
	A.UsedConnectionPoints.insert({ r, c });

	return sf::Vector2f(r, c);
}

void GMap::PlaceRoomBorders(int row, int col, int width, int height)
{
	for (int r = row; r < row + height; r++)
		for (int c = col; c < col + width; c++) {
			std::string tileName = Map[r][c].TextureID;

			if (tileName == "F_UL") {
				Map[r - 2][c - 1] = CreateTile(StoneWallTileNames[EWallTileType::IE_UL], r - 2, c - 1);
				Map[r - 2][c] = CreateTile(GetRandomizedImage(StoneWallTileNames[EWallTileType::IE_UTop]), r - 2, c);
				Map[r - 1][c] = CreateTile(GetRandomizedImage(StoneWallTileNames[EWallTileType::IE_UBottom]), r - 1, c);
				Map[r - 1][c - 1] = CreateTile(GetRandomizedImage(StoneWallTileNames[EWallTileType::IE_L]), r - 1, c - 1);
				Map[r][c - 1] = CreateTile(GetRandomizedImage(StoneWallTileNames[EWallTileType::IE_L]), r, c - 1);
			}
			else if (tileName == "F_UR") {
				Map[r - 2][c + 1] = CreateTile(StoneWallTileNames[EWallTileType::IE_UR], r - 2, c + 1);
				Map[r - 2][c] = CreateTile(GetRandomizedImage(StoneWallTileNames[EWallTileType::IE_UTop]), r - 2, c);
				Map[r - 1][c] = CreateTile(GetRandomizedImage(StoneWallTileNames[EWallTileType::IE_UBottom]), r - 1, c);
				Map[r - 1][c + 1] = CreateTile(GetRandomizedImage(StoneWallTileNames[EWallTileType::IE_R]), r - 1, c + 1);
				Map[r][c + 1] = CreateTile(GetRandomizedImage(StoneWallTileNames[EWallTileType::IE_R]), r, c + 1);
			}
			else if (tileName == "F_BL") {
				Map[r + 1][c - 1] = CreateTile(StoneWallTileNames[EWallTileType::IE_BL], r + 1, c - 1);
				Map[r][c - 1] = CreateTile(GetRandomizedImage(StoneWallTileNames[EWallTileType::IE_L]), r, c - 1);
				Map[r + 1][c] = CreateTile(GetRandomizedImage(StoneWallTileNames[EWallTileType::IE_B]), r + 1, c);
			}
			else if (tileName == "F_BR") {
				Map[r + 1][c + 1] = CreateTile(StoneWallTileNames[EWallTileType::IE_BR], r + 1, c + 1);
				Map[r][c + 1] = CreateTile(GetRandomizedImage(StoneWallTileNames[EWallTileType::IE_R]), r, c + 1);
				Map[r + 1][c] = CreateTile(GetRandomizedImage(StoneWallTileNames[EWallTileType::IE_B]), r + 1, c);
			}
			else if (tileName == "F_U" && Map[r - 1][c].TextureID.find("F_J") == std::string::npos) {
				Map[r - 2][c] = CreateTile(GetRandomizedImage(StoneWallTileNames[EWallTileType::IE_UTop]), r - 2, c);
				Map[r - 1][c] = CreateTile(GetRandomizedImage(StoneWallTileNames[EWallTileType::IE_UBottom]), r - 1, c);
			}
			else if (tileName == "F_B" && Map[r + 1][c].TextureID.find("F_J") == std::string::npos)
				Map[r + 1][c] = CreateTile(GetRandomizedImage(StoneWallTileNames[EWallTileType::IE_B]), r + 1, c);
			else if (tileName == "F_L" && Map[r][c - 1].TextureID.find("F_J") == std::string::npos)
				Map[r][c - 1] = CreateTile(GetRandomizedImage(StoneWallTileNames[EWallTileType::IE_L]), r, c - 1);
			else if (tileName == "F_R" && Map[r][c + 1].TextureID.find("F_J") == std::string::npos)
				Map[r][c + 1] = CreateTile(GetRandomizedImage(StoneWallTileNames[EWallTileType::IE_R]), r, c + 1);
		}
}

void GMap::PlaceCorridorBorders(const FCorridor& Corridor)
{
	auto CanPlace = [&](int r, int c) -> bool {
		if (r < 0 || r >= Height || c < 0 || c >= Width)
			return false;
		return Map[r][c].Type == ETileType::Dirt;
		};

	for (auto& Tile : Corridor.Tiles) {
		if (Map[Tile.second.x][Tile.second.y - 1].TextureID.find("row-10") != std::string::npos ||
			Map[Tile.second.x][Tile.second.y - 1].TextureID.find("row-20") != std::string::npos)
			continue;

		if (Map[Tile.second.x][Tile.second.y + 1].TextureID.find("row-10") != std::string::npos ||
			Map[Tile.second.x][Tile.second.y + 1].TextureID.find("row-20") != std::string::npos)
			continue;

		if (Tile.first == ECorridorTileType::Horizontal) {
			if (Map[Tile.second.x - 1][Tile.second.y].Type != ETileType::Floor)
				if (Map[Tile.second.x - 2][Tile.second.y].Type == ETileType::Floor)
					Map[Tile.second.x - 1][Tile.second.y] = CreateTile(GetRandomizedImage(StoneWallTileNames[EWallTileType::IE_TopBottom]), Tile.second.x - 1, Tile.second.y);
				else
					Map[Tile.second.x - 1][Tile.second.y] = CreateTile(GetRandomizedImage(StoneWallTileNames[EWallTileType::IE_UTop]), Tile.second.x - 1, Tile.second.y);

			if (Map[Tile.second.x + 1][Tile.second.y].Type != ETileType::Floor)
				if (Map[Tile.second.x + 2][Tile.second.y].Type == ETileType::Floor)
					Map[Tile.second.x + 1][Tile.second.y] = CreateTile(GetRandomizedImage(StoneWallTileNames[EWallTileType::IE_TopBottom]), Tile.second.x + 1, Tile.second.y);
				else
					Map[Tile.second.x + 1][Tile.second.y] = CreateTile(GetRandomizedImage(StoneWallTileNames[EWallTileType::IE_B]), Tile.second.x + 1, Tile.second.y);
		}
		else if (Tile.first == ECorridorTileType::Vertical) {
			if (Map[Tile.second.x][Tile.second.y - 1].Type != ETileType::Floor)
				if (Map[Tile.second.x][Tile.second.y - 2].Type == ETileType::Floor)
					Map[Tile.second.x][Tile.second.y - 1] = CreateTile(GetRandomizedImage(StoneWallTileNames[EWallTileType::IE_LeftRight]), Tile.second.x, Tile.second.y - 1);
				else
					Map[Tile.second.x][Tile.second.y - 1] = CreateTile(GetRandomizedImage(StoneWallTileNames[EWallTileType::IE_L]), Tile.second.x, Tile.second.y - 1);

			if (Map[Tile.second.x][Tile.second.y + 1].Type != ETileType::Floor)
				if (Map[Tile.second.x][Tile.second.y + 2].Type == ETileType::Floor)
					Map[Tile.second.x][Tile.second.y + 1] = CreateTile(GetRandomizedImage(StoneWallTileNames[EWallTileType::IE_LeftRight]), Tile.second.x, Tile.second.y + 1);
				else
					Map[Tile.second.x][Tile.second.y + 1] = CreateTile(GetRandomizedImage(StoneWallTileNames[EWallTileType::IE_R]), Tile.second.x, Tile.second.y + 1);
		}
		
		PlaceCorridorJunctionImages(Tile.second.x, Tile.second.y);

		if (Tile.first == ECorridorTileType::Corner) {
			int r = Tile.second.x;
			int c = Tile.second.y;

			bool upIsJunction = Map[r - 1][c].TextureID.find("F_J") != std::string::npos;
			bool downIsJunction = Map[r + 1][c].TextureID.find("F_J") != std::string::npos;
			bool leftIsJunction = Map[r][c - 1].TextureID.find("F_J") != std::string::npos;
			bool rightIsJunction = Map[r][c + 1].TextureID.find("F_J") != std::string::npos;

			if (!leftIsJunction) {
				if (CanPlace(r, c - 1))
					Map[r][c - 1] = CreateTile(StoneWallTileNames[EWallTileType::IE_L], r, c - 1);

				if (upIsJunction) {
					if (CanPlace(r + 1, c))
						Map[r + 1][c] = CreateTile(StoneWallTileNames[EWallTileType::IE_B], r + 1, c);
					if (CanPlace(r + 1, c - 1))
						Map[r + 1][c - 1] = CreateTile(StoneWallTileNames[EWallTileType::EC_UR], r + 1, c - 1);
					if (CanPlace(r - 1, c + 1))
						Map[r - 1][c + 1] = CreateTile(StoneWallTileNames[EWallTileType::IC_BL], r - 1, c + 1);
				}
				else if (downIsJunction) {
					if (CanPlace(r - 1, c))
						Map[r - 1][c] = CreateTile(StoneWallTileNames[EWallTileType::IE_UTop], r - 1, c);
					if (CanPlace(r - 1, c - 1))
						Map[r - 1][c - 1] = CreateTile(StoneWallTileNames[EWallTileType::EC_BR], r - 1, c - 1);
					if (CanPlace(r + 1, c + 1))
						Map[r + 1][c + 1] = CreateTile(StoneWallTileNames[EWallTileType::IC_UL], r + 1, c + 1);
				}
			}
			else if (!rightIsJunction) {
				if (CanPlace(r, c + 1))
					Map[r][c + 1] = CreateTile(StoneWallTileNames[EWallTileType::IE_R], r, c + 1);

				if (upIsJunction) {
					if (CanPlace(r + 1, c))
						Map[r + 1][c] = CreateTile(StoneWallTileNames[EWallTileType::IE_B], r + 1, c);
					if (CanPlace(r + 1, c + 1))
						Map[r + 1][c + 1] = CreateTile(StoneWallTileNames[EWallTileType::EC_UL], r + 1, c + 1);
					if (CanPlace(r - 1, c - 1))
						Map[r - 1][c - 1] = CreateTile(StoneWallTileNames[EWallTileType::IC_BR], r - 1, c - 1);
				}
				else if (downIsJunction) {
					if (CanPlace(r - 1, c))
						Map[r - 1][c] = CreateTile(StoneWallTileNames[EWallTileType::IE_UTop], r - 1, c);
					if (CanPlace(r - 1, c + 1))
						Map[r - 1][c + 1] = CreateTile(StoneWallTileNames[EWallTileType::EC_BL], r - 1, c + 1);
					if (CanPlace(r + 1, c - 1))
						Map[r + 1][c - 1] = CreateTile(StoneWallTileNames[EWallTileType::IC_UR], r + 1, c - 1);
				}
			}
		}
	}
}

std::string GMap::GetRandomizedImage(std::string imagesName)
{
	std::vector<std::string> options;
	std::stringstream ss(imagesName);
	std::string token;

	while (std::getline(ss, token, '|'))
		options.push_back(token);

	return options[rand() % options.size()];
}

void GMap::PlaceCorridorJunctionImages(int row, int col)
{
	auto isRightWall = [&](int r, int c) {
		return Map[r][c].TextureID == "F_R" ||
			Map[r][c].TextureID == "F_BR" ||
			Map[r][c].TextureID == "F_UR" ||
			Map[r][c].TextureID.find("row-10") != std::string::npos ||
			Map[r][c].TextureID.find("row-20") != std::string::npos;
		};

	auto isLeftWall = [&](int r, int c) {
		return Map[r][c].TextureID == "F_L" ||
			Map[r][c].TextureID == "F_BL" ||
			Map[r][c].TextureID == "F_UL" ||
			Map[r][c].TextureID.find("row-10") != std::string::npos ||
			Map[r][c].TextureID.find("row-20") != std::string::npos;
		};

	auto isBottomWall = [&](int r, int c) {
		return Map[r][c].TextureID.find("F_B") != std::string::npos;
		};

	auto isTopWall = [&](int r, int c) {
		return Map[r][c].TextureID.find("F_U") != std::string::npos;
		};

	auto isJunction = [&](int r, int c) {
		return Map[r][c].TextureID.find("F_J") != std::string::npos;
		};

	if (isBottomWall(row - 1, col)) {
		if (isBottomWall(row - 1, col - 1))
			Map[row][col - 1] = CreateTile(StoneWallTileNames[EWallTileType::EE_UR], row, col - 1);
		if (isBottomWall(row - 1, col + 1))
			Map[row][col + 1] = CreateTile(StoneWallTileNames[EWallTileType::EE_UL], row, col + 1);
	}
	else if (isTopWall(row + 2, col)) {
		if (isTopWall(row + 2, col - 1))
			Map[row][col - 1] = CreateTile(StoneWallTileNames[EWallTileType::EE_BR], row - 1, col - 1);
		if (isTopWall(row + 2, col + 1))
			Map[row][col + 1] = CreateTile(StoneWallTileNames[EWallTileType::EE_BL], row - 1, col + 1);
	}

	if (isRightWall(row, col - 1)) {
		if (isRightWall(row - 1, col - 1) || isJunction(row - 1, col - 1))
			Map[row - 1][col] = CreateTile(StoneWallTileNames[EWallTileType::EE_BL], row - 1, col);
		if (isRightWall(row + 1, col - 1) || isJunction(row + 1, col - 1))
			Map[row + 1][col] = CreateTile(StoneWallTileNames[EWallTileType::EE_UL], row + 1, col);
	}
	else if (isLeftWall(row, col + 1)) {
		if (isLeftWall(row - 1, col + 1) || isJunction(row - 1, col + 1))
			Map[row - 1][col] = CreateTile(StoneWallTileNames[EWallTileType::EE_BR], row - 1, col);
		if (isLeftWall(row + 1, col + 1) || isJunction(row + 1, col + 1))
			Map[row + 1][col] = CreateTile(StoneWallTileNames[EWallTileType::EE_UR], row + 1, col);
	}
}

bool GMap::IsTooCloseToCorridor(int r, int c, bool bEndPoint)
{
	for (int dr = -2; dr <= 2; dr++)
		for (int dc = -2; dc <= 2; dc++) {
			int nr = r + dr;
			int nc = c + dc;

			if (nr < 0 || nc < 0 || nr >= Height || nc >= Width)
				continue;

			if (Map[nr][nc].TextureID.find("F_J") != std::string::npos)
			{
				int dist = abs(dr) + abs(dc);

				if (bEndPoint && dist == 1)
					continue;

				return true;
			}
		}

	return false;
}

void GMap::PlaceExit()
{
	sf::Vector2f LastRoomOrigin = Rooms.back().Origin;

	int row = LastRoomOrigin.y / PixelsPerTile;
	int col = LastRoomOrigin.x / PixelsPerTile;

	int j = 0;

	while(Map[row - 1][col + j].Type == ETileType::Wall)
		Map[row - 1][col + j] = CreateTile("row-10-column-8", row - 1, col + j);
}