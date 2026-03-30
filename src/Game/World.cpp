//
// Created by Antoine on 2026-03-07.
//

#include "World.h"

#include "Entities/Characters/PlayerCharacter.h"
#include "Entities/Characters/AgentCharacter.h"
#include "Map/AStar.h"
#include "Controllers/AgentController.h"

GWorld::GWorld(sf::Vector2u windowSize, IGameEventListener*l) : WindowSize(windowSize)
{
    Listener = l;

    CreateMap();
    CreateEntities();
}

GWorld::~GWorld()
{
    for (GController *controller: Controllers)
    {
        controller = nullptr;
    }

    for (GEntity *entity: Entities)
    {
        delete entity;
    }
}

void GWorld::Start()
{
    for (GEntity *entity: Entities)
        entity->Start();
}

void GWorld::Update(float deltaSeconds)
{
	for (GEntity* entity : Entities) {
		entity->Update(deltaSeconds);
	}
}

void GWorld::Render(sf::RenderWindow &window)
{
    if (Map) Map->Display(window);

    for (GEntity *entity: Entities)
    {
        entity->Render(window);
    }
}

void GWorld::HandleEvent(const sf::Event &event)
{
    for (GController *controller: Controllers)
    {
        controller->HandleEvent(event);
    }
}

void GWorld::CreateMap()
{
    Map = std::make_unique<GMap>(WindowSize.x, WindowSize.y);
    Map->LoadMap("Assets/map.csv");
}

void GWorld::CreateEntities() {
	CreatePlayer();
	CreateAgents();
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
    const std::vector<FRoom>& Rooms = Map->GetRooms();

    for (int i = 0; i < Rooms.size(); i++) {
        GAgentCharacter* agent = new GAgentCharacter();
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