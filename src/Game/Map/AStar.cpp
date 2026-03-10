#include "AStar.h"
#include "Map.h"
#include <vector>
#include <queue>

struct NodeCompare {
	bool operator()(FAStarNode* a, FAStarNode* b) { return a->f > b->f; }
};

static float Heuristic(int r1, int c1, int r2, int c2) {
	return std::sqrt(static_cast<float>((r2 - r1) * (r2 - r1) + (c2 - c1) * (c2 - c1)));
}

std::vector<sf::Vector2f> AStar::FindPath(GMap* map, sf::Vector2f worldStart, sf::Vector2f worldEnd)
{
	sf::Vector2f gridStart = map->WorldToGrid(worldStart);
	sf::Vector2f gridEnd = map->WorldToGrid(worldEnd);

	int startRow = static_cast<int>(gridStart.y);
	int startCol = static_cast<int>(gridStart.x);
	int endRow = static_cast<int>(gridEnd.y);
	int endCol = static_cast<int>(gridEnd.x);

	if (!map->IsWalkable(endRow, endCol))
		return {};

	std::vector<std::vector<FAStarNode*>> grid(map->GetHeight(), std::vector<FAStarNode*>(map->GetWidth(), nullptr));
	std::vector<std::vector<bool>> closed(map->GetHeight(), std::vector<bool>(map->GetWidth(), false));
	std::priority_queue<FAStarNode*, std::vector<FAStarNode*>, NodeCompare> open;

	FAStarNode* startNode = new FAStarNode{ startRow, startCol, 0, 0, 0, nullptr };
	grid[startRow][startCol] = startNode;
	open.push(startNode);

	FAStarNode* goalNode = nullptr;

	while (!open.empty()) {
		FAStarNode* currentNode = open.top();
		open.pop();

		if (currentNode->row == endRow && currentNode->col == endCol) {
			goalNode = currentNode;
			break;
		}

		if (closed[currentNode->row][currentNode->col])
			continue;

		closed[currentNode->row][currentNode->col] = true;

		int currentIndex = map->GetNavIndex(currentNode->row, currentNode->col);

		for (int neighborIndex : map->NavGraph[currentIndex].Neighbors) {
			int neighborRow = map->NavGraph[neighborIndex].Row;
			int neighborCol = map->NavGraph[neighborIndex].Col;

			if (closed[neighborRow][neighborCol])
				continue;

			float newG = currentNode->g + map->GetMovementCost(neighborRow, neighborCol);

			if (!grid[neighborRow][neighborCol] || newG < grid[neighborRow][neighborCol]->g) {
				float newH = Heuristic(neighborRow, neighborCol, endRow, endCol);

				FAStarNode* neighborNode = new FAStarNode{ neighborRow, neighborCol, newG + newH, newG, newH, currentNode };
				grid[neighborRow][neighborCol] = neighborNode;
				open.push(neighborNode);
			}
		}
	}

	std::vector<sf::Vector2f> path;

	for (FAStarNode* node = goalNode; node; node = node->parent)
		path.push_back(map->GridToWorld({ (float)node->col, (float)node->row }));

	std::reverse(path.begin(), path.end());

	for (auto& row : grid)
		for (auto* node : row)
			delete node;

	return path;
}
