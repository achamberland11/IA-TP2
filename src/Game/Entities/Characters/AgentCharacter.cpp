#include "AgentCharacter.h"
#include <iostream>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

#include "Game/Controllers/AgentController.h"

GAgentCharacter::GAgentCharacter()
{
    Name = "AgentCharacter";
    Transform->SetPosition(sf::Vector2f(1000, 550));
    Transform->SetScale(sf::Vector2f(1, 1));
    Renderer->SetColor(sf::Color::Magenta);

    FSM = new GFSMComponent(this);
    AddComponent(FSM);

    Controller = new GAgentController(this);

    LoadTextures();

    auto it = m_sprites.find("E_F1");
    if (it != m_sprites.end() && it->second)
        Renderer->SetSprite(*it->second, "E_F1");
    else
        std::cerr << "Error : E_F1 sprite not found" << std::endl;

    MovementSpeed = 75.f; // Agent is slower
    SetWaypoints();
}

GAgentCharacter::~GAgentCharacter()
{
}

void GAgentCharacter::Start()
{
    GCharacter::Start();
}

void GAgentCharacter::Update(float dt)
{
    GCharacter::Update(dt);

    // Sprite rendering
    std::string oldSpriteName = Renderer->GetSpriteName();
    //	sf::Vector2f direction;
    bool flip = true;

    //Flip le sprite si l'agent va a gauche.
    Transform->SetScale(sf::Vector2f(flip ? -std::abs(Transform->GetScale().x) : std::abs(Transform->GetScale().x),
                                     Transform->GetScale().y));

    spriteTimer += dt;

    if (spriteTimer > spriteDuration && !oldSpriteName.empty())
    {
        std::string spriteName = (oldSpriteName.back() == '1') ? "E_F2" : "E_F1";

        auto it = m_sprites.find(spriteName);
        if (it != m_sprites.end() && it->second)
            Renderer->SetSprite(*it->second, spriteName);

        spriteTimer = 0;
    }
}

void GAgentCharacter::Render(sf::RenderWindow &window)
{
    GEntity::Render(window);
    DrawDebug(window);

    // @TODO temporaire, devrait pas être à chaque rendu comme ça
    if (FSM && FSM->GetCurrentState())
    {
        sf::Font font;
        if (!font.openFromFile("Assets/Fonts/Roboto/Roboto.ttf"))
            std::cerr << "Failed to load font" << std::endl;
        sf::Text stateText(font);
        std::string stateName = FSM->GetCurrentState()->GetName();
        stateText.setString("State: " + stateName);
        stateText.setCharacterSize(24);
        stateText.setFillColor(sf::Color::White);
        stateText.setOutlineColor(sf::Color::Black);
        stateText.setOutlineThickness(2.f);

        sf::FloatRect textBounds = stateText.getLocalBounds();
        float textWidth = textBounds.size.x;
        stateText.setPosition({window.getSize().x - textWidth - 20.f, 20.f});

        window.draw(stateText);
    }
}

void GAgentCharacter::DrawDebug(sf::RenderWindow &window)
{
    const sf::Color Color(180, 100, 220, 180);
    const float LineTickness = 5.f;

    sf::Vector2f from = Transform->GetPosition();

    for (int i = 0; i < waypoints.size(); i++)
    {
        sf::Vector2f to = waypoints[i];
        sf::Vector2f diff = to - from;
        float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);

        sf::RectangleShape line(sf::Vector2f(length, LineTickness));
        line.setFillColor(Color);
        line.setOrigin(sf::Vector2f(0.f, LineTickness / 2.f));
        line.setPosition(from);
        line.setRotation(sf::radians(std::atan2(diff.y, diff.x)));

        window.draw(line);

        sf::CircleShape circle(8.f);
        circle.setOrigin(sf::Vector2f(8.f, 8.f));
        circle.setPosition(to);
        circle.setFillColor(Color);

        window.draw(circle);

        from = to;
    }
}

void GAgentCharacter::LoadTextures()
{
    for (const auto &entry: std::filesystem::directory_iterator("Assets/Images/Enemy"))
    {
        if (entry.path().extension() == ".png")
        {
            std::string tileName = entry.path().stem().string();

            if (!m_textures[tileName].loadFromFile(entry.path().string()))
                std::cerr << "Error loading : " << entry.path() << std::endl;
        }
    }

    for (const auto &[name, texture]: m_textures)
    {
        if (m_textures.find(name) == m_textures.end())
            continue;

        m_sprites[name] = std::make_unique<sf::Sprite>(m_textures[name]);
        m_sprites[name]->setScale(Transform->GetScale());
        sf::FloatRect bounds = m_sprites[name]->getLocalBounds();
        m_sprites[name]->setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
    }
}

void GAgentCharacter::SetWaypoints()
{
    waypoints.push_back(sf::Vector2f(1000.f, 550.f));
    waypoints.push_back(sf::Vector2f(975.f, 100.f));
    waypoints.push_back(sf::Vector2f(730.f, 100.f));
    waypoints.push_back(sf::Vector2f(625.f, 375.f));
    waypoints.push_back(sf::Vector2f(500.f, 550.f));
    waypoints.push_back(sf::Vector2f(250.f, 325.f));
    waypoints.push_back(sf::Vector2f(135.f, 350.f));
    waypoints.push_back(sf::Vector2f(150.f, 425.f));
    waypoints.push_back(sf::Vector2f(275.f, 375.f));
    waypoints.push_back(sf::Vector2f(275.f, 575.f));
    waypoints.push_back(sf::Vector2f(650.f, 575.f));
    waypoints.push_back(sf::Vector2f(875.f, 575.f));
}
