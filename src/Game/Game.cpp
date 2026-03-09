//
// Created by Antoine on 2026-03-07.
//

#include "Game.h"

GGame::GGame(sf::Vector2u windowSize)
: WindowSize(windowSize), Window(sf::VideoMode(windowSize), "TP AI")
{
    Window.setFramerateLimit(120);
    Initialize();
}

GGame::~GGame() = default;

void GGame::Initialize()
{
    World = std::make_unique<GWorld>(WindowSize);
}

void GGame::Run() {
    sf::Clock clock;

    while (Window.isOpen()) {
        const float deltaTime = clock.restart().asSeconds();
        HandleEvents();
        Update(deltaTime);
        Render();
    }
}

void GGame::HandleEvents() {
    while (const std::optional event = Window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            Window.close();
        }
    }
}

void GGame::Update(float dt) {
    if (World) {
        World->Update(dt);
    }
}

void GGame::Render() {
    Window.clear(sf::Color::Black);

    if (World) {
        World->Render(Window);
    }

    Window.display();
}