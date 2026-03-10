//
// Created by Antoine on 2026-03-09.
//

#pragma once
#include "Character.h"
#include "../../Controllers/PlayerController.h"
#include "SFML/Window/Event.hpp"
#include <map>
#include <vector>

#include "Game/Components/FSMComponent.h"


class GAgentCharacter : public GCharacter
{
public:
    GAgentCharacter();
    ~GAgentCharacter() override;

    void Start() override;
    void Update(float dt) override;
    void DrawDebug(sf::RenderWindow& window);

private:
    void LoadTextures();

    GFSMComponent* FSM = nullptr;

    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, std::unique_ptr<sf::Sprite>> m_sprites;

    float spriteTimer = 0.f;
    float spriteDuration = 0.5f;

    std::vector<sf::Vector2f> targets = { sf::Vector2f(500, 300), sf::Vector2f(600, 400) };
    int currentTarget = 0;
    bool bFinished = false;
};
