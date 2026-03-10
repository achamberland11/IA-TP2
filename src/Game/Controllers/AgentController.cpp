//
// Created by Antoine on 2026-03-09.
//

#include "AgentController.h"


void GAgentController::Start() {
    GController::Start();
    
    if (!Owner) return;
    
    FSM = Owner->GetComponent<GFSMComponent>();

    assert(FSM && "<GAgentController> : FSMComponent not found !");
    FSM->SetCurrentState(AgentPatrolState::Instance());
    FSM->SetGlobalState(AgentGlobalStates::Instance());
}

void GAgentController::Update(float dt) {
    GController::Update(dt);

    if (!Owner) return;
    if (!FSM)
        return;

    FSM->Update(dt);
}

void GAgentController::HandleEvent(const sf::Event &event) {
    if (!Owner) return;
}