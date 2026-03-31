//
// Created by Antoine on 2026-03-30.
//

#include "AgentGoals.h"

#include "Game/Entities/Characters/AgentCharacter.h"


/**
 * Patrol Goal
 */
void PatrolGoal::Activate()
{
    bActive = true;

    std::vector<sf::Vector2f> patrolPoints = Owner->GetPatrolPoints();
    if (!patrolPoints.empty())
    {
        Owner->GetAgentController()->FindPath(patrolPoints[Owner->GetPatrolIndex()]);
    }
}

void PatrolGoal::Execute(float dt)
{
    std::vector<sf::Vector2f> patrolPoints = Owner->GetPatrolPoints();
    if (patrolPoints.empty()) return;

    if (Owner->GetWaypoints().empty())
    {
        int nextIndex = (Owner->GetPatrolIndex() + 1) % patrolPoints.size();
        Owner->SetPatrolIndex(nextIndex);
        Owner->GetAgentController()->FindPath(patrolPoints[nextIndex]);
    }
}

void PatrolGoal::Terminate()
{
    bActive = false;
}

float PatrolGoal::CalculateUtility()
{
    // Score élevé si pas d'alerte, faible sinon
    // TODO: Intégrer le Blackboard
    return 80.f;
}

float PatrolGoal::CalculateCost()
{
    return 5.f;
}

/**
 * Take Break Goal
 */
void TakeBreakGoal::Activate()
{
    bActive = true;
    bFinished = false;
    CurrentBreakTime = 0.f;

    sf::Vector2f agentPos = Owner->GetTransformComponent()->GetPosition();
    distToBreakRoom = std::sqrt(std::pow(agentPos.x - BreakLocation.x, 2) + std::pow(agentPos.y - BreakLocation.y, 2));

    // TODO: définir la position du breakroom
    BreakLocation = sf::Vector2f(100.f, 100.f);
    Owner->GetAgentController()->FindPath(BreakLocation);
}

void TakeBreakGoal::Execute(float dt)
{
    sf::Vector2f agentPos = Owner->GetTransformComponent()->GetPosition();
    float sqrdDist = std::pow(agentPos.x - BreakLocation.x, 2) + std::pow(agentPos.y - BreakLocation.y, 2);

    if (sqrdDist < 100.f)
    {
        CurrentBreakTime += dt;
        if (CurrentBreakTime >= BreakDuration)
        {
            Terminate();
        }
    }
}

void TakeBreakGoal::Terminate()
{
    bActive = false;
    bFinished = true;
}

float TakeBreakGoal::CalculateUtility()
{
    float baseUtility = 10.f;
    return baseUtility * (std::clamp(BreakDuration, 40.f, 60.f));
}

float TakeBreakGoal::CalculateCost()
{
    float baseCost = 15.f;
    return baseCost * (distToBreakRoom * 0.05f);
}

/**
 * Respond To Alert Goal
 */
void RespondToAlertGoal::Activate()
{
    bActive = true;
    bFinished = false;
}

void RespondToAlertGoal::Execute(float dt)
{
}

void RespondToAlertGoal::Terminate()
{
}

float RespondToAlertGoal::CalculateUtility()
{
    return 0;
}

float RespondToAlertGoal::CalculateCost()
{
    return 0;
}

/**
 * Intercept Goal
 */
void InterceptGoal::Activate()
{
}

void InterceptGoal::Execute(float dt)
{
}

void InterceptGoal::Terminate()
{
}

float InterceptGoal::CalculateUtility()
{
    return 0;
}

float InterceptGoal::CalculateCost()
{
    return 0;
}

/**
 * Return Goal
 */
void ReturnGoal::Activate()
{
    bActive = true;
    bFinished = false;

    std::vector<sf::Vector2f> patrolPoints = Owner->GetPatrolPoints();
    if (!patrolPoints.empty())
    {
        Owner->GetAgentController()->FindPath(patrolPoints[Owner->GetPatrolIndex()]);
    }
}

void ReturnGoal::Execute(float dt)
{
    std::vector<sf::Vector2f> waypoints = Owner->GetWaypoints();
    if (waypoints.empty())
        Terminate();
}

void ReturnGoal::Terminate()
{
    bActive = false;
    bFinished = true;
}

float ReturnGoal::CalculateUtility()
{
    return 0;
}

float ReturnGoal::CalculateCost()
{
    return 0;
}
