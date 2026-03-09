//
// Created by Antoine on 2026-03-07.
//

#pragma once
#include <string>
#include <vector>

#include "../Components/Component.h"
#include "../../Core/Object.h"
#include "../Components/TransformComponent.h"


class GEntity : public GObject
{
public:
    GEntity();
    virtual ~GEntity();

    std::string Name = "Entity";
    bool bActive = true;

    virtual void Start();
    virtual void Update(float dt);

    void AddComponent(GComponent* component);
    void RemoveComponent(GComponent* component);
    std::vector<GComponent*> GetComponents();

    GTransformComponent* GetTransformComponent() { return Transform; }

protected:
    std::vector<GComponent*> Components;
    GTransformComponent* Transform;
};
