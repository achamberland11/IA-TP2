//
// Created by Antoine on 2026-03-09.
//

#pragma once
#include "Game/Entities/Entity.h"
#include "IA/FSM/State.h"

class GAgentCharacter;

/**
 * Global state
 */
class AgentGlobalStates : public State<GEntity>
{
private:
    AgentGlobalStates() {
        StateName = "GlobalState";
    }

    AgentGlobalStates(const AgentGlobalStates&);
    AgentGlobalStates& operator=(const AgentGlobalStates&);

public:
    static AgentGlobalStates* Instance();

    void Enter(GEntity* agent) override;
    void Execute(GEntity* agent) override;
    void Exit(GEntity* agent) override;
};

/**
 * Patrol state
 */
class AgentPatrolState : public State<GEntity> {
private:
    AgentPatrolState() {
        StateName = "PatrolState";
    }

    AgentPatrolState(const AgentPatrolState&);
    AgentPatrolState& operator=(const AgentPatrolState&);

public:
    static AgentPatrolState* Instance();

    void Enter(GEntity* agent) override;
    void Execute(GEntity* agent) override;
    void Exit(GEntity* agent) override;
};

/**
 * Chase State
 */
class AgentChaseState : public State<GEntity> {
private:
    AgentChaseState() {
        StateName = "ChaseState";
    }

    AgentChaseState(const AgentChaseState&);
    AgentChaseState& operator=(const AgentChaseState&);

public:
    static AgentChaseState* Instance();

    void Enter(GEntity* agent) override;
    void Execute(GEntity* agent) override;
    void Exit(GEntity* agent) override;
};

/**
 * Return State
 */
class AgentReturnState : public State<GEntity> {
    AgentReturnState() {
        StateName = "ReturnState";
    }

    AgentReturnState(const AgentReturnState&);
    AgentReturnState& operator=(const AgentReturnState&);

public:
    static AgentReturnState* Instance();

    void Enter(GEntity* agent) override;
    void Execute(GEntity* agent) override;
    void Exit(GEntity* agent) override;
};