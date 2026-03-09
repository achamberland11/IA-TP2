//
// Created by Antoine on 2026-03-09.
//

#pragma once
#include "Controller.h"


class GPlayerController : public GController
{
public:
    GPlayerController(GCharacter* owner) : GController(owner) {}
    ~GPlayerController() override = default;

    void Start() override;
    void Update(float dt) override;

    void HandleEvent(const sf::Event& event) override;

private:
    bool bMoveUp = false;
    bool bMoveDown = false;
    bool bMoveLeft = false;
    bool bMoveRight = false;
};
