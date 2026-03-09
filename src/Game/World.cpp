//
// Created by Antoine on 2026-03-07.
//

#include "World.h"

#include "Entities/Characters/PlayerCharacter.h"
#include "Entities/Characters/AgentCharacter.h"

GWorld::GWorld(sf::Vector2u windowSize) : WindowSize(windowSize) {
    CreateMap();
    CreateEntities();
}

GWorld::~GWorld() {
    for (GController *controller: Controllers) {
        controller = nullptr;
    }

    for (GEntity *entity: Entities) {
        delete entity;
    }
}

void GWorld::Update(float deltaSeconds) {
    for (GEntity* entity: Entities) {
        entity->Update(deltaSeconds);
    }
}

void GWorld::Render(sf::RenderWindow& window) {
    if (Map) Map->Display(window);

    for (GEntity* entity: Entities) {
        entity->Render(window);
    }
}

void GWorld::HandleEvent(const sf::Event &event) {
    for (GController* controller : Controllers) {
        controller->HandleEvent(event);
    }
}

void GWorld::CreateMap() {
    Map = std::make_unique<GMap>(WindowSize.x, WindowSize.y);
    Map->LoadMap("Assets/map.csv");
}

void GWorld::CreateEntities() {
    // @TODO Create Agent and the Intruder (maybe obstacles?)
    CreatePlayer();
    CreateAgent();
}

void GWorld::CreatePlayer() {
    GPlayerCharacter* player = new GPlayerCharacter();
    player->GetTransformComponent()->SetPosition(sf::Vector2f(WindowSize.x / 2.f, WindowSize.y / 2.f));
    player->GetTransformComponent()->SetScale(sf::Vector2f(2, 2));
    Entities.push_back(player);
    Controllers.push_back(player->GetController());
}

void GWorld::CreateAgent() {
    GAgentCharacter* agent = new GAgentCharacter();
    agent->GetTransformComponent()->SetPosition(sf::Vector2f(200, 100));
    agent->GetTransformComponent()->SetScale(sf::Vector2f(2, 2));
    Entities.push_back(agent);
}