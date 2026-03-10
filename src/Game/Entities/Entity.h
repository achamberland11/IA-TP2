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
    template <typename T>
    T* GetComponent();

    GTransformComponent* GetTransformComponent() const { return Transform; }
    GRendererComponent* GetRendererComponent() const { return Renderer; }

    bool IsCollidingWith(const GEntity* other, float halfWidth) const;
protected:
    std::vector<GComponent*> Components;
    GTransformComponent* Transform;
    GRendererComponent* Renderer;
};

template <typename T>
T* GEntity::GetComponent()
{
    static_assert(std::is_base_of<GComponent, T>::value, "T must derive from GComponent");
    for (GComponent* component : Components)
    {
        if (T* cast = dynamic_cast<T*>(component))
            return cast;
    }
    return nullptr;
}