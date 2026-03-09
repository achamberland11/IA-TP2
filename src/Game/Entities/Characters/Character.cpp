//
// Created by Antoine on 2026-03-09.
//

#include "Character.h"


GCharacter::GCharacter() {
    Name = "Character";
    Transform->SetPosition(sf::Vector2f(0, 0));
    Transform->SetScale(sf::Vector2f(1, 1));

    Controller = nullptr;
}

GCharacter::~GCharacter() {
    if (Controller) delete Controller;
}

void GCharacter::Start() {
    GEntity::Start();

    if (Controller) Controller->Start();
}

void GCharacter::Update(float dt) {
    GEntity::Update(dt);

    if (Controller) Controller->Update(dt);

    Transform->SetPosition(Transform->GetPosition() + Velocity * dt);
}

void GCharacter::SetVelocity(const sf::Vector2f &velocity) {
    Velocity = velocity;
}

void GCharacter::StopMoving() {
    Velocity = sf::Vector2f(0, 0);
}
