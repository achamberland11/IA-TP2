//
// Created by Antoine on 2026-03-07.
//

#pragma once
#include "World.h"
#include "../Core/Object.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"


class GGame : public GObject
{
public:
    GGame(sf::Vector2u windowSize);
    virtual ~GGame();

    void Run();

private:
    void Initialize();
    void HandleEvents();
    void Update(float dt);
    void Render();

private:
    sf::Vector2u WindowSize;
    sf::RenderWindow Window;
    std::unique_ptr<GWorld> World;
};
