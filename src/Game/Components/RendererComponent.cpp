//
// Created by Antoine on 2026-03-09.
//

#include "RendererComponent.h"

#include "../Entities/Entity.h"

void GRendererComponent::Start() {
	if (Sprite)
		Sprite->setScale(Owner->GetTransformComponent()->GetScale());
	else
	{
		Shape.setSize(Owner->GetTransformComponent()->GetScale());
		Shape.setFillColor(Color);
	}
}

void GRendererComponent::Update(float deltaSeconds) {

	if (Sprite)
	{
		Sprite->setPosition(Owner->GetTransformComponent()->GetPosition());
		Sprite->setScale(Owner->GetTransformComponent()->GetScale());
	}
	else
	{
		Shape.setPosition(Owner->GetTransformComponent()->GetPosition());
		Shape.setFillColor(Color);
	}
}

void GRendererComponent::Render(sf::RenderWindow& window) {
	if (Sprite)
		window.draw(*Sprite);
	else
		window.draw(Shape);
}
