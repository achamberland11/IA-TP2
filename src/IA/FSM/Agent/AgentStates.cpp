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

void AgentGlobalStates::Enter(GEntity* agent) {
}

void AgentGlobalStates::Execute(GEntity* agent) {
}

void AgentGlobalStates::Exit(GEntity* agent) {
}

/**
 * Patrol State
 */
AgentPatrolState *AgentPatrolState::Instance() {
    static AgentPatrolState instance;

    return &instance;
}

void AgentPatrolState::Enter(GEntity* agent) {
}

void AgentPatrolState::Execute(GEntity* agent) {
}

void AgentPatrolState::Exit(GEntity* agent) {
}

/**
 * Chase State
 */
AgentChaseState *AgentChaseState::Instance() {
    static AgentChaseState instance;

    return &instance;
}

void AgentChaseState::Enter(GEntity* agent) {
}

void AgentChaseState::Execute(GEntity* agent) {
}

void AgentChaseState::Exit(GEntity* agent) {
}

/**
 * Return State
 */
AgentReturnState *AgentReturnState::Instance() {
    static AgentReturnState instance;

    return &instance;
}

void AgentReturnState::Enter(GEntity* agent) {
}

void AgentReturnState::Execute(GEntity* agent) {
}

void AgentReturnState::Exit(GEntity* agent) {
}
