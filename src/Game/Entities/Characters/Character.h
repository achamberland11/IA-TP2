//
// Created by Antoine on 2026-03-09.
//

#pragma once
#include "../Entity.h"


class GCharacter : public GEntity
{
public:
    GCharacter();
    ~GCharacter() override;

    void Start() override;
    void Update(float dt) override;

};
