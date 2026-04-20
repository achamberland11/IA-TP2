//
// Created by Antoine on 2026-03-07.
//

#include "Game.h"

GGame *GGame::Instance = nullptr;
sf::Font GGame::Font;

GGame::GGame(sf::Vector2u windowSize)
    : WindowSize(windowSize), Window(sf::VideoMode(windowSize), "TP AI"){
    Instance = this;
    Window.setFramerateLimit(120);
    Initialize();
}

GGame::~GGame() {
    World.reset();
    GlobalBB.reset();
    delete EndText;
    EndText = nullptr;
    Instance = nullptr;
}

void GGame::Initialize() {
    GlobalBB = std::make_unique<GlobalBlackboard>();

    World = std::make_unique<GWorld>(WindowSize, this);
    World->Start();

    Font = sf::Font();
    
    if (Font.openFromFile("Assets/Fonts/Roboto/Roboto.ttf"))
        bFontLoaded = true;
    else
        std::cerr << "Failed to load font" << std::endl;

    EndText = new sf::Text(Font);
    EndText->setCharacterSize(80);
    EndText->setFillColor(sf::Color::White);
    EndText->setStyle(sf::Text::Bold);
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

    if (GlobalBB) {
        GlobalBB->SyncRadio();
    }

    CheckWinCondition();
}

void GGame::Render() {
    Window.clear(sf::Color::Black);

    if (GameState == EGameState::Running) {
        if (World)
            World->Render(Window);
    }
    else
        Window.draw(*EndText);

    sf::Text PressQText(Font);
    sf::Text PressEText(Font);

    PressQText.setString("Press Q to activate the lever and open the door");
    PressQText.setPosition(sf::Vector2f(30,30));
    PressEText.setString("Press E to jam the enemies radio for 5 sec");
    PressEText.setPosition(sf::Vector2f(30,85));
    Window.draw(PressQText);
    Window.draw(PressEText);
    if (GetPlayerCharacter()->bRadioJamming)
    {
        sf::Text JamCooldownText(Font);

        JamCooldownText.setString("Jam cooldown: " + std::to_string(GetPlayerCharacter()->RadioJamTimer));
        JamCooldownText.setPosition(sf::Vector2f(30,120));

        Window.draw(JamCooldownText);
    }

    sf::CircleShape Circle;
    Circle.setFillColor(sf::Color::Yellow);
    Circle.setRadius(5.f);
    Circle.setPosition(GetGlobalBlackboard()->ListenToRadio().PlayerLastPosition);
    Window.draw(Circle);

    Window.display();
}

void GGame::OnGameOver()
{
    GameState = EGameState::Lost;

    EndText->setFillColor(sf::Color::Red);
    EndText->setString("GAME OVER!");

    sf::FloatRect Bounds = EndText->getLocalBounds();
    EndText->setOrigin(sf::Vector2f(Bounds.size.x / 2, Bounds.size.y / 2));
    EndText->setPosition(sf::Vector2f(WindowSize.x / 2, WindowSize.y / 2));

    std::cout << "Agent collided with player! Exiting program.\n";
}

void GGame::OnGameWon()
{
    GameState = EGameState::Won;

    EndText->setFillColor(sf::Color::Green);
    EndText->setString("YOU WON!");

    sf::FloatRect Bounds = EndText->getLocalBounds();
    EndText->setOrigin(sf::Vector2f(Bounds.size.x / 2, Bounds.size.y / 2));
    EndText->setPosition(sf::Vector2f(WindowSize.x / 2, WindowSize.y / 2));

    std::cout << "Player exited the mine! Exiting program.\n";
}

void GGame::OnSwitchToggled(bool bIsOn)
{
    if (World && World->GetMap())
        World->GetMap()->ChangeExitVisibility(bIsOn);
}

void GGame::CheckWinCondition()
{
    if (GameState != EGameState::Running)
        return;

    if (!World || !World->GetMap() || !World->GetPlayerCharacter())
        return;

    GMap* Map = World->GetMap();
    GPlayerCharacter* Player = World->GetPlayerCharacter();

    sf::Vector2f PlayerPos = Player->GetTransformComponent()->GetPosition();

    sf::Vector2f GridPos = Map->WorldToGrid(PlayerPos);

    int row = static_cast<int>(GridPos.y);
    int col = static_cast<int>(GridPos.x);

    if (!Map->HasExit())
        return;

    if (row == Map->GetExitTilePos().x && col == Map->GetExitTilePos().y)
        OnGameWon();
}
