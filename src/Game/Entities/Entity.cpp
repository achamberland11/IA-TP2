//
// Created by Antoine on 2026-03-07.
//

#include "Entity.h"

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Sprite.hpp"

GEntity::GEntity()
{
    Transform = new GTransformComponent(this);
    Renderer = new GRendererComponent(this);
    AddComponent(Transform);
    AddComponent(Renderer);
}

GEntity::~GEntity()
{
    for (auto component: Components) delete component;
    Components.clear();
}

void GEntity::Start()
{
    for (auto component: Components) component->Start();
}

void GEntity::Update(float dt)
{
    for (auto component: Components) component->Update(dt);
}

void GEntity::Render(sf::RenderWindow &window)
{
    if (!bActive || Transform == nullptr) return;

    Renderer->Render(window);
}

void GEntity::AddComponent(GComponent *component)
{
    Components.push_back(component);
}

void GEntity::RemoveComponent(GComponent *component)
{
    auto it = std::find(Components.begin(), Components.end(), component);
    if (it != Components.end()) Components.erase(it);
}

std::vector<GComponent *> GEntity::GetComponents()
{
    return Components;
}

bool GEntity::IsCollidingWith(const GEntity* other, float halfWidth) const
{
    sf::Vector2f pos = GetTransformComponent()->GetPosition();
    sf::Vector2f otherPos = other->GetTransformComponent()->GetPosition();

    return (std::abs(pos.x - otherPos.x) < halfWidth * 2) &&
        (std::abs(pos.y - otherPos.y) < halfWidth * 2);
}
