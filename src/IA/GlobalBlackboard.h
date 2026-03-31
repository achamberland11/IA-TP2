//
// Created by dreux on 30-03-26.
//
#pragma once
#include <cstdlib>

#include "SFML/System/Vector2.hpp"

struct RadioInfo{
    bool bPlayerSeen = false;
    sf::Vector2f PlayerLastPosition;
    int reportingAgentID = -1;
};

class GlobalBlackboard{
private:
    RadioInfo readBuffer;
    RadioInfo writeBuffer;
    float interferenceChance = 0.15f;

public:
    float RadioMaxDist = 300.f;

    float GetDistToReportingAgent(sf::Vector2f agentPos, int agentID) const;

    void ChangeInterferenceChance(float newChance) {
        interferenceChance = newChance;
    }

    void ResetInterferenceChance() {
        interferenceChance = 0.25f;
    }

    const RadioInfo& ListenToRadio() const {return readBuffer;}

    void TryBroadcastToRadio(sf::Vector2f const PlayerPos, int const agentID){
        writeBuffer.bPlayerSeen = true;
        writeBuffer.PlayerLastPosition = PlayerPos;
        writeBuffer.reportingAgentID = agentID;
    }

    void SyncRadio() //done at end of frame
    {
        if (rand() % 100 > interferenceChance) {
            readBuffer = writeBuffer;
        }
    }
};