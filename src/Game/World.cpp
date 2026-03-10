//
// Created by Antoine on 2026-03-07.
//

#include "World.h"

#include "Entities/Characters/PlayerCharacter.h"
#include "Entities/Characters/AgentCharacter.h"
#include "Map/AStar.h"
#include "Controllers/AgentController.h"

GWorld::GWorld(sf::Vector2u windowSize) : WindowSize(windowSize) {
	CreateMap();
	CreateEntities();
}

GWorld::~GWorld() {
	for (GController* controller : Controllers) {
		controller = nullptr;
	}

	for (GEntity* entity : Entities) {
		delete entity;
	}
}

void GWorld::Update(float deltaSeconds) {
	pathTimer += deltaSeconds;

	if (pathTimer >= pathInterval) {
		GAgentCharacter* agent = nullptr;
		GPlayerCharacter* player = nullptr;

		for (GEntity* entity : Entities) {
			if (!agent)
				agent = dynamic_cast<GAgentCharacter*>(entity);

			if (!player)
				player = dynamic_cast<GPlayerCharacter*>(entity);
		}

		if (agent && player) {
			auto path = AStar::FindPath(Map.get(),
				agent->GetTransformComponent()->GetPosition(),
				player->GetTransformComponent()->GetPosition());

			GAgentController* agentCtrl = dynamic_cast<GAgentController*>(agent->GetController());

			agentCtrl->SetTargets(path);
		}

		pathTimer = 0.f;

		if (agent->IsCollidingWith(player, 8.f)) {
			std::cout << "Agent collided with player! Exiting program.\n";
			std::exit(0);
		}
	}

	for (GEntity* entity : Entities) {
		entity->Update(deltaSeconds);
	}
}

GPlayerCharacter* GWorld::GetPlayerCharacter() {
    for (GEntity* entity : Entities) {
        if (auto player = dynamic_cast<GPlayerCharacter*>(entity))
            return player;
    }

    return nullptr;
}

GAgentCharacter* GWorld::GetAgentCharacter() {
    for (GEntity* entity : Entities) {
        if (auto agent = dynamic_cast<GAgentCharacter*>(entity))
            return agent;
    }

    return nullptr;
}

void GWorld::Render(sf::RenderWindow& window) {
	if (Map) Map->Display(window);

	for (GEntity* entity : Entities) {
		if (auto agent = dynamic_cast<GAgentCharacter*>(entity))
			agent->DrawDebug(window);

		entity->Render(window);
	}
}

void GWorld::HandleEvent(const sf::Event& event) {
	for (GController* controller : Controllers) {
		controller->HandleEvent(event);
	}
}

void GWorld::CreateMap() {
	Map = std::make_unique<GMap>(WindowSize.x, WindowSize.y);
	Map->LoadMap("Assets/map.csv");
}

void GWorld::CreateEntities() {
	auto* player = CreatePlayer();
	CreateAgent(player);
}

GPlayerCharacter* GWorld::CreatePlayer() {
	GPlayerCharacter* player = new GPlayerCharacter();
	// player->GetTransformComponent()->SetPosition(sf::Vector2f(WindowSize.x / 2.f, WindowSize.y / 2.f));
	player->GetTransformComponent()->SetScale(sf::Vector2f(2, 2));
	Entities.push_back(player);
	Controllers.push_back(player->GetController());

	return player;
}

void GWorld::CreateAgent(GPlayerCharacter* player) {
	GAgentCharacter* agent = new GAgentCharacter();
	dynamic_cast<GAgentController*>(agent->GetController())->SetPlayer(player);
	// agent->GetTransformComponent()->SetPosition(sf::Vector2f(200, 100));
	agent->GetTransformComponent()->SetScale(sf::Vector2f(1.25f, 1.25f));
	Entities.push_back(agent);
}