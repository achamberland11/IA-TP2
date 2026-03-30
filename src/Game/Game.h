//
// Created by Antoine on 2026-03-07.
//

#pragma once
#include "World.h"
#include "../Core/Object.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include "GameEventListener.h"

class GGame : public GObject, public IGameEventListener
{
public:
    GGame(sf::Vector2u windowSize);
    virtual ~GGame();

    static GGame* GetInstance() { return Instance; }

    void Run();

    GWorld* GetWorld() const { return World.get(); }
    GMap* GetMap() const { return World->GetMap(); }
    sf::RenderWindow& GetWindow() { return Window; }
    GPlayerCharacter* GetPlayerCharacter() const { return World->GetPlayerCharacter(); }
    std::vector<GAgentCharacter*> GetAgentsCharacter() const { return World->GetAgentsCharacter(); }

    void OnGameOver() override;
    void OnGameWon() override;

private:
    void Initialize();
    void HandleEvents();
    void Update(float dt);
    void Render();

private:
    static GGame* Instance;

    sf::Vector2u WindowSize;
    sf::RenderWindow Window;
    std::unique_ptr<GWorld> World;
};
