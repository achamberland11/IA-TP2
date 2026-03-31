//
// Created by Antoine on 2026-03-30.
//

#include "GOBComponent.h"

#include <iostream>
#include <ostream>

#include "Game/Components/ConeVisionComponent.h"
#include "IA/GOB/Goal.h"
#include "IA/GOB/Agent/AgentGoals.h"

void GGOBComponent::Start()
{
    GAgentCharacter* agent = (GAgentCharacter*)Owner;

    Goals.push_back(new PatrolGoal(agent));
    Goals.push_back(new TakeBreakGoal(agent));
    Goals.push_back(new InterceptGoal(agent));
    Goals.push_back(new RespondToAlertGoal(agent));
    Goals.push_back(new ReturnGoal(agent));

    ActiveGoal = Goals[0];
}

void GGOBComponent::Update(float deltaSeconds)
{
    if (!ActiveGoal) return;

    if (!ActiveGoal->IsActive())
        ActivateGoal(Goals[0]);

    ActiveGoal->Execute(deltaSeconds);

    if (ActiveGoal->IsFinished())
    {
        TerminateActiveGoal();
        EvaluateGoals();
    }
}

void GGOBComponent::EvaluateGoals()
{
    ActivateGoal(Goals[0]);
}

void GGOBComponent::ActivateGoal(Goal *goal)
{
    if (!goal) return;
    ActiveGoal = goal;
    ActiveGoal->Activate();
}

void GGOBComponent::TerminateActiveGoal()
{
    ActiveGoal->Terminate();
    ActiveGoal = nullptr;
}
