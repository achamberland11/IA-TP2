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
    TirednessTimer = 0.f;

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

    TirednessTimer += dt;
    if (TirednessTimer >= 3.f)
    {
        Owner->IncreaseTiredness(1);
        TirednessTimer = 0.f;
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

    BreakLocation = Owner->GetBreakRoom()->Center();

    sf::Vector2f agentPos = Owner->GetTransformComponent()->GetPosition();
    distToBreakRoom = std::sqrt(std::pow(agentPos.x - BreakLocation.x, 2) + std::pow(agentPos.y - BreakLocation.y, 2));

    Owner->GetAgentController()->FindPath(BreakLocation);
}

void TakeBreakGoal::Execute(float dt)
{
    if (!Owner->GetWaypoints().empty())
        return;

    sf::Vector2f agentPos = Owner->GetTransformComponent()->GetPosition();
    float sqrdDist = std::pow(agentPos.x - BreakLocation.x, 2) + std::pow(agentPos.y - BreakLocation.y, 2);

    if (sqrdDist < DetectionRangeSqrd)
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
    Owner->ResetTiredness();
    bActive = false;
    bFinished = true;
}

float TakeBreakGoal::CalculateUtility()
{
    // Score modéré, variable selon la fatigue/temps écoulé
    return std::min(BaseUtility * Owner->GetTiredness(), MaxUtility);
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
    Owner->IncreaseTiredness(4);
    bActive = true;
    bFinished = false;

    if (Blackboard && Blackboard->ListenToRadio().bPlayerSeen)
        AlertLocation = Blackboard->ListenToRadio().PlayerLastPosition;

    if (Owner->GetAgentController())
        Owner->GetAgentController()->FindPath(AlertLocation);
}

void RespondToAlertGoal::Execute(float dt)
{
    if (!Blackboard)
    {
        Terminate();
        return;
    }

    const RadioInfo &radioInfo = Blackboard->ListenToRadio();

    if (Owner->IsPlayerVisible())
    {
        AlertLocation = radioInfo.PlayerLastPosition;
        Owner->GetAgentController()->FindPath(AlertLocation);
    } else if (radioInfo.bPlayerSeen)
    {
        AlertLocation = radioInfo.PlayerLastPosition;
        Owner->GetAgentController()->FindPath(AlertLocation);
    }


    sf::Vector2f agentPos = Owner->GetTransformComponent()->GetPosition();
    float sqrdDist = std::pow(agentPos.x - AlertLocation.x, 2) + std::pow(agentPos.y - AlertLocation.y, 2);

    if (sqrdDist < DetectionRangeSqrd && !Owner->IsPlayerVisible())
    {
        Blackboard->ResetRadio();
        Terminate();
    }
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
        float distToReportSqrd = Blackboard->GetDistToReportingAgent(
            Owner->GetTransformComponent()->GetPosition(),
            Owner->GetAgentID());

        if (distToReportSqrd != -1.f && distToReportSqrd <= Blackboard->RadioMaxDist * Blackboard->RadioMaxDist)
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
    Owner->IncreaseTiredness(6);

    if (Blackboard && Blackboard->ListenToRadio().bPlayerSeen)
        LastKnownPosition = Blackboard->ListenToRadio().PlayerLastPosition;
    else if (Owner->IsPlayerVisible())
        LastKnownPosition = Blackboard->ListenToRadio().PlayerLastPosition;

    if (Owner->GetAgentController())
        Owner->GetAgentController()->FindPath(LastKnownPosition);
}

void InterceptGoal::Execute(float dt)
{
    if (!Blackboard)
    {
        Terminate();
        return;
    }

    LastKnownPosition = Blackboard->ListenToRadio().PlayerLastPosition;
    sf::Vector2f predictedPos = LastKnownPosition;

    if (Owner->IsPlayerVisible())
    {
        LastKnownPosition = Owner->GetAgentController()->GetPlayer()->GetTransformComponent()->GetPosition();
        sf::Vector2f playerPos = LastKnownPosition;
        sf::Vector2f playerVel = Owner->GetAgentController()->GetPlayer()->GetVelocity();
        float predictionTime = 0.5f;
        predictedPos = playerPos + playerVel * predictionTime;
    }

    Owner->GetAgentController()->FindPath(predictedPos);

    sf::Vector2f agentPos = Owner->GetTransformComponent()->GetPosition();
    float sqrdDist = std::pow(agentPos.x - LastKnownPosition.x, 2) + std::pow(agentPos.y - LastKnownPosition.y, 2);
    if (sqrdDist < DetectionRangeSqrd && !Owner->IsPlayerVisible())
    {
        Terminate();
    }
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
        float distToReportSqrd = Blackboard->GetDistToReportingAgent(Owner->GetTransformComponent()->GetPosition(),
                                                                     Owner->GetAgentID());
        if (distToReportSqrd != -1.f)
            if (distToReportSqrd <= DetectionRangeSqrd)
                return MaxUtility;
    }

    return BaseUtility;
}

float InterceptGoal::CalculateCost()
{
    // Coût élevé
    return Cost;
}

/**
 * Return Goal
 */
void ReturnGoal::Activate()
{
    bActive = true;
    bFinished = false;
    Owner->IncreaseTiredness(3);

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
