//
// Created by Antoine on 2026-03-09.
//

#include "PlayerController.h"

#include "../Entities/Characters/Character.h"

void GPlayerController::Start() {
    GController::Start();
}

void GPlayerController::Update(float dt) {
    GController::Update(dt);

    if (!Owner) return;

    constexpr float speed = 100.0f; // @TODO speed hardcoded, should be in PlayerCharacter
    sf::Vector2f direction(0.f, 0.f);

    if (bMoveUp) direction.y = -1.f;
    if (bMoveDown) direction.y = 1.f;
    if (bMoveLeft) direction.x = -1.f;
    if (bMoveRight) direction.x = 1.f;

    if (direction == sf::Vector2f(0.f, 0.f))
        Owner->StopMoving();
    else {
        const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        direction.x /= length;
        Owner->SetVelocity(direction * speed);
    }
}

void GPlayerController::HandleEvent(const sf::Event &event) {
    if (!Owner) return;

    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        switch (keyPressed->code) {
            case sf::Keyboard::Key::W: bMoveUp = true; break;
            case sf::Keyboard::Key::S: bMoveDown = true; break;
            case sf::Keyboard::Key::A: bMoveLeft = true; break;
            case sf::Keyboard::Key::D: bMoveRight = true; break;
            default: break;
        }
    }

    if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>()) {
        switch (keyReleased->code) {
            case sf::Keyboard::Key::W: bMoveUp = false; break;
            case sf::Keyboard::Key::S: bMoveDown = false; break;
            case sf::Keyboard::Key::A: bMoveLeft = false; break;
            case sf::Keyboard::Key::D: bMoveRight = false; break;
            default: break;
        }
    }

}
