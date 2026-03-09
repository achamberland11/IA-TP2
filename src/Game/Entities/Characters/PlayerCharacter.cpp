//
// Created by Antoine on 2026-03-09.
//

#include "PlayerCharacter.h"

GPlayerCharacter::GPlayerCharacter() {
    Name = "PlayerCharacter";
    Transform->SetPosition(sf::Vector2f(50, 50));
    Transform->SetScale(sf::Vector2f(1, 1));
    Renderer->SetColor(sf::Color::Cyan);

    Controller = new GPlayerController(this);
}

GPlayerCharacter::~GPlayerCharacter() {
}

void GPlayerCharacter::Start() {
    GCharacter::Start();
}

void GPlayerCharacter::Update(float dt) {
    GCharacter::Update(dt);
}
