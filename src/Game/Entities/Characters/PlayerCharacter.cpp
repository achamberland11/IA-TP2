//
// Created by Antoine on 2026-03-09.
//

#include "PlayerCharacter.h"
#include <iostream>

#include "Game/Game.h"

GPlayerCharacter::GPlayerCharacter() {
    Name = "PlayerCharacter";
    Transform->SetPosition(sf::Vector2f(250, 150));
    Transform->SetScale(sf::Vector2f(1, 1));
    Renderer->SetColor(sf::Color::Cyan);

    Controller = new GPlayerController(this);

	LoadTextures();

	auto it = Sprites.find("C_Front");
	if (it != Sprites.end() && it->second)
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

	if (!IsMoving()) {
		auto it = Sprites.find(spriteName);
		if (it != Sprites.end() && it->second)
			Renderer->SetSprite(*it->second, spriteName);
	}

	if (IsMoving() && !oldSpriteName.empty())
	{
		spriteName += "Walk";

		SpriteTimer += dt;

		if (SpriteTimer > SpriteDuration)
		{
			spriteName += oldSpriteName.back() == '1' ? '2' : '1';

			auto it = Sprites.find(spriteName);
			if (it != Sprites.end() && it->second)
				Renderer->SetSprite(*it->second, spriteName);

			SpriteTimer = 0;
		}
	}

	if (bRadioJamming)
	{
		RadioJamTimer += dt;
		if (RadioJamTimer >= RadioJamDuration)
		{
			bRadioJamming = false;
			RadioJamTimer = 0.f;
			GGame::GetInstance()->GetGlobalBlackboard()->ResetInterferenceChance();
		}
	}

    GCharacter::Update(dt);
}

void GPlayerCharacter::LoadTextures() {
	for (const auto& entry : std::filesystem::directory_iterator("Assets/Images/Character")) {
		if (entry.path().extension() == ".png") {
			std::string tileName = entry.path().stem().string();

			if (!Textures[tileName].loadFromFile(entry.path().string()))
				std::cerr << "Error loading : " << entry.path() << std::endl;
		}
	}

	for (const auto& [name, texture] : Textures) {
		if (Textures.find(name) == Textures.end())
			continue;

		Sprites[name] = std::make_unique<sf::Sprite>(Textures[name]);
		Sprites[name]->setScale(Transform->GetScale());
		sf::FloatRect bounds = Sprites[name]->getLocalBounds();
		Sprites[name]->setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
	}
}

void GPlayerCharacter::TryRadioJamming()
{
	if (bRadioJamming)
		return;

	GGame *game = GGame::GetInstance();
	if (!game)
		return;

	GlobalBlackboard *GlobalBB = game->GetGlobalBlackboard();
	if (!GlobalBB)
		return;

	GlobalBB->ChangeInterferenceChance(100.f);
	bRadioJamming = true;
}