//
// Created by Antoine on 2026-03-30.
//

#pragma once
#include <vector>

#include "Game/Components/Component.h"
#include "IA/GlobalBlackboard.h"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Network/SocketHandle.hpp"
#include "SFML/System/Vector2.hpp"

class Goal;

class GGOBComponent : public GComponent
{
public:
    GENERATE_COMPONENT(false, true, true)

    GGOBComponent(GEntity* owner) : GComponent(owner) { Name = "GOP"; }
    ~GGOBComponent() override;

    void Start() override;
    void Update(float deltaSeconds) override;
    void Render(sf::RenderWindow &window) override;

    Goal* GetActiveGoal() const { return ActiveGoal; }

    sf::Vector2f GetGoalTextPosition() const { return GoalTextPosition; }
    float GetGoalTextHeight() const { return GoalTextHeight; }

private:
    void EvaluateGoals();
    void ActivateGoal(Goal* goal);
    void TerminateActiveGoal();

    GlobalBlackboard* Blackboard = nullptr;

    std::vector<Goal*> Goals;
    Goal* ActiveGoal = nullptr;
    
    sf::Vector2f GoalTextPosition;
    float GoalTextHeight = 0.f;

    bool bAlertActive = false;
    float AlertTimer = 0.f;
    float BreakTimer = 0.f;
};
