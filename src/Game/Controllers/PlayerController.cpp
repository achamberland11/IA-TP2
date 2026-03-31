//
// Created by Antoine on 2026-03-09.
//

#include "PlayerController.h"

#include <iostream>

#include "../Entities/Characters/Character.h"
#include "Game/Game.h"

void GPlayerController::Start() {
    GController::Start();
}

void GPlayerController::Update(float dt) {
    GController::Update(dt);

    if (!Owner) return;
    Direction = sf::Vector2f(0.f, 0.f);

    if (bMoveUp) Direction.y = -1.f;
    if (bMoveDown) Direction.y = 1.f;
    if (bMoveLeft) Direction.x = -1.f;
    if (bMoveRight) Direction.x = 1.f;

    if (Direction == sf::Vector2f(0.f, 0.f))
        Owner->StopMoving();
    else {
        const float length = std::sqrt(Direction.x * Direction.x + Direction.y * Direction.y);
        Direction.x /= length;
        Owner->SetVelocity(Direction * Owner->GetMovementSpeed());
        Owner->GetTransformComponent()->SetRotation(Direction);
    }
}

void GPlayerController::HandleEvent(const sf::Event &event) {
    if (!Owner) return;

    if (const auto *keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        switch (keyPressed->code) {
            case sf::Keyboard::Key::W: bMoveUp = true;
                break;
            case sf::Keyboard::Key::S: bMoveDown = true;
                break;
            case sf::Keyboard::Key::A: bMoveLeft = true;
                break;
            case sf::Keyboard::Key::D: bMoveRight = true;
                break;
            case sf::Keyboard::Key::Q: 
                GGame::GetInstance()->GetWorld()->TryInteractSwitch();
                break;
            default: break;
        }
    }

    if (const auto *keyReleased = event.getIf<sf::Event::KeyReleased>()) {
        switch (keyReleased->code) {
            case sf::Keyboard::Key::W: bMoveUp = false;
                break;
            case sf::Keyboard::Key::S: bMoveDown = false;
                break;
            case sf::Keyboard::Key::A: bMoveLeft = false;
                break;
            case sf::Keyboard::Key::D: bMoveRight = false;
                break;
            default: break;
        }
    }

    if (const auto *mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2f worldPos = GGame::GetInstance()->GetWindow().mapPixelToCoords(
        mouseButtonPressed->position
    );

    std::cout << "World pos: (" << worldPos.x << ", " << worldPos.y << ")" << std::endl;
    }
}
