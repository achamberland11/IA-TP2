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

    tirenessTimer += dt;
    if (tirenessTimer >= 3.f)
    {
        Owner->IncreaseTireness(1);
        tirenessTimer = 0.f;
    }
}

void PatrolGoal::Terminate()
{
    bActive = false;
}

float PatrolGoal::CalculateUtility()
{
    // Score élevé si pas d'alerte, faible sinon
    // TODO: Intégrer le Blackboard pour le ifelse
    return 80.f;
}

float PatrolGoal::CalculateCost()
{
    // Coût faible.
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
    Owner->ResetTireness();
    bActive = false;
    bFinished = true;
}

float TakeBreakGoal::CalculateUtility()
{
    // Score modéré, variable selon la fatigue/temps écoulé
    float baseUtility = 10.f;
    return std::min(baseUtility * Owner->GetTireness(), 100.f);
}

float TakeBreakGoal::CalculateCost()
{
    // Coût faible-modéré (dépend de la distance)
    float baseCost = 8.f;
    return baseCost * (distToBreakRoom * 0.05f);
}

/**
 * Respond To Alert Goal
 */
void RespondToAlertGoal::Activate()
{
    bActive = true;
    bFinished = false;
    Owner->IncreaseTireness(4);
}

void RespondToAlertGoal::Execute(float dt)
{
}

void RespondToAlertGoal::Terminate()
{
}

float RespondToAlertGoal::CalculateUtility()
{
    // Très haut score tant que l'alerte est active
    // TODO: Intégrer le blackboard pour l'alerte et le ifelse
    return 0;
}

float RespondToAlertGoal::CalculateCost()
{
    // Coût modéré
    return 15.f;
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
    // Score maximal si l'intrus est proche ou visible
    if (Owner->IsPlayerVisible())
        return 250.f;
    return 0;
}

float InterceptGoal::CalculateCost()
{
    // Coût élevé
    return 30.f;
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
    // Score faible mais positif lorsque hors zone
    if (Owner->IsInRoom())
        return -10.f;
    else
        return 75.f;
}

float ReturnGoal::CalculateCost()
{
    // Coût faible-modéré
    return 8.f;
}
