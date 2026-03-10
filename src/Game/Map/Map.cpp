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
    for (const auto &entry: std::filesystem::directory_iterator(folderPath))
    {
        if (entry.path().extension() == ".png")
        {
            std::string tileName = entry.path().stem().string();

            if (!Textures[tileName].loadFromFile(entry.path().string()))
                std::cerr << "Error loading : " << entry.path() << std::endl;
        }
    }
}

FTile GMap::CreateTile(const std::string &rawValue)
{
    FTile tile{};

    tile.TextureID = rawValue;

    size_t sep = rawValue.find('|');
    std::string base = (sep != std::string::npos) ? rawValue.substr(0, sep) : rawValue;
    tile.ObjectID = (sep != std::string::npos) ? rawValue.substr(sep + 1) : "";

    tile.TextureID = base;

    if (rawValue == "Dirt")
    {
        tile.TextureID += std::to_string(GetRand(22, 85));
        tile.Type = ETileType::Dirt;
        tile.Walkable = false;
        tile.BlocksVision = false;
        tile.MovementCost = 10;
    } else if (rawValue.find("F_") == 0)
    {
        if (rawValue.find("F_M") == 0)
            tile.TextureID += std::to_string(GetRand(8, 65));
        if (rawValue.find("F_J") == 0)
            tile.TextureID += std::to_string(GetRand(5, 10));

        tile.Type = ETileType::Floor;
        tile.Walkable = true;
        tile.BlocksVision = false;
        tile.MovementCost = 1;
    } else
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

            Map[row][col] = CreateTile(value);
        }
    }

    for (auto &[tile, texture]: Textures)
    {
        Sprites[tile] = std::make_unique<sf::Sprite>(texture);

        sf::Vector2u textureSize = texture.getSize();
        float scaleX = static_cast<float>(PixelsPerTile) / textureSize.x;
        float scaleY = static_cast<float>(PixelsPerTile) / textureSize.y;
        Sprites[tile]->setScale(sf::Vector2f(scaleX, scaleY));
    }

    BuildNavGraph();
}

void GMap::Display(sf::RenderWindow &window)
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

            const std::string &tileName = Map[row][col].TextureID;

            auto it = Sprites.find(tileName);

            if (it == Sprites.end())
                continue;

            it->second->setPosition(position);

            window.draw(*it->second);

            const std::string &objectName = Map[row][col].ObjectID;

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
    // return Map[row][col].Walkable;
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

    for(int r = 0; r < Height; r++)
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
