//
// Created by Antoine on 2026-03-09.
//

#pragma once
#include "Controller.h"
#include "Game/Components/FSMComponent.h"
#include "Game/Entities/Characters/Character.h"
#include "IA/FSM/Agent/AgentStates.h"
#include <Game/Entities/Characters/PlayerCharacter.h>
#include "IA/GlobalBlackboard.h"
#include "Game/Components/ConeVisionComponent.h"

class GAgentController : public GController
{
public:
    GAgentController(GCharacter* owner);
    ~GAgentController() override = default;

    void Start() override;
    void Update(float dt) override;

    void HandleEvent(const sf::Event& event) override;
    int GetCurrentTarget() { return CurrentTarget; }
    void FindPath(const sf::Vector2f &target);

    GPlayerCharacter* GetPlayer() { return Player; }

private:
    GFSMComponent* FSM = nullptr;
    GConeVisionComponent* Vision = nullptr;

    GPlayerCharacter* Player = nullptr;
    GAgentCharacter* Agent = nullptr;

    int CurrentTarget = 0;
    bool bFinished = false;

};
