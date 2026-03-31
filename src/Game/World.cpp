//
// Created by Antoine on 2026-03-07.
//

#include "World.h"

#include "Entities/Characters/PlayerCharacter.h"
#include "Entities/Characters/AgentCharacter.h"
#include "Map/AStar.h"
#include "Controllers/AgentController.h"

GWorld::GWorld(sf::Vector2u windowSize, IGameEventListener *l) : WindowSize(windowSize)
{
	Listener = l;

	CreateMap();
	CreateEntities();
}

GWorld::~GWorld()
{
	for (GController *controller : Controllers)
	{
		controller = nullptr;
	}

	for (GEntity *entity : Entities)
	{
		delete entity;
	}
}

void GWorld::Start()
{
	for (GEntity *entity : Entities)
		entity->Start();
}

void GWorld::Update(float deltaSeconds)
{
	for (GEntity *entity : Entities)
	{
		entity->Update(deltaSeconds);
	}
}

void GWorld::Render(sf::RenderWindow &window)
{
	if (Map)
		Map->Display(window);

	for (GEntity *entity : Entities)
	{
		entity->Render(window);
	}
}

void GWorld::HandleEvent(const sf::Event &event)
{
	for (GController *controller : Controllers)
	{
		controller->HandleEvent(event);
	}
}

void GWorld::TryInteractSwitch()
{
	if (!GameSwitch)
		return;

	if (GameSwitch->IsCollidingWith(Player, Player->GetRendererComponent()->GetShape().getSize().x))
		GameSwitch->Interact();
}

void GWorld::CreateMap()
{
	Map = std::make_unique<GMap>(WindowSize.x, WindowSize.y);
	Map->LoadMap("Assets/map.csv");
}

void GWorld::CreateEntities()
{
	CreatePlayer();
	CreateAgents();
	CreateSwitch();
}

void GWorld::CreatePlayer()
{
	GPlayerCharacter *player = new GPlayerCharacter();
	player->SetListener(Listener);
	player->GetTransformComponent()->SetScale(sf::Vector2f(2, 2));
	Entities.push_back(player);
	Controllers.push_back(player->GetController());
	Player = player;
}

void GWorld::CreateAgents()
{
	const std::vector<FRoom> &Rooms = Map->GetRooms();

	for (int i = 0; i < Rooms.size(); i++)
		if (!Rooms[i].bIsBreakRoom)
		{
			GAgentCharacter *agent = new GAgentCharacter();
			agent->SetAgentID(i);
			agent->SetListener(Listener);
			agent->SetRoom(Rooms[i]);
			agent->SetPatrolWaypoints(GMap::PixelsPerTile);
			agent->GetTransformComponent()->SetScale(sf::Vector2f(1.25f, 1.25f));
			agent->GetTransformComponent()->SetPosition(Rooms[i].Center());
			Entities.push_back(agent);
			Controllers.push_back(agent->GetController());
			Agents.push_back(agent);
		}
}

void GWorld::CreateSwitch()
{
	GSwitch *sw = new GSwitch();

	sf::Vector2f Pos;
	sf::Vector2f GridPos;

	int row = 0;
	int col = 0;

	bool bFoundValidPos = false;

	for (int attempt = 0; attempt < 50; attempt++) {
		Pos = Map->GetRandomPosition();
		GridPos = GetMap()->WorldToGrid(Pos);

		row = static_cast<int>(GridPos.y);
		col = static_cast<int>(GridPos.x);

		bool bOccupied = false;

		for (GEntity* Entity : Entities) {
			sf::Vector2f EntityPos = Entity->GetTransformComponent()->GetPosition();
			sf::Vector2f EntityGrid = GetMap()->WorldToGrid(EntityPos);

			int eRow = static_cast<int>(EntityGrid.y);
			int eCol = static_cast<int>(EntityGrid.x);

			if (eRow == row && eCol == col) {
				bOccupied = true;
				break;
			}
		}

		if (bOccupied)
			continue;

		bFoundValidPos = true;
		break;
	}

	if (!bFoundValidPos) {
		std::cerr << "Failed to find valid position for switch\n";
		delete sw;
		return;
	}

	sw->GetTransformComponent()->SetPosition(Pos);
	sw->SetListener(Listener);
	Entities.push_back(sw);
	GameSwitch = sw;

	GetMap()->SetTileToObstacle(row, col);
}
