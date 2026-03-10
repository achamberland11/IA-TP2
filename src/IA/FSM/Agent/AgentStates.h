//
// Created by Antoine on 2026-03-09.
//

#pragma once
#include "IA/FSM/State.h"

class GAgentCharacter;

/**
 * Global state
 */
class AgentGlobalStates : public State<GAgentCharacter>
{
private:
    AgentGlobalStates() {
        StateName = "GlobalState";
    }

    AgentGlobalStates(const AgentGlobalStates&);
    AgentGlobalStates& operator=(const AgentGlobalStates&);

public:
    static AgentGlobalStates* Instance();

    void Enter(GAgentCharacter* agent) override;
    void Execute(GAgentCharacter* agent) override;
    void Exit(GAgentCharacter* agent) override;
};

/**
 * Patrol state
 */
class AgentPatrolState : public State<GAgentCharacter> {
private:
    AgentPatrolState() {
        StateName = "PatrolState";
    }

    AgentPatrolState(const AgentPatrolState&);
    AgentPatrolState& operator=(const AgentPatrolState&);

public:
    static AgentPatrolState* Instance();

    void Enter(GAgentCharacter* agent) override;
    void Execute(GAgentCharacter* agent) override;
    void Exit(GAgentCharacter* agent) override;
};

/**
 * Chase State
 */
class AgentChaseState : public State<GAgentCharacter> {
private:
    AgentChaseState() {
        StateName = "ChaseState";
    }

    AgentChaseState(const AgentChaseState&);
    AgentChaseState& operator=(const AgentChaseState&);

public:
    static AgentChaseState* Instance();

    void Enter(GAgentCharacter* agent) override;
    void Execute(GAgentCharacter* agent) override;
    void Exit(GAgentCharacter* agent) override;
};

/**
 * Return State
 */
class AgentReturnState : public State<GAgentCharacter> {
    AgentReturnState() {
        StateName = "ReturnState";
    }

    AgentReturnState(const AgentReturnState&);
    AgentReturnState& operator=(const AgentReturnState&);

public:
    static AgentReturnState* Instance();

    void Enter(GAgentCharacter* agent) override;
    void Execute(GAgentCharacter* agent) override;
    void Exit(GAgentCharacter* agent) override;
};