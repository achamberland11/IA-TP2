//
// Created by Antoine on 2026-03-09.
//

#include "FSMComponent.h"

void GFSMComponent::Start() {
}

void GFSMComponent::Update(float deltaSeconds) {
    if (GlobalState) GlobalState->Execute(Owner);

    if (CurrentState) CurrentState->Execute(Owner);
}

void GFSMComponent::ChangeState(State<GEntity> *newState) {
    assert(newState && "<GFSMComponent> : Trying to change to a null state !");

    PreviousState = CurrentState;

    CurrentState->Exit(Owner);

    CurrentState = newState;

    CurrentState->Enter(Owner);
}

void GFSMComponent::RevertToPreviousState() {
    ChangeState(PreviousState);
}

bool GFSMComponent::isInState(const State<GEntity> &state) const {
    return CurrentState == &state;
}

std::string GFSMComponent::GetNameOfCurrentState() const {
    return CurrentState->GetName();
}
