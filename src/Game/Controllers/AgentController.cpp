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

	sf::Vector2f agentPos = Owner->GetTransformComponent()->GetPosition();
	sf::Vector2f playerPos = Player->GetTransformComponent()->GetPosition();
	sf::Vector2f playerVel = Player->GetVelocity();

	//Calculate prediction time
	sf::Vector2f toPlayer = playerPos - agentPos;
	float distance = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);

	//Predict
	float agentSpeed = Owner->GetMovementSpeed();
	float predictionTime = (agentSpeed > 0.f) ? distance / agentSpeed : 0.f;

	const float MaxPrediction = 0.5f;

	if (predictionTime > MaxPrediction)
		predictionTime = MaxPrediction;

	sf::Vector2 predictedPos = playerPos + playerVel * predictionTime;

	direction = predictedPos - agentPos;
	float dist = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	if (dist > 0.f)
		direction /= dist;
	else 
		return { 0.f, 0.f };


	//Arrive behavior
	float speed = Owner->GetMovementSpeed();
	if (distance < SlowingRadius)
		speed *= distance / SlowingRadius;

	return direction * speed;
}
