#pragma once

class IGameEventListener {
public:
    virtual void OnGameOver() = 0;
    virtual void OnGameWon() = 0;
    virtual ~IGameEventListener() = default;
};