//
// Created by Antoine on 2026-03-09.
//

#include "AgentStates.h"

#include "Game/Game.h"
#include "Game/Entities/Characters/AgentCharacter.h"


/**
 * Global State
 */
AgentGlobalStates *AgentGlobalStates::Instance() {
    static AgentGlobalStates instance;

    return &instance;
}

void AgentGlobalStates::Enter(GEntity *agent) {
    Agent = static_cast<GAgentCharacter*>(agent);
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

void AgentPatrolState::Enter(GEntity *agent) {
    Agent = static_cast<GAgentCharacter*>(agent);
    PatrolIndex = 0;

    std::vector<sf::Vector2f> patrolPoints = Agent->GetPatrolPoints();

    Agent->GetAgentController()->FindPath(patrolPoints[PatrolIndex+1]);
}

void AgentPatrolState::Execute(GEntity* agent) {
    if (!Agent)
        return;

    std::vector<sf::Vector2f> patrolPoints = Agent->GetPatrolPoints();
    if (patrolPoints.empty())
        return;

    sf::Vector2f agentPos = Agent->GetTransformComponent()->GetPosition();
    sf::Vector2f targetPos = patrolPoints[PatrolIndex];

    sf::Vector2f diff = targetPos - agentPos;
    float distanceSq = diff.x * diff.x + diff.y * diff.y;

    if (distanceSq < 25.f)
    {
        PatrolIndex = (PatrolIndex + 1) % patrolPoints.size();
        Agent->GetAgentController()->FindPath(patrolPoints[PatrolIndex]);
    }
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

void AgentChaseState::Enter(GEntity *agent) {
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

void AgentReturnState::Enter(GEntity *agent) {
}

void AgentReturnState::Execute(GEntity* agent) {
}

void AgentReturnState::Exit(GEntity* agent) {
}
