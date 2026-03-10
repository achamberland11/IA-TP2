//
// Created by Antoine on 2026-03-09.
//

#include "AgentController.h"

void GAgentController::Start() {
    GController::Start();
}

void GAgentController::Update(float dt) {
    GController::Update(dt);

    if (!Owner) return;
}

void GAgentController::HandleEvent(const sf::Event &event) {
    if (!Owner) return;
}