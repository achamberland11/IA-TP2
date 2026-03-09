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

    GTransformComponent(GEntity* owner) : GComponent(owner)
    {
        Name = "Transform";
    }

    void Start() override {}
    void Update(float deltaSeconds) override {}

    sf::Vector2f GetPosition() const { return Position; }
    sf::Vector2f GetRotation() const { return Rotation; }
    sf::Vector2f GetScale() const { return Scale; }

    void SetPosition(const sf::Vector2f& pos) { Position = pos; }
    void SetRotation(const sf::Vector2f& rot) { Rotation = rot; }
    void SetScale(const sf::Vector2f& scale) { Scale = scale; }

private:
    sf::Vector2f Position = {0,0};
    sf::Vector2f Rotation = {0,0};
    sf::Vector2f Scale = {1,1};
};
