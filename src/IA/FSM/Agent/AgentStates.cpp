//
// Created by Antoine on 2026-03-09.
//

#include "AgentStates.h"

#include "Game/Game.h"
#include "Game/Entities/Characters/AgentCharacter.h"


/**
 * Global State
 */
AgentGlobalStates* AgentGlobalStates::Instance()
{
	static AgentGlobalStates instance;

	return &instance;
}

void AgentGlobalStates::Enter(GEntity* agent)
{
	Agent = static_cast<GAgentCharacter*>(agent);
}

void AgentGlobalStates::Execute(GEntity* agent)
{
}

void AgentGlobalStates::Exit(GEntity* agent)
{
}

/**
 * Patrol State
 */
AgentPatrolState* AgentPatrolState::Instance()
{
	static AgentPatrolState instance;

	return &instance;
}

void AgentPatrolState::Enter(GEntity* agent)
{
	Agent = static_cast<GAgentCharacter*>(agent);
	PatrolIndex = Agent->GetPatrolIndex();

	std::vector<sf::Vector2f> patrolPoints = Agent->GetPatrolPoints();

	Agent->GetAgentController()->FindPath(patrolPoints[PatrolIndex + 1]);
}

void AgentPatrolState::Execute(GEntity* agent)
{
	if (!Agent)
		return;

	std::vector<sf::Vector2f> patrolPoints = Agent->GetPatrolPoints();
	if (patrolPoints.empty())
		return;

	std::vector<sf::Vector2f> waypoints = Agent->GetWaypoints();
	if (waypoints.empty())
	{
		PatrolIndex = (PatrolIndex + 1) % patrolPoints.size();
		Agent->GetAgentController()->FindPath(patrolPoints[PatrolIndex]);
		Agent->SetPatrolIndex(PatrolIndex);
	}
}

void AgentPatrolState::Exit(GEntity* agent)
{
}

/**
 * Chase State
 */
AgentChaseState* AgentChaseState::Instance()
{
	static AgentChaseState instance;

	return &instance;
}

void AgentChaseState::Enter(GEntity* agent)
{
	Agent = static_cast<GAgentCharacter*>(agent);
}

void AgentChaseState::Execute(GEntity* agent)
{
	if (!Agent)
		return;

	sf::Vector2f targetPos = Agent->GetAgentController()->GetPlayer()->GetTransformComponent()->GetPosition();

	Agent->GetAgentController()->FindPath(targetPos);
}

void AgentChaseState::Exit(GEntity* agent)
{
}

/**
 * Return State
 */
AgentReturnState* AgentReturnState::Instance()
{
	static AgentReturnState instance;

	return &instance;
}

void AgentReturnState::Enter(GEntity* agent)
{
	Agent = static_cast<GAgentCharacter*>(agent);

	std::vector<sf::Vector2f> patrolPoints = Agent->GetPatrolPoints();

	Agent->GetAgentController()->FindPath(patrolPoints[Agent->GetPatrolIndex()]);
}

void AgentReturnState::Execute(GEntity* agent)
{
	std::vector<sf::Vector2f> waypoints = Agent->GetWaypoints();
	if (waypoints.empty())
	{
		Agent->GetFSM()->ChangeState(AgentPatrolState::Instance());
	}
}

void AgentReturnState::Exit(GEntity* agent)
{
}
