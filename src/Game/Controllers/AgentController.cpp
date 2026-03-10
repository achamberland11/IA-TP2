//
// Created by Antoine on 2026-03-09.
//

#include "AgentController.h"


void GAgentController::Start() {
    GController::Start();
    
    if (!Owner) return;
    
    FSM = Owner->GetComponent<GFSMComponent>();

    assert(FSM && "<GAgentController> : FSMComponent not found !");
    FSM->SetCurrentState(AgentPatrolState::Instance());
    FSM->SetGlobalState(AgentGlobalStates::Instance());
}

void GAgentController::Update(float dt) {
	if (!Owner)
		return;

	if(FSM)
		FSM->Update(dt);

	sf::Vector2f steering = ComputeSteering(dt);
	Owner->SetVelocity(steering);

	//Owner->Update(dt);
}

void GAgentController::HandleEvent(const sf::Event &event) {
    if (!Owner) return;
}

sf::Vector2f GAgentController::ComputeSteering(float dt)
{
	if (targets.empty() || currentTarget >= targets.size())
		return { 0.f, 0.f };

	sf::Vector2f position = Owner->GetTransformComponent()->GetPosition();
	direction = targets[currentTarget] - position;
	float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	//Arrive behavior
	float speed = Owner->GetMovementSpeed();
	if (distance < SlowingRadius)
		speed *= distance / SlowingRadius;

	if (distance < 2.f) {
		if (currentTarget + 1 < targets.size())
			currentTarget++;
		else
			return { 0.f, 0.f };
	}

	direction /= distance;

	return direction * speed;
}
