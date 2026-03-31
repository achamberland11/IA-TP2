//
// Created by Antoine on 2026-03-30.
//

#include "GOBComponent.h"

#include <iostream>
#include <ostream>

#include "Game/Game.h"
#include "Game/Components/ConeVisionComponent.h"
#include "IA/GOB/Goal.h"
#include "IA/GOB/Agent/AgentGoals.h"

void GGOBComponent::Start()
{
    GAgentCharacter* agent = (GAgentCharacter*)Owner;

    Blackboard = GGame::GetInstance()->GetGlobalBlackboard();

    Goals.push_back(new PatrolGoal(agent, Blackboard));
    Goals.push_back(new TakeBreakGoal(agent, Blackboard));
    Goals.push_back(new InterceptGoal(agent, Blackboard));
    Goals.push_back(new RespondToAlertGoal(agent, Blackboard));
    Goals.push_back(new ReturnGoal(agent, Blackboard));

    ActiveGoal = Goals[0];
}

void GGOBComponent::Update(float deltaSeconds)
{
    if (!ActiveGoal) return;

    ActiveGoal->Execute(deltaSeconds);

    EvaluateGoals();
}

void GGOBComponent::EvaluateGoals()
{
    if (Goals.empty()) return;

    Goal *bestGoal = Goals[0];
    float bestScore = -std::numeric_limits<float>::infinity();

    for (Goal *goal : Goals)
    {
        if (!goal)
            continue;

        const float score = goal->GetEffectiveScore();
        if (score > bestScore)
        {
            bestScore = score;
            bestGoal = goal;
        }
    }

    if (bestGoal)
        ActivateGoal(bestGoal);
}

void GGOBComponent::ActivateGoal(Goal *goal)
{
    if (!goal) return;

    if (ActiveGoal == goal)
        return;

    if (ActiveGoal)
        ActiveGoal->Terminate();

    ActiveGoal = goal;
    ActiveGoal->Activate();
}

void GGOBComponent::TerminateActiveGoal()
{
    ActiveGoal->Terminate();
    ActiveGoal = nullptr;
}
