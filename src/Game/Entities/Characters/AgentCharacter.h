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
#include "Game/Components/ConeVisionComponent.h"


class GAgentCharacter : public GCharacter
{
public:
    GAgentCharacter();
    ~GAgentCharacter() override;

    void Start() override;
    void Update(float dt) override;
    void DrawDebug(sf::RenderWindow& window);

    // Vision-related methods
    void SetupVisionComponent(float range = 150.f, float angle = 90.f);
    std::vector<GEntity*> GetVisibleEntities(const std::vector<GEntity*>& potentialTargets) const;

private:
    void LoadTextures();

    GFSMComponent* FSM = nullptr;
    GConeVisionComponent* VisionComponent = nullptr;

    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, std::unique_ptr<sf::Sprite>> m_sprites;

    float spriteTimer = 0.f;
    float spriteDuration = 0.5f;
};
