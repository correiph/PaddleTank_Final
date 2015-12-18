#include "Globals.h"
#include "vector2_math_utils.h"

#include "PaddleTankEntityStates.h"

#include "PaddleTankGameEntity.h"
#include "InputManager.h"
#include "Messaging\MessageDispatcher.h"
#include "message_types.h"
#include "time.h"

#include <Box2D/Box2D.h>

#pragma region HumanEntityState
// Begin Class
// PaddleTankHumanControlledEntityState
//
void PaddleTankHumanControlledEntityState::Enter(PaddleTankGameEntity *entity) {
	//REgister needed keys.
	G_InputManager.RegisterKey(sf::Keyboard::Space); 
	G_InputManager.RegisterKey(sf::Keyboard::W);
	G_InputManager.RegisterKey(sf::Keyboard::S);
	m_idleTime = 0.0f;
}

//this is the states normal update function
void PaddleTankHumanControlledEntityState::Execute(PaddleTankGameEntity *entity, float delta) {
	m_notIdle = false;
	//Check for user up and down keypresses and moves the tank accordingly.
	if (G_InputManager.isKeyDown(sf::Keyboard::W)) {
		entity->ApplyLinearImpulse(b2Vec2(0.0f, -PADDLE_TANK_IMPULSE_POWER));
		m_notIdle = true;
	}
	if (G_InputManager.isKeyDown(sf::Keyboard::S)) {
		entity->ApplyLinearImpulse(b2Vec2(0.0f, PADDLE_TANK_IMPULSE_POWER));
		m_notIdle = true;
	}
	//Get the mouse position and calculate the correct angle for the turret
	sf::Vector2f mousePos = vec2utils::ConvertVectorType<sf::Vector2i, sf::Vector2f>(G_InputManager.mousePosition());
	sf::Vector2f tankPos = entity->GetPosition();
	sf::Vector2f bdir = mousePos - tankPos;
	sf::Vector2f normBdir = vec2utils::NormalOf<sf::Vector2f, sf::Vector2f>(bdir);
	float angleRads = std::atan2(normBdir.y, normBdir.x);
	entity->SetTurretAngle(angleRads);
	//Fires the gun if a shot is ready.
	if (G_InputManager.isKeyDown(sf::Keyboard::Space) || G_InputManager.isMouseButtonDown(sf::Mouse::Left)) {
		if (entity->IsShotReady() && entity->BulletsInMagazine()) {
			entity->Shoot();
		}
		m_notIdle = true;
	}
	if (m_notIdle == false)
		m_idleTime += delta;
	else
		m_idleTime = 0.0f;

	if (m_idleTime >= 1.0f)
		Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY, 0, entity->ID(), message_type::TOIDLE, nullptr);
}

//this will execute when the state is exited. 
void PaddleTankHumanControlledEntityState::Exit(PaddleTankGameEntity *entity) {
	//Deregister the keys.
	G_InputManager.DeregisterKey(sf::Keyboard::Space);
	G_InputManager.DeregisterKey(sf::Keyboard::W);
	G_InputManager.DeregisterKey(sf::Keyboard::S);
}

//this executes if the agent receives a message from the 
//message dispatcher
bool PaddleTankHumanControlledEntityState::OnMessage(PaddleTankGameEntity *entity, const Telegram& msg) {
	return false;
}

//
// PaddleTankHumanControlledEntityState
// End Class
#pragma endregion
//Feel free to make more states
#pragma region HumanIdleState
// Begin Class
// PaddleTankHumanControlledEntityState
//
void PaddleTankHumanControlledIdleState::Enter(PaddleTankGameEntity *entity) {
	//REgister needed keys.
	G_InputManager.RegisterKey(sf::Keyboard::Space);
	G_InputManager.RegisterKey(sf::Keyboard::W);
	G_InputManager.RegisterKey(sf::Keyboard::S);
}

//this is the states normal update function
void PaddleTankHumanControlledIdleState::Execute(PaddleTankGameEntity *entity, float delta) {
	m_notIdle = false;
	//Check for user up and down keypresses and moves the tank accordingly.
	if (G_InputManager.isKeyDown(sf::Keyboard::W)) {
		entity->ApplyLinearImpulse(b2Vec2(0.0f, -PADDLE_TANK_IMPULSE_POWER));
		//Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY, 0, entity->ID(), message_type::TOACTIVE, nullptr);
		m_notIdle = true;
	}
	if (G_InputManager.isKeyDown(sf::Keyboard::S)) {
		entity->ApplyLinearImpulse(b2Vec2(0.0f, PADDLE_TANK_IMPULSE_POWER));
		//Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY, 0, entity->ID(), message_type::TOACTIVE, nullptr);
		m_notIdle = true;
	}
	//Get the mouse position and calculate the correct angle for the turret
	sf::Vector2f mousePos = vec2utils::ConvertVectorType<sf::Vector2i, sf::Vector2f>(G_InputManager.mousePosition());
	sf::Vector2f tankPos = entity->GetPosition();
	sf::Vector2f bdir = mousePos - tankPos;
	sf::Vector2f normBdir = vec2utils::NormalOf<sf::Vector2f, sf::Vector2f>(bdir);
	float angleRads = std::atan2(normBdir.y, normBdir.x);
	entity->SetTurretAngle(angleRads);
	//Fires the gun if a shot is ready.
	if (G_InputManager.isKeyDown(sf::Keyboard::Space) || G_InputManager.isMouseButtonDown(sf::Mouse::Left)) {
		if (entity->IsShotReady() && entity->BulletsInMagazine()) {
			entity->Shoot();
		}
		//Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY, 0, entity->ID(), message_type::TOACTIVE, nullptr);
		m_notIdle = true;
	}
	if (m_notIdle)
	{
		Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY, 0, entity->ID(), message_type::TOACTIVE, nullptr);
	}
	else
	{
		entity->getStats()->RegenerateHealth(HEALTH_REGEN_RATE);
		entity->getStats()->RegeneratePower(POWER_REGEN_RATE);
	}
}

//this will execute when the state is exited. 
void PaddleTankHumanControlledIdleState::Exit(PaddleTankGameEntity *entity) {
	//Deregister the keys.
	G_InputManager.DeregisterKey(sf::Keyboard::Space);
	G_InputManager.DeregisterKey(sf::Keyboard::W);
	G_InputManager.DeregisterKey(sf::Keyboard::S);
	
}

//this executes if the agent receives a message from the 
//message dispatcher
bool PaddleTankHumanControlledIdleState::OnMessage(PaddleTankGameEntity *entity, const Telegram& msg) {
	return false;
}

//
// PaddleTankHumanControlledIdleState
// End Class
#pragma endregion

#pragma region AIState
// PaddleTankAIControlledEntity

//this is the states normal update function
void PaddleTankAIControlledEntityState::Enter(PaddleTankGameEntity *entity) {
	srand(time(NULL));
}

void PaddleTankAIControlledEntityState::Execute(PaddleTankGameEntity *entity, float delta) {
	
	StrafeUpAndDown(entity, delta);
	AimAtPlayer(entity);

	// Dispatcher->DispatchDelayedMessages(delta);

}

//this will execute when the state is exited. 
void PaddleTankAIControlledEntityState::Exit(PaddleTankGameEntity *entity) {

}

//this executes if the agent receives a message from the 
//message dispatcher
bool PaddleTankAIControlledEntityState::OnMessage(PaddleTankGameEntity *entity, const Telegram& msg) {
	return false;
}

void PaddleTankAIControlledEntityState::StrafeUpAndDown(PaddleTankGameEntity *entity, float delta) {
	M_RANDOMFORCEDURATION = LOW_VALUE + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HIGH_VALUE - LOW_VALUE)));


	if (M_CURRENTTIME <= M_STRAFETIME) {
		M_CURRENTTIME += delta;
	}
	else {
		if (M_CURRENTFORCETIME < M_APPLYFORCEDURATION) {
			entity->ApplyLinearImpulse(b2Vec2(0.0f, M_STRAFEDIRECTION * -PADDLE_TANK_IMPULSE_POWER));
			M_CURRENTFORCETIME += delta;
		}
		else {
			M_STRAFEDIRECTION *= -1;
			M_CURRENTTIME = 0.0f;
			M_CURRENTFORCETIME = 0.0f - M_RANDOMFORCEDURATION;
		}
	}
}

void PaddleTankAIControlledEntityState::AimAtPlayer(PaddleTankGameEntity *entity) {
	sf::Vector2f mousePos = vec2utils::ConvertVectorType<sf::Vector2i, sf::Vector2f>(G_InputManager.mousePosition());
	sf::Vector2f tankPos = entity->GetPosition();
	sf::Vector2f bdir = mousePos - tankPos;
	sf::Vector2f normBdir = vec2utils::NormalOf<sf::Vector2f, sf::Vector2f>(bdir);
	float angleRads = std::atan2(normBdir.y, normBdir.x);
	entity->SetTurretAngle(angleRads);
}



#pragma endregion

