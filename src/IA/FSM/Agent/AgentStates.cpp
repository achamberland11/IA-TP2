//
// Created by Antoine on 2026-03-09.
//

#include "AgentStates.h"

/**
 * Global State
 */
AgentGlobalStates *AgentGlobalStates::Instance() {
    static AgentGlobalStates instance;

    return &instance;
}

void AgentGlobalStates::Enter(GAgentCharacter *agent) {
}

void AgentGlobalStates::Execute(GAgentCharacter *agent) {
}

void AgentGlobalStates::Exit(GAgentCharacter *agent) {
}

/**
 * Patrol State
 */
AgentPatrolState *AgentPatrolState::Instance() {
    static AgentPatrolState instance;

    return &instance;
}

void AgentPatrolState::Enter(GAgentCharacter *agent) {
}

void AgentPatrolState::Execute(GAgentCharacter *agent) {
}

void AgentPatrolState::Exit(GAgentCharacter *agent) {
}

/**
 * Chase State
 */
AgentChaseState *AgentChaseState::Instance() {
    static AgentChaseState instance;

    return &instance;
}

void AgentChaseState::Enter(GAgentCharacter *agent) {
}

void AgentChaseState::Execute(GAgentCharacter *agent) {
}

void AgentChaseState::Exit(GAgentCharacter *agent) {
}

/**
 * Return State
 */
AgentReturnState *AgentReturnState::Instance() {
    static AgentReturnState instance;

    return &instance;
}

void AgentReturnState::Enter(GAgentCharacter *agent) {
}

void AgentReturnState::Execute(GAgentCharacter *agent) {
}

void AgentReturnState::Exit(GAgentCharacter *agent) {
}
