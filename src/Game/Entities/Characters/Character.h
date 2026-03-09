//
// Created by Antoine on 2026-03-09.
//

#pragma once
#include "../Entity.h"
#include "../../Controllers/Controller.h"
#include "Game/Map/Map.h"


class GCharacter : public GEntity
{
public:
    GCharacter();
    ~GCharacter() override;

    void Start() override;
    void Update(float dt) override;

    void SetController(GController* controller) { Controller = controller; }
    GController* GetController() const { return Controller; }

    void SetVelocity(const sf::Vector2f& velocity);
    void StopMoving();

    float GetMovementSpeed() const { return MovementSpeed; }
    bool IsMoving() const { return bMoving; }

protected:
    GController* Controller;
    sf::Vector2f Velocity = sf::Vector2f(0, 0);
    float MovementSpeed = 100.f;
    bool bMoving = false;
};
