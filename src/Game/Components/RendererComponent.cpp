//
// Created by Antoine on 2026-03-09.
//

#include "RendererComponent.h"

#include "../Entities/Entity.h"

void GRendererComponent::Start() {
    // Sprite->setScale(Owner->GetTransformComponent()->GetScale());

    Shape.setSize(Owner->GetTransformComponent()->GetScale());
    Shape.setFillColor(Color);
}

void GRendererComponent::Update(float deltaSeconds) {
    // Sprite->setColor(Color);
    // Sprite->setPosition(Owner->GetTransformComponent()->GetPosition());
    // Sprite->setRotation(Owner->GetTransformComponent()->GetRotation());

    Shape.setPosition(Owner->GetTransformComponent()->GetPosition());
    Shape.setFillColor(Color);
}

void GRendererComponent::Render(sf::RenderWindow &window) {
    // window.draw(Sprite);
    window.draw(Shape);
}
