//
// Created by Antoine on 2026-03-09.
//

#pragma once
#include "Character.h"
#include "../../Controllers/PlayerController.h"
#include "SFML/Window/Event.hpp"
#include <map>


class GAgentCharacter : public GCharacter
{
public:
    GAgentCharacter();
    ~GAgentCharacter() override;

    void Start() override;
    void Update(float dt) override;

private:
    void LoadTextures();

    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, std::unique_ptr<sf::Sprite>> m_sprites;

    float spriteTimer = 0.f;
    float spriteDuration = 0.5f;
};
