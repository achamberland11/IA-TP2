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

GGOBComponent::~GGOBComponent()
{
    TerminateActiveGoal();
    
    for (Goal *goal : Goals)
    {
        delete goal;
    }
    Goals.clear();
}

void GGOBComponent::Start()
{
    GAgentCharacter *agent = (GAgentCharacter *) Owner;

    Blackboard = GGame::GetInstance()->GetGlobalBlackboard();

    Goals.push_back(new PatrolGoal(agent, Blackboard));
    Goals.push_back(new TakeBreakGoal(agent, Blackboard));
    Goals.push_back(new InterceptGoal(agent, Blackboard));
    Goals.push_back(new RespondToAlertGoal(agent, Blackboard));
    Goals.push_back(new ReturnGoal(agent, Blackboard));

    ActiveGoal = Goals[1];
}

void GGOBComponent::Update(float deltaSeconds)
{
    if (!ActiveGoal) return;

    ActiveGoal->Execute(deltaSeconds);

    EvaluateGoals();
}

void GGOBComponent::Render(sf::RenderWindow &window)
{
    GComponent::Render(window);

    if (Owner && GetActiveGoal() && GGame::GetInstance()->IsFontLoaded())
    {
        std::string stateName = GetActiveGoal()->GetName();
        const std::string& textGoal = stateName;
        sf::Vector2f textPosition = Owner->GetTransformComponent()->GetPosition();

        const sf::Font& font = GGame::GetFont();
        sf::Text goalText(font);
        
        goalText.setString(textGoal);
        goalText.setCharacterSize(12);
        goalText.setFillColor(sf::Color::White);
        goalText.setStyle(sf::Text::Bold);
        goalText.setOutlineColor(sf::Color::Black);
        goalText.setOutlineThickness(1.5f);
        goalText.setOrigin(goalText.getGlobalBounds().getCenter());
        textPosition.y = textPosition.y + Owner->GetRendererComponent()->GetSprite()->getGlobalBounds().size.y;
        goalText.setPosition(textPosition);
        
        GoalTextPosition = goalText.getPosition();
        GoalTextHeight = goalText.getGlobalBounds().size.y;

        window.draw(goalText);
    }
}

void GGOBComponent::EvaluateGoals()
{
    if (Goals.empty()) return;

    Goal *bestGoal = Goals[1];
    float bestScore = -std::numeric_limits<float>::infinity();

    for (Goal *goal: Goals)
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
    if (!ActiveGoal) return;

    ActiveGoal->Terminate();
    ActiveGoal = nullptr;
}
