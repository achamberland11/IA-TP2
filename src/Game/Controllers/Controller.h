//
// Created by Antoine on 2026-03-09.
//

#pragma once
#include "../../Core/Object.h"
#include "../Entities/Characters/Character.h"
#include "SFML/Window/Event.hpp"


class GController : public GObject
{
public:
    GController(GCharacter* owner) : Owner(owner) {};
    virtual ~GController() = default;

    virtual void Start();
    virtual void Update(float dt);

    virtual void HandleEvent(sf::Event& event) = 0;

protected:
    GCharacter* Owner;
};
