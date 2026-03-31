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
    if (Blackboard && Blackboard->ListenToRadio().bPlayerSeen)
        return BaseUtility;
    return MaxUtility;
}

float PatrolGoal::CalculateCost()
{
    // Coût faible.
    return Cost;
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
    return std::min(BaseUtility * Owner->GetTireness(), MaxUtility);
}

float TakeBreakGoal::CalculateCost()
{
    // Coût faible-modéré (dépend de la distance)
    return Cost * (distToBreakRoom * 0.05f);
}

/**
 * Respond To Alert Goal
 */
void RespondToAlertGoal::Activate()
{
    Owner->IncreaseTireness(4);
    bActive = true;
    bFinished = false;
}

void RespondToAlertGoal::Execute(float dt)
{
    if (Owner->IsPlayerVisible())
        Owner->GetAgentController()->FindPath(Blackboard->ListenToRadio().PlayerLastPosition);

    sf::Vector2f agentPos = Owner->GetTransformComponent()->GetPosition();
    float sqrdDist = std::pow(agentPos.x - AlertLocation.x, 2) + std::pow(agentPos.y - AlertLocation.y, 2);
    if (sqrdDist < 1000.f && !Owner->IsPlayerVisible() && !Blackboard->ListenToRadio().bPlayerSeen)
        Terminate();
}

void RespondToAlertGoal::Terminate()
{
    bActive = false;
    bFinished = true;
}

float RespondToAlertGoal::CalculateUtility()
{
    // Très haut score tant que l'alerte est active
    if (Blackboard && Blackboard->ListenToRadio().bPlayerSeen)
    {
        return MaxUtility;
        float distToReportSqrd = Blackboard->GetDistToReportingAgent(Owner->GetTransformComponent()->GetPosition(),
                                                                 Owner->GetAgentID());
        if (distToReportSqrd != -1.f)
            if (distToReportSqrd <= Blackboard->RadioMaxDist * Blackboard->RadioMaxDist)
                return MaxUtility;
    }
    return BaseUtility;
}

float RespondToAlertGoal::CalculateCost()
{
    // Coût modéré
    return Cost;
}

/**
 * Intercept Goal
 */
void InterceptGoal::Activate()
{
    bActive = true;
    bFinished = false;
    Owner->IncreaseTireness(6);
}

void InterceptGoal::Execute(float dt)
{
    // TODO: Intercept movement
    if (Owner->IsPlayerVisible())
        Owner->GetAgentController()->FindPath(
            Owner->GetAgentController()->GetPlayer()->GetTransformComponent()->GetPosition());
    else
        Owner->GetAgentController()->FindPath(Blackboard->ListenToRadio().PlayerLastPosition);

    sf::Vector2f agentPos = Owner->GetTransformComponent()->GetPosition();
    float sqrdDist = std::pow(agentPos.x - LastKnownPosition.x, 2) + std::pow(agentPos.y - LastKnownPosition.y, 2);
    if (sqrdDist < 1000.f && !Owner->IsPlayerVisible() && !Blackboard->ListenToRadio().bPlayerSeen)
        Terminate();
}

void InterceptGoal::Terminate()
{
    bActive = false;
    bFinished = true;
}

float InterceptGoal::CalculateUtility()
{
    // Score maximal si l'intrus est proche ou visible
    if (Owner->IsPlayerVisible())
        return MaxUtility;

    if (Blackboard && Blackboard->ListenToRadio().bPlayerSeen)
    {
        return MaxUtility;
        float distToReportSqrd = Blackboard->GetDistToReportingAgent(Owner->GetTransformComponent()->GetPosition(),
                                                                     Owner->GetAgentID());
        if (distToReportSqrd != -1.f)
            if (distToReportSqrd <= Blackboard->RadioMaxDist * Blackboard->RadioMaxDist)
                return MaxUtility;
    }

    return BaseUtility;
}

float InterceptGoal::CalculateCost()
{
    // Coût élevé
    if (!Owner->IsPlayerVisible())
        return Cost * 2;
    return Cost;
}

/**
 * Return Goal
 */
void ReturnGoal::Activate()
{
    bActive = true;
    bFinished = false;
    Owner->IncreaseTireness(3);

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
    if (!Owner->IsInRoom())
        return MaxUtility;

    return BaseUtility;
}

float ReturnGoal::CalculateCost()
{
    // Coût faible-modéré
    return Cost;
}
