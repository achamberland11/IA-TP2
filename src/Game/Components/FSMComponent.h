//
// Created by Antoine on 2026-03-09.
//

#pragma once
#include "Component.h"
#include "IA/FSM/State.h"
#include <assert.h>


class GFSMComponent : public GComponent
{
public:
    GENERATE_COMPONENT(false, true, true)

    GFSMComponent(GEntity* owner) : GComponent(owner)
    {
        Name = "FSM";
        CurrentState = nullptr;
        PreviousState = nullptr;
        GlobalState = nullptr;
    }

    void Start() override;
    void Update(float deltaSeconds) override;

    void SetCurrentState(State<GEntity>* state) { CurrentState = state; }
    void SetGlobalState(State<GEntity>* state) { GlobalState = state; }
    void SetPreviousState(State<GEntity>* state) { PreviousState = state; }

    void ChangeState(State<GEntity>* newState);
    void RevertToPreviousState();
    bool isInState(const State<GEntity>& state) const;

    State<GEntity>* GetCurrentState() const { return CurrentState; }
    State<GEntity>* GetGlobalState() const { return GlobalState; }
    State<GEntity>* GetPreviousState() const { return PreviousState; }

    std::string GetNameOfCurrentState() const;

private:
    State<GEntity>* CurrentState;
    State<GEntity>* PreviousState;
    State<GEntity>* GlobalState;

};
