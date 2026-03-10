#include "AgentCharacter.h"
#include <iostream>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

#include "Game/Game.h"
#include "Game/World.h"
#include "Game/Controllers/AgentController.h"
#include "Game/Map/AStar.h"

GAgentCharacter::GAgentCharacter()
{
    Name = "AgentCharacter";
    Transform->SetPosition(sf::Vector2f(1000, 550));
    Transform->SetScale(sf::Vector2f(1, 1));
    Renderer->SetColor(sf::Color::Magenta);

    FSM = new GFSMComponent(this);
    AddComponent(FSM);

    AgentController = new GAgentController(this);
    Controller = AgentController;

    // Initialize vision component
    VisionComponent = new GConeVisionComponent(this);
    SetupVisionComponent(150.f, 90.f);
    AddComponent(VisionComponent);

    LoadTextures();

    auto it = Sprites.find("E_F1");
    if (it != Sprites.end() && it->second)
        Renderer->SetSprite(*it->second, "E_F1");
    else
        std::cerr << "Error : E_F1 sprite not found" << std::endl;

    MovementSpeed = 50.f; // Agent is slower
    SetPatrolWaypoints();
}

GAgentCharacter::~GAgentCharacter() {}

void GAgentCharacter::Start()
{
    GCharacter::Start();
}

void GAgentCharacter::Update(float dt)
{
    GCharacter::Update(dt);

    // Sprite rendering
    std::string oldSpriteName = Renderer->GetSpriteName();

    bool flip = Direction.x < 0.f;

    //Flip le sprite si l'agent va a gauche.
    Transform->SetScale(sf::Vector2f(flip ? -std::abs(Transform->GetScale().x) : std::abs(Transform->GetScale().x),
                                     Transform->GetScale().y));

    SpriteTimer += dt;

    if (SpriteTimer > SpriteDuration && !oldSpriteName.empty())
    {
        std::string spriteName = (oldSpriteName.back() == '1') ? "E_F2" : "E_F1";

        auto it = Sprites.find(spriteName);
        if (it != Sprites.end() && it->second)
            Renderer->SetSprite(*it->second, spriteName);

        SpriteTimer = 0;
    }

    // Update vision direction based on movement
    if (VisionComponent && Velocity != sf::Vector2f(0.f, 0.f))
    {
        float facingAngle = std::atan2(Velocity.y, Velocity.x) * 180.f / 3.14159265359f;
        VisionComponent->SetDirection(facingAngle);
    }

    // Debug vision

    if (VisionComponent && AgentController->GetPlayer())
    {
        if (VisionComponent->CanSeeEntity(AgentController->GetPlayer()))
            std::cout << "Player detected!" << std::endl;
    }

    SetVelocity(ComputeSteering(dt));

    if (IsCollidingWith(AgentController->GetPlayer(), 8.f))
    {
        std::cout << "Agent collided with player! Exiting program.\n";
        std::exit(0);
    }
}

void GAgentCharacter::Render(sf::RenderWindow &window)
{
    GEntity::Render(window);
    DrawDebug(window);

    // @TODO temporaire, devrait pas être à chaque rendu comme ça
    /*if (FSM && FSM->GetCurrentState())
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
    }*/
}

void GAgentCharacter::DrawDebug(sf::RenderWindow &window)
{
    const sf::Color Color(180, 100, 220, 255);
    const float LineTickness = 3.f;

    sf::Vector2f from = Transform->GetPosition();
    sf::Vector2f prevDir(0.f, 0.f);


    for (int i = WaypointIndex; i < Waypoints.size(); i++)
    {
        sf::Vector2f to = Waypoints[i];
        sf::Vector2f diff = to - from;
        float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        sf::Vector2f dir = diff / length;

        sf::RectangleShape line(sf::Vector2f(length, LineTickness));
        line.setFillColor(Color);
        line.setOrigin(sf::Vector2f(0.f, LineTickness / 2.f));
        line.setPosition(from);
        line.setRotation(sf::radians(std::atan2(diff.y, diff.x)));

        window.draw(line);

        float dot = prevDir.x * dir.x + prevDir.y * dir.y;

        if (i > WaypointIndex && dot < 0.99f && (prevDir.x != 0.f || prevDir.y != 0.f))
        {
            sf::CircleShape circle(4.f);
            circle.setOrigin(sf::Vector2f(4.f, 4.f));
            circle.setPosition(from);
            circle.setFillColor(Color);

            window.draw(circle);
        }

        from = to;
        prevDir = dir;

    }

    // Draw vision cone
    if (VisionComponent)
    {
        const sf::Color ColorVisionCone(255, 0, 0, 180);
        float range = VisionComponent->GetVisionRange();
        float angle = VisionComponent->GetVisionAngle();
        float direction = VisionComponent->GetDirection();

        sf::Vector2f origin = Transform->GetPosition();
        float halfAngle = angle / 2.f;

        // Left line of vision cone
        sf::RectangleShape leftLine(sf::Vector2f(range, LineTickness));
        leftLine.setFillColor(ColorVisionCone);
        leftLine.setOrigin(sf::Vector2f(0.f, LineTickness / 2.f));
        leftLine.setPosition(origin);
        leftLine.setRotation(sf::degrees(direction - halfAngle));
        window.draw(leftLine);

        // Right line of vision cone
        sf::RectangleShape rightLine(sf::Vector2f(range, LineTickness));
        rightLine.setFillColor(ColorVisionCone);
        rightLine.setOrigin(sf::Vector2f(0.f, LineTickness / 2.f));
        rightLine.setPosition(origin);
        rightLine.setRotation(sf::degrees(direction + halfAngle));
        window.draw(rightLine);

        // End line of cone
        const float degToRad = 3.14159265359f / 180.f;
        const float leftAngleRad = (direction - halfAngle) * degToRad;
        const float rightAngleRad = (direction + halfAngle) * degToRad;

        sf::Vector2f leftLineEndPoint(
            origin.x + std::cos(leftAngleRad) * range,
            origin.y + std::sin(leftAngleRad) * range
        );
        sf::Vector2f rightLineEndPoint(
            origin.x + std::cos(rightAngleRad) * range,
            origin.y + std::sin(rightAngleRad) * range
        );

        sf::Vector2f endDiff = rightLineEndPoint - leftLineEndPoint;
        float endLength = std::sqrt(endDiff.x * endDiff.x + endDiff.y * endDiff.y);

        sf::RectangleShape endLine(sf::Vector2f(endLength, LineTickness));
        endLine.setFillColor(ColorVisionCone);
        endLine.setOrigin(sf::Vector2f(0.f, LineTickness / 2.f));
        endLine.setPosition(leftLineEndPoint);
        endLine.setRotation(sf::radians(std::atan2(endDiff.y, endDiff.x)));
        window.draw(endLine);
    }
}

void GAgentCharacter::SetWaypoints(const std::vector<sf::Vector2f>& waypoints)
{
    std::cout << "Waypoints set!" << std::endl;
    Waypoints = waypoints;
    WaypointIndex = 0;
}

void GAgentCharacter::LoadTextures()
{
    for (const auto &entry: std::filesystem::directory_iterator("Assets/Images/Enemy"))
    {
        if (entry.path().extension() == ".png")
        {
            std::string tileName = entry.path().stem().string();

            if (!Textures[tileName].loadFromFile(entry.path().string()))
                std::cerr << "Error loading : " << entry.path() << std::endl;
        }
    }

    for (const auto &[name, texture]: Textures)
    {
        if (Textures.find(name) == Textures.end())
            continue;

        Sprites[name] = std::make_unique<sf::Sprite>(Textures[name]);
        Sprites[name]->setScale(Transform->GetScale());
        sf::FloatRect bounds = Sprites[name]->getLocalBounds();
        Sprites[name]->setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
    }
}

void GAgentCharacter::SetupVisionComponent(float range, float angle)
{
    if (VisionComponent)
    {
        VisionComponent->SetVisionRange(range);
        VisionComponent->SetVisionAngle(angle);
        VisionComponent->SetDirection(0.f); // Can be set based on character's facing direction
    }
}


void GAgentCharacter::SetPatrolWaypoints()
{
    PatrolPoints.push_back(sf::Vector2f(1000.f, 550.f));
    PatrolPoints.push_back(sf::Vector2f(975.f, 100.f));
    PatrolPoints.push_back(sf::Vector2f(730.f, 100.f));
    PatrolPoints.push_back(sf::Vector2f(625.f, 375.f));
    PatrolPoints.push_back(sf::Vector2f(500.f, 550.f));
    PatrolPoints.push_back(sf::Vector2f(250.f, 325.f));
    PatrolPoints.push_back(sf::Vector2f(135.f, 350.f));
    PatrolPoints.push_back(sf::Vector2f(150.f, 425.f));
    PatrolPoints.push_back(sf::Vector2f(275.f, 375.f));
    PatrolPoints.push_back(sf::Vector2f(275.f, 575.f));
    PatrolPoints.push_back(sf::Vector2f(650.f, 575.f));
    PatrolPoints.push_back(sf::Vector2f(875.f, 575.f));
}

sf::Vector2f GAgentCharacter::ComputeSteering(float dt)
{
    GPlayerCharacter* player = AgentController->GetPlayer();
	sf::Vector2f agentPos = Transform->GetPosition();


	if (FSM->GetCurrentState() == AgentChaseState::Instance()) {
        if (!player)
            return { 0.f, 0.f };

	    sf::Vector2f playerPos = player->GetTransformComponent()->GetPosition();
	    sf::Vector2f playerVel = player->GetVelocity();

	    //Calculate prediction time
	    sf::Vector2f toPlayer = playerPos - agentPos;
	    float distance = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);

		//Predict
		float predictionTime = (MovementSpeed > 0.f) ? distance / MovementSpeed : 0.f;

		const float MaxPrediction = 0.5f;

		if (predictionTime > MaxPrediction)
			predictionTime = MaxPrediction;

		sf::Vector2 predictedPos = playerPos + playerVel * predictionTime;

		Direction = predictedPos - agentPos;
		float dist = std::sqrt(Direction.x * Direction.x + Direction.y * Direction.y);

		if (dist > 0.f)
			Direction /= dist;
		else
			return { 0.f, 0.f };

		//Arrive behavior
		float speed = MovementSpeed;
		if (distance < SlowingRadius)
			speed *= distance / SlowingRadius;

		return Direction * speed;
	}
	//Patrol or Return
	else {
		if (Waypoints.empty() || WaypointIndex >= Waypoints.size())
			return { 0.f, 0.f };

		sf::Vector2f targetPos = Waypoints[WaypointIndex];
		Direction = targetPos - agentPos;
		float distance = std::sqrt(Direction.x * Direction.x + Direction.y * Direction.y);

		if (distance < 2.f)
			if (WaypointIndex + 1 < Waypoints.size())
				WaypointIndex++;
			else
				return { 0.f, 0.f };

		Direction /= distance;

		//Arrive behavior
		float speed = MovementSpeed;
		if (distance < SlowingRadius)
			speed *= distance / SlowingRadius;

		return Direction * speed;
	}
}
