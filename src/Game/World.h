//
// Created by Antoine on 2026-03-07.
//

#pragma once
#include "../Core/Object.h"
#include "Controllers/Controller.h"
#include "Entities/Entity.h"
#include "Entities/Characters/AgentCharacter.h"
#include "Entities/Characters/PlayerCharacter.h"
#include "Map/Map.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"

class GWorld : public GObject
{
public:
    GWorld(sf::Vector2u windowSize);
    ~GWorld() override;

    void Update(float deltaSeconds);

    GPlayerCharacter *GetPlayerCharacter();

    GAgentCharacter *GetAgentCharacter();

    void Render(sf::RenderWindow& window);

    void HandleEvent(const sf::Event& event);

    GMap* GetMap() const { return Map.get(); }

private:
    void CreateMap();
    void CreateEntities();
    void CreatePlayer();
    void CreateAgent();

    sf::Vector2u WindowSize;
    std::unique_ptr<GMap> Map;

    std::vector<GEntity*> Entities;
    std::vector<GController*> Controllers;

    float pathTimer = 0.f;
    float pathInterval = 0.f;
};
