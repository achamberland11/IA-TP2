//
// Created by Antoine on 2026-03-30.
//

#pragma once

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
    Goal(GAgentCharacter* owner, EGoalType type) : Owner(owner), Type(type) {}
    virtual ~Goal() = default;

    virtual void Activate() = 0;
    virtual void Execute(float dt) = 0;
    virtual void Terminate() = 0;

    virtual float CalculateUtility() = 0;
    virtual float CalculateCost() = 0;

    float GetEffectiveScore() { return CalculateUtility() - CalculateCost(); }

    EGoalType GetType() const { return Type; }
    bool IsActive() const { return bActive; }
    bool IsFinished() const { return bFinished; }

protected:
    GAgentCharacter* Owner;
    EGoalType Type;
    float Utility = 0.f;
    bool bActive = false;
    bool bFinished = false;
};