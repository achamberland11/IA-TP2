//
// Created by Antoine on 2026-03-30.
//

#pragma once
#include "Core/Object.h"
#include "Game/Map/Map.h"
#include "IA/GOB/Goal.h"
#include "SFML/System/Vector2.hpp"


class PatrolGoal : public Goal
{
public:
    PatrolGoal(GAgentCharacter* agent, GlobalBlackboard* blackboard) : Goal(agent, EGoalType::Patrol, blackboard)
    {
        Name = "PatrolGoal";
        BaseUtility = 8.f;
        MaxUtility = 30.f;
        Cost = 5.f;
    }

    void Activate() override;
    void Execute(float dt) override;
    void Terminate() override;

    float CalculateUtility() override;
    float CalculateCost() override;

private:
    int CurrentPatrolIndex = 0;
    int NextPatrolIndex = CurrentPatrolIndex+1;
    sf::Vector2f CurrentTarget;
    float TirednessTimer;
};

class TakeBreakGoal : public Goal
{
public:
    TakeBreakGoal(GAgentCharacter* agent, GlobalBlackboard* blackboard) : Goal(agent, EGoalType::TakeBreak, blackboard)
    {
        Name = "TakeBreakGoal";
        BaseUtility = 4.f;
        MaxUtility = 150.f;
        Cost = 10.f;
    }

    void Activate() override;
    void Execute(float dt) override;
    void Terminate() override;

    float CalculateUtility() override;
    float CalculateCost() override;

    void SetBreakLocation(sf::Vector2f location) { BreakLocation = location; }

private:
    FRoom* BreakRoom = nullptr;
    sf::Vector2f BreakLocation;
    bool bGoToBreakRoom = true;
    float BreakDuration = 10.f;
    float CurrentBreakTime = 0.f;
    float DistToBreakRoomSqrd = 0.f;
};

class RespondToAlertGoal : public Goal
{
public:
    RespondToAlertGoal(GAgentCharacter* agent, GlobalBlackboard* blackboard) : Goal(agent, EGoalType::RespondToAlert, blackboard)
    {
        Name = "RespondToAlertGoal";
        BaseUtility = 0.f;
        MaxUtility = 105.f;
        Cost = 10.f;
    }

    void Activate() override;
    void Execute(float dt) override;
    void Terminate() override;

    float CalculateUtility() override;
    float CalculateCost() override;

private:
    sf::Vector2f AlertLocation;
};

class InterceptGoal : public Goal
{
public:
    InterceptGoal(GAgentCharacter* agent, GlobalBlackboard* blackboard) : Goal(agent, EGoalType::Intercept, blackboard)
    {
        Name = "InterceptGoal";
        BaseUtility = 10.f;
        MaxUtility = 150.f;
        Cost = 30.f;
    }

    void Activate() override;
    void Execute(float dt) override;
    void Terminate() override;

    float CalculateUtility() override;
    float CalculateCost() override;

private:
    sf::Vector2f LastKnownPosition;
};

class ReturnGoal : public Goal
{
public:
    ReturnGoal(GAgentCharacter* agent, GlobalBlackboard* blackboard) : Goal(agent, EGoalType::Return, blackboard)
    {
        Name = "ReturnGoal";
        BaseUtility = -10.f;
        MaxUtility = 90.f;
        Cost = 8.f;
    }

    void Activate() override;
    void Execute(float dt) override;
    void Terminate() override;

    float CalculateUtility() override;
    float CalculateCost() override;
};