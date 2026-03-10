//
// Created by dreux on 09-03-26.
//

#pragma once
#include "Component.h"
#include "SFML/System/Vector2.hpp"
#include <cmath>
#include <vector>

class GEntity;

class GConeVisionComponent : public GComponent
{
public:
    GENERATE_COMPONENT(false, false, false)

    GConeVisionComponent(GEntity* owner) : GComponent(owner)
    {
        Name = "ConeVision";
    }

    void Start() override;
    void Update(float deltaSeconds) override;

    // Configuration
    void SetVisionRange(float range) { VisionRange = range; }
    void SetVisionAngle(float angle) { VisionAngle = angle; } // in degrees
    void SetDirection(float degrees) { ViewDirection = degrees; } // in degrees

    // Getters
    float GetVisionRange() const { return VisionRange; }
    float GetVisionAngle() const { return VisionAngle; }
    float GetDirection() const { return ViewDirection; }

    // Detection
    bool CanSeeEntity(GEntity* target) const;

private:
    // Helper methods
    sf::Vector2f GetOwnerPosition() const;
    float GetAngleBetween(const sf::Vector2f& from, const sf::Vector2f& to) const;
    bool IsAngleWithinCone(float targetAngle, float coneAngle) const;

    // Vision parameters
    float VisionRange = 100.f;
    float VisionAngle = 60.f;
    float ViewDirection = 0.f;
};
