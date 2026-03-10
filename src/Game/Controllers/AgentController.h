//
// Created by Antoine on 2026-03-09.
//

#pragma once
#include "Controller.h"
#include "Game/Components/FSMComponent.h"
#include "Game/Entities/Characters/Character.h"
#include "IA/FSM/Agent/AgentStates.h"

class GAgentController : public GController
{
public:
    GAgentController(GCharacter* owner) : GController(owner)
    {}
    ~GAgentController() override = default;

    void Start() override;
    void Update(float dt) override;

    void HandleEvent(const sf::Event& event) override;

    void SetTargets(std::vector<sf::Vector2f> newTargets) { targets = newTargets; }
    std::vector<sf::Vector2f> GetTargets() { return targets; }
    int GetCurrentTarget() { return currentTarget; }
    sf::Vector2f GetDirection() { return direction; }

private:
    GFSMComponent* FSM = nullptr;

    sf::Vector2f direction;

    std::vector<sf::Vector2f> targets;
    int currentTarget = 0;
    bool bFinished = false;

    sf::Vector2f ComputeSteering(float dt);

    //Steering
    const float SlowingRadius = 50.f;
};
