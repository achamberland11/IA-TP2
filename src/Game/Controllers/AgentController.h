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
    GAgentController(GCharacter* owner);
    ~GAgentController() override = default;

    void Start() override;
    void Update(float dt) override;

    void HandleEvent(const sf::Event& event) override;

private:
    GFSMComponent* FSM = nullptr;
};
