//
// Created by Antoine on 2026-03-07.
//

#include "World.h"

GWorld::GWorld(sf::Vector2u windowSize) : WindowSize(windowSize) {
    CreateMap();
    CreateEntities();
}

GWorld::~GWorld() = default;

void GWorld::CreateMap() {
    Map = std::make_unique<GMap>(WindowSize.x, WindowSize.y);
    Map->LoadMap("Assets/map.csv");
}

void GWorld::CreateEntities() {
    // @TODO Create Agent and the Intruder (maybe obstacles?)
}

void GWorld::Update(float deltaSeconds) {
    // @TODO call Update for each entities
}

void GWorld::Render(sf::RenderWindow& window) {
    if (Map) Map->Display(window);

    // @TODO render entitites
}