//
// Created by Antoine on 2026-03-09.
//

#include "FSMComponent.h"

#include "Game/Game.h"
#include "Game/Entities/Entity.h"
#include "IA/FSM/Agent/AgentStates.h"

GFSMComponent::~GFSMComponent()
{
    delete CurrentState;
    delete GlobalState;
    delete PreviousState;
    CurrentState = nullptr;
    GlobalState = nullptr;
    PreviousState = nullptr;
}

void GFSMComponent::Start()
{
    SetCurrentState(AgentPatrolState::Instance());
    SetGlobalState(AgentGlobalStates::Instance());
}

void GFSMComponent::Update(float deltaSeconds)
{
    if (GlobalState) GlobalState->Execute(Owner);

    if (CurrentState) CurrentState->Execute(Owner);
}

void GFSMComponent::Render(sf::RenderWindow& window)
{
    GComponent::Render(window);

    if (Owner && GetCurrentState() && GGame::GetInstance()->IsFontLoaded())
    {
        std::string stateName = GetCurrentState()->GetName();
        const std::string& textGoal = stateName;
        sf::Vector2f textPosition = Owner->GetTransformComponent()->GetPosition();
        
        const sf::Font& font = GGame::GetFont();
        sf::Text stateText(font);

        stateText.setString(textGoal);
        stateText.setCharacterSize(12);
        stateText.setFillColor(sf::Color::White);
        stateText.setStyle(sf::Text::Bold);
        stateText.setOutlineColor(sf::Color::Black);
        stateText.setOutlineThickness(1.5f);
        stateText.setOrigin(stateText.getGlobalBounds().getCenter());
        textPosition.y = textPosition.y + Owner->GetRendererComponent()->GetSprite()->getGlobalBounds().size.y;
        stateText.setPosition(textPosition);
        
        StateTextPosition = stateText.getPosition();
        StateTextHeight = stateText.getGlobalBounds().size.y;

        window.draw(stateText);
    }
}

void GFSMComponent::ChangeState(State<GEntity> *newState)
{
    assert(newState && "<GFSMComponent> : Trying to change to a null state !");

    PreviousState = CurrentState;

    if (CurrentState) CurrentState->Exit(Owner);

    CurrentState = newState;

    CurrentState->Enter(Owner);
}

void GFSMComponent::RevertToPreviousState()
{
    ChangeState(PreviousState);
}

bool GFSMComponent::isInState(const State<GEntity> &state) const
{
    return CurrentState == &state;
}

std::string GFSMComponent::GetNameOfCurrentState() const
{
    return CurrentState->GetName();
}
