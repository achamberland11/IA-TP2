//
// Created by Antoine on 2026-03-09.
//

#pragma once
#include "Character.h"
#include "../../Controllers/PlayerController.h"
#include "SFML/Window/Event.hpp"


class GPlayerCharacter : public GCharacter
{
public:
    GPlayerCharacter();
    ~GPlayerCharacter() override;

    void Start() override;
    void Update(float dt) override;

private:
};
