//
// Created by Antoine on 2026-03-09.
//

#pragma once
#include "Character.h"
#include <map>

#include "Game/Components/FSMComponent.h"
#include "Game/Components/ConeVisionComponent.h"

class GConeVisionComponent;

class GAgentCharacter : public GCharacter
{
public:
    GAgentCharacter();
    ~GAgentCharacter() override;

    void Start() override;
    void Update(float dt) override;
    void Render(sf::RenderWindow &window) override;
    void DrawDebug(sf::RenderWindow &window);

    std::vector<sf::Vector2f> GetWaypoints() const { return waypoints; }
    bool WaypointReached() const { return bWaypointReached; }

    // Vision-related methods
    void SetupVisionComponent(float range = 150.f, float angle = 90.f);

private:
    void LoadTextures();

    void SetWaypoints();

    GFSMComponent *FSM = nullptr;
    GConeVisionComponent* VisionComponent = nullptr;

    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, std::unique_ptr<sf::Sprite> > m_sprites;

    float spriteTimer = 0.f;
    float spriteDuration = 0.5f;

    std::vector<sf::Vector2f> waypoints;

    bool bWaypointReached = false;
};