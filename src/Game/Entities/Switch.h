#pragma once
#include "Entity.h"
#include <Game/GameEventListener.h>
#include <map>

enum SwitchStatus {
    On, Off
};
class GSwitch : public GEntity {
public:
    GSwitch();
    ~GSwitch() override;

    void Start() override;
    void Update(float dt) override;

    void SetListener(IGameEventListener* l) { Listener = l; }

    SwitchStatus GetStatus() { return Status; }
    void Interact();

private:
    void LoadTextures();

    std::map<std::string, sf::Texture> Textures;
    std::map<std::string, std::unique_ptr<sf::Sprite>> Sprites;

    IGameEventListener* Listener;

    SwitchStatus Status;
};