//
// Created by Antoine on 2026-03-30.
//

#pragma once
#include <vector>

#include "Game/Components/Component.h"
#include "IA/GlobalBlackboard.h"
#include "SFML/Network/SocketHandle.hpp"
#include "SFML/System/Vector2.hpp"

class Goal;

class GGOBComponent : public GComponent
{
public:
    GENERATE_COMPONENT(false, true, true)

    GGOBComponent(GEntity* owner) : GComponent(owner) { Name = "GOP"; }

    void Start() override;
    void Update(float deltaSeconds) override;

    Goal* GetActiveGoal() { return ActiveGoal; }

private:
    void EvaluateGoals();
    void ActivateGoal(Goal* goal);
    void TerminateActiveGoal();

    GlobalBlackboard* Blackboard = nullptr;

    std::vector<Goal*> Goals;
    Goal* ActiveGoal = nullptr;

    bool bAlertActive = false;
    float AlertTimer = 0.f;
    float BreakTimer = 0.f;
};
