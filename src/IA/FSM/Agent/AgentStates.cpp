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
	std::vector<sf::Vector2f> patrolPoints = Agent->GetPatrolPoints();

	Agent->GetAgentController()->FindPath(patrolPoints[Agent->GetPatrolIndex()]);
}

void AgentPatrolState::Execute(GEntity* agent)
{
	Agent = static_cast<GAgentCharacter*>(agent);

	std::vector<sf::Vector2f> patrolPoints = Agent->GetPatrolPoints();
	if (patrolPoints.empty())
		return;

	if (Agent->GetWaypoints().empty())
	{
		int nextIndex = (Agent->GetPatrolIndex() + 1) % patrolPoints.size();
		Agent->SetPatrolIndex(nextIndex);
		Agent->GetAgentController()->FindPath(patrolPoints[nextIndex]);
	}
	
	if (Agent->IsPlayerVisible() && Agent->GetFSM())
		Agent->GetFSM()->ChangeState(AgentChaseState::Instance());
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

	if (!Agent->IsPlayerVisible())
		targetPos = Agent->GetBlackboard()->ListenToRadio().PlayerLastPosition;
	
	Agent->GetAgentController()->FindPath(targetPos);
	
	if (Agent->GetWaypoints().empty() && !Agent->IsPlayerVisible() && Agent->GetFSM())
		Agent->GetFSM()->ChangeState(AgentReturnState::Instance());
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
	sf::Vector2f targetPos = patrolPoints[Agent->GetPatrolIndex()];

	Agent->GetAgentController()->FindPath(targetPos);
	
	if (Agent->IsPlayerVisible())
		Agent->GetFSM()->ChangeState(AgentChaseState::Instance());
}

void AgentReturnState::Execute(GEntity* agent)
{
	std::vector<sf::Vector2f> waypoints = Agent->GetWaypoints();
	if (waypoints.empty() && Agent->GetFSM())
	{
		Agent->GetFSM()->ChangeState(AgentPatrolState::Instance());
	}
}

void AgentReturnState::Exit(GEntity* agent)
{
}
