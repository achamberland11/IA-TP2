//
// Created by Antoine on 2026-03-09.
//

#include "Character.h"

#include <iostream>

#include "Game/Game.h"


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

    if (GGame::GetInstance()->GetMap() && Velocity != sf::Vector2f(0, 0)) {
        sf::Vector2f currentPos = Transform->GetPosition();
        sf::Vector2f finalPos = currentPos;

        // Try X axis
        sf::Vector2f testX(currentPos.x + Velocity.x * dt, currentPos.y);
        sf::Vector2f gridX = GGame::GetInstance()->GetMap()->WorldToGrid(testX);
        if (GGame::GetInstance()->GetMap()->IsWalkable(gridX.y, gridX.x))
            finalPos.x = testX.x;

        // Try Y axis
        sf::Vector2f testY(currentPos.x, currentPos.y + Velocity.y * dt);
        sf::Vector2f gridY = GGame::GetInstance()->GetMap()->WorldToGrid(testY);
        if (GGame::GetInstance()->GetMap()->IsWalkable(gridY.y, gridY.x))
            finalPos.y = testY.y;

        Transform->SetPosition(finalPos);
    } else {
        Transform->SetPosition(Transform->GetPosition() + Velocity * dt);
    }
}

void GCharacter::SetVelocity(const sf::Vector2f &velocity) {
    Velocity = velocity;
    if (velocity != sf::Vector2f(0, 0)) bMoving = true;
}

void GCharacter::StopMoving() {
    Velocity = sf::Vector2f(0, 0);
    bMoving = false;
    // std::cout << "StopMoving" << std::endl;
}
