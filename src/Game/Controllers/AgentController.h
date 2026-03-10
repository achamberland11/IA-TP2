//
// Created by Antoine on 2026-03-09.
//

#pragma once
#include "Controller.h"
#include "Game/Components/FSMComponent.h"


class GAgentController : public GController
{
public:
    GAgentController(GCharacter* owner) : GController(owner)
    {}
    ~GAgentController() override = default;

    void Start() override;
    void Update(float dt) override;

    void HandleEvent(const sf::Event& event) override;

private:
    GFSMComponent* FSM;
};
