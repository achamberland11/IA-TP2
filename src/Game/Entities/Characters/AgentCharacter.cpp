#include "AgentCharacter.h"
#include <iostream>

GAgentCharacter::GAgentCharacter()
{
	Name = "AgentCharacter";
	Transform->SetPosition(sf::Vector2f(200, 100));
	Transform->SetScale(sf::Vector2f(1, 1));
	Renderer->SetColor(sf::Color::Magenta);

	LoadTextures();

	auto it = m_sprites.find("E_F1");
	if (it != m_sprites.end() && it->second)
		Renderer->SetSprite(*it->second, "E_F1");
	else
		std::cerr << "Error : E_F1 sprite not found" << std::endl;
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
	sf::Vector2f position = Transform->GetPosition();
	sf::Vector2f direction = target - position;
	float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	if (distance <= 2) 
		SetVelocity({ 0.f, 0.f });
	else {
		direction /= distance;
		SetVelocity(direction * static_cast<float>(100));
	}

	std::string oldSpriteName = Renderer->GetSpriteName();
	//sf::Vector2f direction;
	bool flip = true;

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

	GCharacter::Update(dt);
}

void GAgentCharacter::DrawDebug(sf::RenderWindow& window)
{
	sf::Vector2f from = Transform->GetPosition();
	sf::Vector2f to = target;
	sf::Vector2f diff = to - from;
	float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);
	float thickness = 5.f;
	sf::Color lineColor (180, 100, 220, 180);
	
	sf::RectangleShape line(sf::Vector2f(length, thickness));
	line.setFillColor(lineColor);
	line.setOrigin(sf::Vector2f(0.f, thickness / 2.f));
	line.setPosition(from);
	line.setRotation(sf::radians(std::atan2(diff.y, diff.x)));

	window.draw(line);
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
