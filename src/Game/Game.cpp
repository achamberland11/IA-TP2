//
// Created by Antoine on 2026-03-07.
//

#include "Game.h"

GGame *GGame::Instance = nullptr;

GGame::GGame(sf::Vector2u windowSize)
    : WindowSize(windowSize), Window(sf::VideoMode(windowSize), "TP AI"), EndText(Font) {
    Instance = this;
    Window.setFramerateLimit(120);
    Initialize();
}

GGame::~GGame() {
    Instance = nullptr;
}

void GGame::Initialize() {
    World = std::make_unique<GWorld>(WindowSize, this);
    World->Start();

    if (!Font.openFromFile("Assets/Fonts/Roboto/Roboto.ttf"))
        std::cerr << "Failed to load font" << std::endl;

    EndText.setFont(Font);
    EndText.setCharacterSize(80);
    EndText.setFillColor(sf::Color::White);
    EndText.setStyle(sf::Text::Bold);
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
    if (GameState == EGameState::Running && World) {
        World->Update(dt);
    }
}

void GGame::Render() {
    Window.clear(sf::Color::Black);

    if (GameState == EGameState::Running) {
        if (World)
            World->Render(Window);
    }
    else
        Window.draw(EndText);

    Window.display();
}

void GGame::OnGameOver()
{
    GameState = EGameState::Lost;

    EndText.setFillColor(sf::Color::Red);
    EndText.setString("GAME OVER!");

    sf::FloatRect Bounds = EndText.getLocalBounds();
    EndText.setOrigin(sf::Vector2f(Bounds.size.x / 2, Bounds.size.y / 2));
    EndText.setPosition(sf::Vector2f(WindowSize.x / 2, WindowSize.y / 2));

    std::cout << "Agent collided with player! Exiting program.\n";
}

void GGame::OnGameWon()
{
    GameState = EGameState::Won;

    EndText.setFillColor(sf::Color::Green);
    EndText.setString("YOU WON!");

    sf::FloatRect Bounds = EndText.getLocalBounds();
    EndText.setOrigin(sf::Vector2f(Bounds.size.x / 2, Bounds.size.y / 2));
    EndText.setPosition(sf::Vector2f(WindowSize.x / 2, WindowSize.y / 2));

    std::cout << "Player exited the mine! Exiting program.\n";
}
