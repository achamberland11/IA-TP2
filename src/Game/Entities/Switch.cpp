#include "Switch.h"
#include <iostream>

GSwitch::GSwitch()
{
	Name = "Switch";
	Transform->SetScale(sf::Vector2f(1.5f, 1.5f));
	Renderer->SetColor(sf::Color::Cyan);

	LoadTextures();

	auto it = Sprites.find("SwitchOff");
	if (it != Sprites.end() && it->second)
		Renderer->SetSprite(*it->second, "SwitchOff");
	else
		std::cerr << "Error : SwitchOff sprite not found" << std::endl;
}

GSwitch::~GSwitch()
{
}

void GSwitch::Start()
{
	GEntity::Start();
}

void GSwitch::Update(float dt)
{
	auto it = Sprites.find(Renderer->GetSpriteName());
	if (it != Sprites.end() && it->second)
		if (Status == SwitchStatus::On)
			Renderer->SetSprite(*it->second, "SwitchOn");
		else
			Renderer->SetSprite(*it->second, "SwitchOff");


	GEntity::Update(dt);
}

void GSwitch::Interact()
{
	if (Listener) {
		std::cout << "Switch status: ";

		if (Status == SwitchStatus::On) {
			Status = SwitchStatus::Off;
			std::cout << "Off \n";
			Listener->OnSwitchToggled(false);
		}
		else {
			Status = SwitchStatus::On;
			std::cout << "On \n";
			Listener->OnSwitchToggled(true);
		}
	}
}

void GSwitch::LoadTextures()
{
	for (const auto& entry : std::filesystem::directory_iterator("Assets/Images/Switch")) {
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
