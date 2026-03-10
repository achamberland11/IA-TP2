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

        float halfWidth = 4.f * Transform->GetScale().x;
        float halfHeight = 8.f * Transform->GetScale().y;

        // Try X axis
        sf::Vector2f testX(currentPos.x + Velocity.x * dt, currentPos.y);
        sf::Vector2f gridXL = GGame::GetInstance()->GetMap()->WorldToGrid({ testX.x - halfWidth, testX.y + halfHeight });
        sf::Vector2f gridXR = GGame::GetInstance()->GetMap()->WorldToGrid({ testX.x + halfWidth, testX.y + halfHeight });
        sf::Vector2f gridXC = GGame::GetInstance()->GetMap()->WorldToGrid({ testX.x, testX.y + halfHeight });
        if (GGame::GetInstance()->GetMap()->IsWalkable(gridXL.y, gridXL.x) &&
            GGame::GetInstance()->GetMap()->IsWalkable(gridXR.y, gridXR.x) &&
            GGame::GetInstance()->GetMap()->IsWalkable(gridXC.y, gridXC.x))
            finalPos.x = testX.x;

        // Try Y axis
        sf::Vector2f testY(currentPos.x, currentPos.y + Velocity.y * dt);
        int collisionMargin = 15.f;
        float checkYVert = (Velocity.y > 0.f) ? (testY.y + halfHeight - collisionMargin) : (testY.y - halfHeight + collisionMargin);

        sf::Vector2f gridYL = GGame::GetInstance()->GetMap()->WorldToGrid({ testY.x - halfWidth, testY.y + halfHeight });
        sf::Vector2f gridYR = GGame::GetInstance()->GetMap()->WorldToGrid({ testY.x + halfWidth, testY.y + halfHeight });
        sf::Vector2f gridYC = GGame::GetInstance()->GetMap()->WorldToGrid({ testY.x, testY.y + halfHeight });
        if (GGame::GetInstance()->GetMap()->IsWalkable(gridYL.y, gridYL.x) &&
            GGame::GetInstance()->GetMap()->IsWalkable(gridYR.y, gridYR.x) &&
            GGame::GetInstance()->GetMap()->IsWalkable(gridYC.y, gridYC.x))
        if (GGame::GetInstance()->GetMap()->IsWalkable(gridYL.y, gridYL.x) &&
            GGame::GetInstance()->GetMap()->IsWalkable(gridYR.y, gridYR.x) &&
            GGame::GetInstance()->GetMap()->IsWalkable(gridYC.y, gridYC.x))
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
