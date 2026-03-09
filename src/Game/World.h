//
// Created by Antoine on 2026-03-07.
//

#pragma once
#include "../Core/Object.h"
#include "Entities/Entity.h"
#include "Map/Map.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"


class GWorld : public GObject
{
public:
    GWorld(sf::Vector2u windowSize);
    ~GWorld() override;

    void Update(float deltaSeconds);
    void Render(sf::RenderWindow& window);

    GMap* GetMap() const { return Map.get(); }

private:
    void CreateMap();
    void CreateEntities();

private:
    sf::Vector2u WindowSize;
    std::unique_ptr<GMap> Map;

    std::vector<GEntity*> Entities;
};
