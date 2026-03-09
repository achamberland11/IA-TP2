//
// Created by Antoine on 2026-03-09.
//

#include "PlayerCharacter.h"
#include <iostream>

GPlayerCharacter::GPlayerCharacter() {
    Name = "PlayerCharacter";
    Transform->SetPosition(sf::Vector2f(100, 100));
    Transform->SetScale(sf::Vector2f(1, 1));
    Renderer->SetColor(sf::Color::Cyan);

    Controller = new GPlayerController(this);

	LoadTextures();

	auto it = m_sprites.find("C_Front");
	if (it != m_sprites.end() && it->second)
		Renderer->SetSprite(*it->second, "C_Front");
	else
		std::cerr << "Error : C_Front sprite not found" << std::endl;

}

GPlayerCharacter::~GPlayerCharacter() {
}

void GPlayerCharacter::Start() {
    GCharacter::Start();
}

void GPlayerCharacter::Update(float dt) {
	std::string oldSpriteName = Renderer->GetSpriteName();
	std::string spriteName = "C_";
	std::string direction;

	sf::Vector2f Rotation = Transform->GetRotation();

	if (Rotation.y > 0)
		direction = "Front";
	else if (Rotation.y < 0)
		direction = "Back";
	else if (Rotation.x < 0)
		direction = "Left";
	else if (Rotation.x > 0)
		direction = "Right";

	spriteName += direction;

	/*std::string oldDirection = oldSpriteName.substr(2);
	size_t walkPos = oldDirection.find("Walk");

	if (walkPos != std::string::npos)
		oldDirection = oldDirection.substr(0, walkPos);
	*/

	if (static_cast<GPlayerController*>(Controller)->IsMoving())
	{
		spriteName += "Walk";

		spriteTimer += dt;

		if (spriteTimer > spriteDuration)
		{
			spriteName += (Renderer->GetSpriteName().back() == '1') ? "2" : "1";

			auto it = m_sprites.find(spriteName);
			if (it != m_sprites.end() && it->second)
				Renderer->SetSprite(*it->second, spriteName);

			spriteTimer = 0;
		}
	}

    GCharacter::Update(dt);
}

void GPlayerCharacter::LoadTextures() {
	for (const auto& entry : std::filesystem::directory_iterator("Assets/Images/Character")) {
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