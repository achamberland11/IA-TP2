//
// Created by Antoine on 2026-03-07.
//

#pragma once
#include "Component.h"
#include "SFML/Network/SocketHandle.hpp"
#include "SFML/System/Vector2.hpp"


class GTransformComponent : public GComponent
{
public:
    GENERATE_COMPONENT(false, false, false)

    GTransformComponent()
    {
        Name = "Transform";
    }

    void Start() override {}
    void Update(float deltaSeconds) override {}

    bool CanDuplicate() const override { return bCanDuplicate; }
    bool CanBeDisabled() const override { return bCanBeDisabled; }
    bool CanBeDeleted() const override { return bCanBeDeleted; }

    sf::Vector2f Position = {0,0};
    sf::Vector2f Rotation = {0,0};
    sf::Vector2f Scale = {1,1};
};
