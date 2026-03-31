//
// Created by Antoine on 2026-03-30.
//

#pragma once
#include "Core/Object.h"
#include "IA/GOB/Goal.h"
#include "SFML/System/Vector2.hpp"


class PatrolGoal : public Goal
{
public:
    PatrolGoal(GAgentCharacter* agent, GlobalBlackboard* blackboard) : Goal(agent, EGoalType::Patrol, blackboard)
    {
        Name = "PatrolGoal";
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
    float tirenessTimer;
};

class TakeBreakGoal : public Goal
{
public:
    TakeBreakGoal(GAgentCharacter* agent, GlobalBlackboard* blackboard) : Goal(agent, EGoalType::TakeBreak, blackboard)
    {
        Name = "TakeBreakGoal";
    }

    void Activate() override;
    void Execute(float dt) override;
    void Terminate() override;

    float CalculateUtility() override;
    float CalculateCost() override;

    void SetBreakLocation(sf::Vector2f location) { BreakLocation = location; }

private:
    sf::Vector2f BreakLocation;
    bool bGoToBreakRoom = true;
    float BreakDuration = 10.f;
    float CurrentBreakTime = 0.f;
    float distToBreakRoom = 0.f;
};

class RespondToAlertGoal : public Goal
{
public:
    RespondToAlertGoal(GAgentCharacter* agent, GlobalBlackboard* blackboard) : Goal(agent, EGoalType::RespondToAlert, blackboard)
    {
        Name = "RespondToAlertGoal";
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
    }

    void Activate() override;
    void Execute(float dt) override;
    void Terminate() override;

    float CalculateUtility() override;
    float CalculateCost() override;
};