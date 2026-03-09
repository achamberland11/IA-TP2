#include "AgentCharacter.h"
#include <iostream>
#include <SFML/Graphics/CircleShape.hpp>

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
	//Temporaire
	if (!bFinished && !targets.empty()) {

		sf::Vector2f position = Transform->GetPosition();
		sf::Vector2f direction = targets[currentTarget] - position;
		float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

		if (distance <= 2) {
			if (currentTarget + 1 < targets.size())
				currentTarget++;
			else {
				bFinished = true;
				SetVelocity({ 0.f, 0.f });
			}
		}
		else {
			direction /= distance;
			SetVelocity(direction * static_cast<float>(100));
		}
	}

	std::string oldSpriteName = Renderer->GetSpriteName();
	//	sf::Vector2f direction;
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
	const sf::Color Color(180, 100, 220, 180);
	const float LineTickness = 5.f;

	sf::Vector2f from = Transform->GetPosition();

	for (int i = currentTarget; i < targets.size(); i++) {
		sf::Vector2f to = targets[i];
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
