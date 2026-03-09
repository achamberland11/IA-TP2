//
// Created by Antoine on 2026-03-07.
//

#pragma once
#include <string>
#include "../../Core/Object.h"

class GEntity;

class GComponent : public GObject
{
public:
    GENERATE_COMPONENT(true, true, true)

    GComponent(GEntity* owner) : Owner(owner) {}
    virtual ~GComponent() = default;

    virtual void Start() = 0;
    virtual void Update(float deltaSeconds) = 0;

    bool CanDuplicate() const { return bCanDuplicate; }
    bool CanBeDisabled() const { return bCanBeDisabled; }
    bool CanBeDeleted() const { return bCanBeDeleted; }


    std::string Name = "Component";
    bool bEnabled = true;

protected:
    GEntity* Owner;
};
