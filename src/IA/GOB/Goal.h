//
// Created by Antoine on 2026-03-30.
//

#pragma once
#include "IA/GlobalBlackboard.h"

class GAgentCharacter;

enum class EGoalType
{
    Patrol,
    TakeBreak,
    RespondToAlert,
    Intercept,
    Return,
};

class Goal
{
public:
    Goal(GAgentCharacter* owner, EGoalType type, GlobalBlackboard* blackboard) : Owner(owner), Type(type), Blackboard(blackboard) {}
    virtual ~Goal()
    {
        Owner = nullptr;
        Blackboard = nullptr;
    }

    virtual void Activate() = 0;
    virtual void Execute(float dt) = 0;
    virtual void Terminate() = 0;

    float GetEffectiveScore() { return CalculateUtility() - CalculateCost(); }

    EGoalType GetType() const { return Type; }
    bool IsActive() const { return bActive; }
    bool IsFinished() const { return bFinished; }

protected:
    virtual float CalculateUtility() = 0;
    virtual float CalculateCost() = 0;

    GAgentCharacter* Owner;
    EGoalType Type;
    float Utility = 0.f;
    bool bActive = false;
    bool bFinished = false;

    GlobalBlackboard* Blackboard;
};