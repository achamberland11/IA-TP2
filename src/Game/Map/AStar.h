#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class GMap;

struct FAStarNode {
	int row, col;
	float g = 0.f; //Cost
	float h = 0.f; //Heuristic to goal
	float f = 0.f; //g + h

	FAStarNode* parent = nullptr;
};

class AStar {
public:
	static std::vector<sf::Vector2f> FindPath(GMap* map, sf::Vector2f worldStart, sf::Vector2f worldEnd);
};
