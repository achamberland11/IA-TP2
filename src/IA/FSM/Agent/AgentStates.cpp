//
// Created by Antoine on 2026-03-09.
//

#include "AgentStates.h"
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
}

void AgentPatrolState::Execute(GEntity* agent) {
    if (!Agent)
        return;

    // @TODO temp type shit
    std::vector<sf::Vector2f> waypoints = Agent->GetWaypoints();
    sf::Vector2f currentWaypoint = waypoints[PatrolIndex];

    if (!waypoints.empty() && !Agent->WaypointReached()) {
        sf::Vector2f position = Agent->GetTransformComponent()->GetPosition();
        sf::Vector2f direction = currentWaypoint - position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance < 0.1f) {
            if (PatrolIndex + 1 < waypoints.size())
                PatrolIndex++;
            else {
                PatrolIndex = 0;
            }
        }
        else {
            direction /= distance;
            Agent->SetVelocity(direction * 100.f);
        }
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
