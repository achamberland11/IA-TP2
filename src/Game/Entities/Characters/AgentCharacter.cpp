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
	std::string oldSpriteName = Renderer->GetSpriteName();
	sf::Vector2f direction;
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
