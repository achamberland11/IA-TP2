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

    Player = GGame::GetInstance()->GetPlayerCharacter();
    Agent = static_cast<GAgentCharacter*>(Owner);

    // FSM = Owner->GetComponent<GFSMComponent>();
    Vision = Owner->GetComponent<GConeVisionComponent>();

    // assert(FSM && "<GAgentController> : FSMComponent not found !");
    // FSM->ChangeState(AgentPatrolState::Instance());
}

void GAgentController::Update(float dt)
{
    GController::Update(dt);

    if (!Owner) return;
    /*if (!FSM)
        return;*/

    /*if (Agent->IsPlayerVisible())
    {
        if (FSM->GetCurrentState() != AgentChaseState::Instance())
            FSM->ChangeState(AgentChaseState::Instance());
    }
    else if (FSM->GetCurrentState() == AgentChaseState::Instance())
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
    GMap* map = GGame::GetInstance()->GetMap();
    if (!map) return;

    auto path = AStar::FindPath(
        map,
        Owner->GetTransformComponent()->GetPosition(),
        target);
     
    if (path.empty())
        return;

    path.erase(path.begin());

    /*std::cout << Owner->GetTransformComponent()->GetPosition().x << ", " << Owner->GetTransformComponent()->GetPosition().y << std::endl;
    std::cout << target.x << ", " << target.y << std::endl;
    std::cout << path.size() << std::endl;*/

    Agent->SetWaypoints(path);
}