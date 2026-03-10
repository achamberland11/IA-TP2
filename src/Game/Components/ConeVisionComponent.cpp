//
// Created by dreux on 09-03-26.
//

#include "ConeVisionComponent.h"

#include "../Entities/Entity.h"
#include "TransformComponent.h"
#include "Game/Game.h"
#include <cmath>

const float PI = 3.14159f;

void GConeVisionComponent::Start()
{
}

void GConeVisionComponent::Update(float deltaSeconds)
{
}

sf::Vector2f GConeVisionComponent::GetOwnerPosition() const
{
    if (!Owner) return sf::Vector2f(0, 0);

    GTransformComponent* transform = Owner->GetTransformComponent();
    return transform ? transform->GetPosition() : sf::Vector2f(0, 0);
}

float GConeVisionComponent::GetAngleBetween(const sf::Vector2f& from, const sf::Vector2f& to) const
{
    float dx = to.x - from.x;
    float dy = to.y - from.y;

    // Calculate angle in radians and convert to degrees
    float angle = std::atan2(dy, dx) * 180.f / PI;

    // Normalize to 0-360 range
    if (angle < 0) angle += 360.f;

    return angle;
}

bool GConeVisionComponent::IsAngleWithinCone(float targetAngle, float coneAngle) const
{
    // Normalize angles to 0-360 range
    float normalizedViewDir = std::fmod(ViewDirection, 360.f);
    if (normalizedViewDir < 0) normalizedViewDir += 360.f;

    float normalizedTargetAngle = std::fmod(targetAngle, 360.f);
    if (normalizedTargetAngle < 0) normalizedTargetAngle += 360.f;

    // Calculate the half-angle of the cone
    float halfAngle = coneAngle / 2.f;

    // Calculate the difference between target angle and view direction
    float angleDiff = normalizedTargetAngle - normalizedViewDir;

    // Normalize the difference to -180 to 180 range
    while (angleDiff > 180.f) angleDiff -= 360.f;
    while (angleDiff < -180.f) angleDiff += 360.f;

    // Check if the target angle is within the cone
    return std::abs(angleDiff) <= halfAngle;
}

bool GConeVisionComponent::CanSeeEntity(GEntity* target) const
{
    if (!target || !Owner) return false;

    GTransformComponent* targetTransform = target->GetTransformComponent();
    if (!targetTransform) return false;

    sf::Vector2f ownerPos = GetOwnerPosition();
    sf::Vector2f targetPos = targetTransform->GetPosition();
    sf::Vector2f directionToPlayer = targetPos - ownerPos;
    float distance = directionToPlayer.length();

    if (distance > VisionRange) return false;

    float targetAngle = GetAngleBetween(ownerPos, targetPos);
    if (!IsAngleWithinCone(targetAngle, VisionAngle)) return false;

    GGame* game = GGame::GetInstance();
    if (!game || !game->GetMap() || distance <= 0.f) return true;

    const float stepSize = 5.f;
    for (float step = 0.f; step < distance; step += stepSize)
    {
        sf::Vector2f checkPos = ownerPos + directionToPlayer * (step / distance);
        sf::Vector2f gridPos = game->GetMap()->WorldToGrid(checkPos);

        int row = static_cast<int>(gridPos.y);
        int col = static_cast<int>(gridPos.x);
        if (game->GetMap()->BlocksVision(row, col))
            return false;
    }

    return true;
}
