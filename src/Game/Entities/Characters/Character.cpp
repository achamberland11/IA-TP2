//
// Created by Antoine on 2026-03-09.
//

#include "Character.h"


GCharacter::GCharacter() {
    Name = "Character";
}

void GCharacter::Start() {
    GEntity::Start();
}

void GCharacter::Update(float dt) {
    GEntity::Update(dt);
}
