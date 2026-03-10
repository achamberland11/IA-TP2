#include "AgentCharacter.h"
#include <iostream>
#include <SFML/Graphics/CircleShape.hpp>

#include "Game/Controllers/AgentController.h"

GAgentCharacter::GAgentCharacter()
{
	Name = "AgentCharacter";
	Transform->SetPosition(sf::Vector2f(625, 150));
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

	MovementSpeed = 50.f; // Agent is slower
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

	std::string oldSpriteName = Renderer->GetSpriteName();

	GAgentController* agent = dynamic_cast<GAgentController*>(Controller);

	bool flip = agent->GetDirection().x < 0.f;

	//Flip le sprite si l'agent va a gauche.
	Transform->SetScale(sf::Vector2f(flip ? -std::abs(Transform->GetScale().x) : std::abs(Transform->GetScale().x), Transform->GetScale().y));

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

void GAgentCharacter::DrawDebug(sf::RenderWindow& window)
{
	const sf::Color Color(180, 100, 220, 255);
	const float LineTickness = 3.f;

	sf::Vector2f from = Transform->GetPosition();
	sf::Vector2f prevDir(0.f, 0.f);

	GAgentController* agent = dynamic_cast<GAgentController*>(Controller);

	for (int i = agent->GetCurrentTarget(); i < agent->GetTargets().size(); i++) {
		sf::Vector2f to = agent->GetTargets()[i];
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

		if (i > (int)agent->GetCurrentTarget() && dot < 0.99f && (prevDir.x != 0.f || prevDir.y != 0.f)) {
			sf::CircleShape circle(4.f);
			circle.setOrigin(sf::Vector2f(4.f, 4.f));
			circle.setPosition(from);
			circle.setFillColor(Color);

			window.draw(circle);
		}

		from = to;
		prevDir = dir;
	}
}

void GAgentCharacter::LoadTextures()
{
	for (const auto& entry : std::filesystem::directory_iterator("Assets/Images/Enemy")) {
		if (entry.path().extension() == ".png") {
			std::string tileName = entry.path().stem().string();

			if (!m_textures[tileName].loadFromFile(entry.path().string()))
				std::cerr << "Error loading : " << entry.path() << std::endl;
		}
	}

	for (const auto& [name, texture] : m_textures) {
		if (m_textures.find(name) == m_textures.end())
			continue;

		m_sprites[name] = std::make_unique<sf::Sprite>(m_textures[name]);
		m_sprites[name]->setScale(Transform->GetScale());
		sf::FloatRect bounds = m_sprites[name]->getLocalBounds();
		m_sprites[name]->setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
	}
}
