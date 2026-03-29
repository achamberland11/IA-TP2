//
// Created by Antoine on 2026-03-09.
//

#pragma once
#include "Character.h"
#include "../../Controllers/PlayerController.h"
#include "SFML/Window/Event.hpp"
#include <map>
#include <Game/GameEventListener.h>


class GPlayerCharacter : public GCharacter
{
public:
    GPlayerCharacter();
    ~GPlayerCharacter() override;

    void Start() override;
    void Update(float dt) override;

    void SetListener(IGameEventListener* l) { Listener = l; }

private:
    void LoadTextures();

    std::map<std::string, sf::Texture> Textures;
    std::map<std::string, std::unique_ptr<sf::Sprite>> Sprites;

    float SpriteTimer = 0.f;
    float SpriteDuration = 0.05f;

    IGameEventListener* Listener;
};
