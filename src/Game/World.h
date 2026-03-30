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
#include "GameEventListener.h"

class GWorld : public GObject
{
public:
    GWorld(sf::Vector2u windowSize, IGameEventListener* l);
    ~GWorld() override;

    void Start();
    void Update(float deltaSeconds);

    void Render(sf::RenderWindow& window);

    void HandleEvent(const sf::Event &event);

    GMap *GetMap() const { return Map.get(); }
    GPlayerCharacter *GetPlayerCharacter() const { return Player; }
    std::vector<GAgentCharacter*> GetAgentsCharacter() const { return Agents; }

private:
    void CreateMap();
    void CreateEntities();
    void CreatePlayer();
    void CreateAgents();

    sf::Vector2u WindowSize;
    std::unique_ptr<GMap> Map;

    std::vector<GEntity*> Entities;
    std::vector<GController*> Controllers;

    GPlayerCharacter *Player;
    std::vector<GAgentCharacter*> Agents;

    IGameEventListener* Listener = nullptr;
};
