#include "IA/GlobalBlackboard.h"

#include <cstddef>

#include "Game/Game.h"
#include "Game/Entities/Characters/AgentCharacter.h"

float GlobalBlackboard::GetDistToReportingAgent(sf::Vector2f agentPos, int agentID) const
{
    if (!readBuffer.bPlayerSeen || readBuffer.reportingAgentID == agentID)
        return -1.f;

    const GGame* game = GGame::GetInstance();
    if (!game)
        return -1.f;

    const auto agents = game->GetAgentsCharacter();
    const int reportingId = readBuffer.reportingAgentID;
    if (reportingId < 0 || static_cast<std::size_t>(reportingId) >= agents.size())
        return -1.f;

    const GAgentCharacter* reportingAgent = agents[static_cast<std::size_t>(reportingId)];
    if (!reportingAgent || !reportingAgent->GetTransformComponent())
        return -1.f;

    const sf::Vector2f toReport = reportingAgent->GetTransformComponent()->GetPosition() - agentPos;
    return toReport.lengthSquared();
}
