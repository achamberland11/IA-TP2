//
// Created by Antoine on 2026-03-07.
//

#include "Game.h"

GGame *GGame::Instance = nullptr;

GGame::GGame(sf::Vector2u windowSize)
    : WindowSize(windowSize), Window(sf::VideoMode(windowSize), "TP AI") {
    Instance = this;
    Window.setFramerateLimit(120);
    Initialize();
}

GGame::~GGame() {
    Instance = nullptr;
}

void GGame::Initialize() {
    GlobalBB = std::make_unique<GlobalBlackboard>();

    World = std::make_unique<GWorld>(WindowSize, this);
    World->Start();
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
    while (const std::optional<sf::Event> &event = Window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            Window.close();
        }

        if (World) World->HandleEvent(*event);
    }
}

void GGame::Update(float dt) {
    if (World) {
        World->Update(dt);
    }

    if (GlobalBB) {
        GlobalBB->SyncRadio();
    }
}

void GGame::Render() {
    Window.clear(sf::Color::Black);

    if (World) {
        World->Render(Window);
    }

    Window.display();
}

void GGame::OnGameOver()
{
    std::cout << "Agent collided with player! Exiting program.\n";
    Window.close();
}

void GGame::OnGameWon()
{
    std::cout << "Player exited the mine! Exiting program.\n";
    Window.close();
}
