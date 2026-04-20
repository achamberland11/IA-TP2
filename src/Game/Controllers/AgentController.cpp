//
// Created by Antoine on 2026-03-09.
//

#include "AgentController.h"

#include "Game/Game.h"
#include "Game/Map/AStar.h"


GAgentController::GAgentController(GCharacter *owner) : GController(owner)
{
}

void GAgentController::Start()
{
    GController::Start();

    if (!Owner) return;

    GGame *game = GGame::GetInstance();
    if (!game) return;

    Player = game->GetPlayerCharacter();
    Agent = dynamic_cast<GAgentCharacter *>(Owner);

    Vision = Owner->GetComponent<GConeVisionComponent>();
    
    GlobalBlackboard = game->GetGlobalBlackboard();
}

void GAgentController::Update(float dt)
{
    GController::Update(dt);

    if (!Owner /*|| !FSM*/ || !Vision || !Player || !Agent)
        return;

    GGame *game = GGame::GetInstance();
    if (!game)
        return;

    if (!GlobalBlackboard)
        return;

    const RadioInfo &radioInfo = GlobalBlackboard->ListenToRadio();

    if (Agent->IsPlayerVisible())
    {
        GlobalBlackboard->TryBroadcastToRadio(Player->GetTransformComponent()->GetPosition(), Agent->GetAgentID());
    }
    else if (radioInfo.bPlayerSeen)
    {
        float distToReport = GlobalBlackboard->GetDistToReportingAgent(Agent->GetTransformComponent()->GetPosition(),
                                                               Agent->GetAgentID());
        if (distToReport != -1.f)
        {
            if (distToReport <= GlobalBlackboard->RadioMaxDist * GlobalBlackboard->RadioMaxDist)
            {
                // if (FSM->GetCurrentState() != AgentChaseState::Instance())
                    // FSM->ChangeState(AgentChaseState::Instance());
            }
        }
    }
    /*else if (FSM->GetCurrentState() == AgentChaseState::Instance())
    {
        FSM->ChangeState(AgentReturnState::Instance());
    }*/
}

void GAgentController::HandleEvent(const sf::Event &event)
{
    if (!Owner) return;
}


void GAgentController::FindPath(const sf::Vector2f &target)
{
    GGame *game = GGame::GetInstance();
    if (!game || !Owner || !Agent) return;

    GMap *map = game->GetMap();
    if (!map) return;

    sf::Vector2f gridTarget = map->WorldToGrid(target);
    int targetRow = (int)gridTarget.y;
    int targetCol = (int)gridTarget.x;

    if (!map->IsWalkable(targetRow, targetCol))
    {
        const int rowOffsets[] = { -1, 1, 0, 0 };
        const int colOffsets[] = { 0, 0, -1, -1 };

        bool foundValidNeighbor = false;

        for (int i = 0; i < 4; ++i)
        {
            int neighborRow = targetRow + rowOffsets[i];
            int neighborCol = targetCol + colOffsets[i];

            if (map->IsWalkable(neighborRow, neighborCol))
            {
                gridTarget = sf::Vector2f((float)(neighborCol), static_cast<float>(neighborRow));
                foundValidNeighbor = true;
                break;
            }
        }

        if (!foundValidNeighbor)
            return;

    }

    sf::Vector2f validTarget = map->GridToWorld(gridTarget);

    auto path = AStar::FindPath(
        map,
        Owner->GetTransformComponent()->GetPosition(),
        validTarget);
     
    if (path.empty())
        return;

    path.erase(path.begin());

    /*std::cout << Owner->GetTransformComponent()->GetPosition().x << ", " << Owner->GetTransformComponent()->GetPosition().y << std::endl;
    std::cout << target.x << ", " << target.y << std::endl;
    std::cout << path.size() << std::endl;*/

    Agent->SetWaypoints(path);
}