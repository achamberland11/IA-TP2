//
// Created by Antoine on 2026-03-09.
//

#pragma once
#include "Character.h"
#include <map>

#include "../../Components/AI/FSMComponent.h"
#include "Game/Components/ConeVisionComponent.h"
#include "Game/Controllers/AgentController.h"
#include "Game/GameEventListener.h"
#include "Game/Components/AI/GOBComponent.h"

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

    void SetRoom(FRoom r)
    {
        Room = r;
        bHasRoom = true;
    }

    std::vector<sf::Vector2f> GetWaypoints() const { return Waypoints; }

    void SetWaypoints(const std::vector<sf::Vector2f> &waypoints);

    std::vector<sf::Vector2f> GetPatrolPoints() const { return PatrolPoints; }
    int GetPatrolIndex() const { return PatrolIndex; }
    void SetPatrolIndex(int index) { PatrolIndex = index; }

    void SetPatrolWaypoints(int PixelsPerTile);

    // Vision-related methods
    void SetupVisionComponent(float range = 150.f, float angle = 90.f);

    GAgentController *GetAgentController() const { return AgentController; }
    // GFSMComponent *GetFSM() const { return FSM; }
    GGOBComponent *GetGOB() const { return GOB; }

    void SetListener(IGameEventListener *l) { Listener = l; }

    int GetTireness() const { return Tireness; }
    void IncreaseTireness(int amount) { Tireness += amount; }
    void ResetTireness() { Tireness = 0; }
    bool IsPlayerVisible() const { return bPlayerVisible; }
    bool IsInRoom() const { return Room.Contains(GetTransformComponent()->GetPosition()); }

private:
    void LoadTextures();

    sf::Vector2f ComputeSteering(float dt);

    // GFSMComponent *FSM = nullptr;
    GGOBComponent *GOB = nullptr;
    GConeVisionComponent *VisionComponent = nullptr;

    std::map<std::string, sf::Texture> Textures;
    std::map<std::string, std::unique_ptr<sf::Sprite> > Sprites;

    GAgentController *AgentController = nullptr;

    int Tireness = 0.0f;
    bool bPlayerVisible = false;

    sf::Font Font;
    bool bFontLoaded = false;

    float SpriteTimer = 0.f;
    float SpriteDuration = 0.5f;

    std::vector<sf::Vector2f> PatrolPoints;
    std::vector<sf::Vector2f> Waypoints;
    sf::Vector2f CurrentWaypoint;
    int WaypointIndex = 0;
    int PatrolIndex = 0;

    //Steering
    const float SlowingRadius = 50.f;
    sf::Vector2f Direction;

    IGameEventListener *Listener = nullptr;
    FRoom Room;
    bool bHasRoom = false;
};
