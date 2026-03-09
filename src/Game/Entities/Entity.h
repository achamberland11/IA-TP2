//
// Created by Antoine on 2026-03-07.
//

#pragma once
#include <string>
#include <vector>

#include "../Components/Component.h"
#include "../../Core/Object.h"
#include "../Components/RendererComponent.h"
#include "../Components/TransformComponent.h"
#include "SFML/Graphics/RenderWindow.hpp"


class GEntity : public GObject
{
public:
    GEntity();
    virtual ~GEntity() override;

    std::string Name = "Entity";
    bool bActive = true;

    virtual void Start();
    virtual void Update(float dt);
    virtual void Render(sf::RenderWindow& window);

    void AddComponent(GComponent* component);
    void RemoveComponent(GComponent* component);
    std::vector<GComponent*> GetComponents();

    GTransformComponent* GetTransformComponent() const { return Transform; }
    GRendererComponent* GetRendererComponent() const { return Renderer; }

protected:
    std::vector<GComponent*> Components;
    GTransformComponent* Transform;
    GRendererComponent* Renderer;
};
