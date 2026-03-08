//
// Created by Antoine on 2026-03-07.
//

#include "Entity.h"

GEntity::GEntity()
{
    Transform = new GTransformComponent();
    // GTransformComponent::StaticClass().Factory(this);
}

GEntity::~GEntity()
{
    for (auto component : Components) delete component;
    Components.clear();
}

void GEntity::AddComponent(GComponent* component)
{
    Components.push_back(component);
}

void GEntity::RemoveComponent(GComponent* component)
{
    auto it = std::find(Components.begin(), Components.end(), component);
    if (it != Components.end()) Components.erase(it);
}

std::vector<GComponent*> GEntity::GetComponents()
{
    return Components;
}