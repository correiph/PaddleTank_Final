#include "Globals.h"

#include "PaddleTankGameEntity.h"
#include "PaddleTankEntityStates.h"

#include "FSM/EntityStateMachine.h"
#include "vector2_math_utils.h"
#include "InputManager.h"
#include "TextureAtlas.h"
#include "Messaging\MessageDispatcher.h"
#include "message_types.h"

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

PaddleTankGameEntity::PaddleTankGameEntity(b2Body &body, TextureAtlas &atlas, std::string const &tankSpriteName, std::string const &barrelSpriteName, float density, EntityState<PaddleTankGameEntity> *startState)
	: Box2DGameEntity(body, atlas, tankSpriteName, FixtureType::RECT, density), m_stateMachine(nullptr),
	m_barrelSprite(nullptr), m_mapEntID(-1), m_shotReady(true), m_magazine(5)
{
	this->SetEntityType(PADDLE_TANK_ENTITY);
	//Set the sprite to the correct position. Don't forget to convert units.
	m_barrelSprite = new sf::Sprite(*atlas.GetSprite(barrelSpriteName));
	//Set the origin to the middle left edge of the sprite so it rotates properly.
	m_barrelSprite->setOrigin(0.0f, m_barrelSprite->getTextureRect().height * 0.5f);
	m_barrelSprite->setPosition(m_sprite->getPosition());
	//Create the state machine and set the starting state if one was specified.
	m_stateMachine = new StateMachine<PaddleTankGameEntity>(this);
	if (startState) {
		m_stateMachine->SetCurrentState(startState);
		m_stateMachine->CurrentState()->Enter(this);
	}
}

PaddleTankGameEntity::~PaddleTankGameEntity()
{
	//clean up.
	delete m_barrelSprite;
	delete m_stateMachine;
}

void PaddleTankGameEntity::Update(float delta) {
	//First update the base class, which among other things will set the
	// tank body sprites position based on the box2d bodies position. 
	Box2DGameEntity::Update(delta);
	//Set the barrel to the correct position.
	m_barrelSprite->setPosition(m_sprite->getPosition());
	//Update the state machine. This will, among other things, handle
	// the barrels rotation.
	m_stateMachine->Update(delta);
}

bool PaddleTankGameEntity::HandleMessage(const Telegram& msg) {
	switch (msg.Msg) {
	case message_type::BULLETREADY:
		m_shotReady = true;
		return true;
	case message_type::RELOAD:
		m_magazine = 5;
		return true;
	case message_type::TOACTIVE:
		m_stateMachine->ChangeState(PaddleTankHumanControlledEntityState::Instance());
		return true;
	case message_type::TOIDLE:
		m_stateMachine->ChangeState(PaddleTankHumanControlledIdleState::Instance());
		return true;
	default:
		//You remember how the short circuiting works on the logical or right?
		return m_stateMachine->HandleMessage(msg) || Box2DGameEntity::HandleMessage(msg);
	}
	return false;
}

void PaddleTankGameEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	Box2DGameEntity::draw(target, states);
	target.draw(*m_barrelSprite, states);
}

void PaddleTankGameEntity::ApplyLinearImpulse(b2Vec2 const &impulse) {
	//Always apply to the center of the tank.
	m_body->ApplyLinearImpulse(impulse, m_body->GetPosition(), true);
}

void PaddleTankGameEntity::SetTurretAngle(float angleRads) {
	//Set the barrel sprite.
	m_barrelSprite->setRotation(angleRads * DEGREES_PER_RADIANS);
}

void PaddleTankGameEntity::Shoot() {
	//Calculate the correct position to spawn the bullet and the correct facing for the bullet.
	float barrelDirRads = m_barrelSprite->getRotation() * RADIANS_PER_DEGREES;
	sf::Vector2f normBdir(std::cos(barrelDirRads), std::sin(barrelDirRads));
	m_shotReady = false;
	SpawnBulletData *spawnData = new SpawnBulletData();
	//The +18 is to account for the fact that if you spawn the bullet such that it's centroid is at the
	// far top of the barrel of the tank gun, the bullet may immediately hit the tank when the barrel is
	// over one of the tanks corners. The extra little bit ensures it is never spawned in a position where it
	// collides with the spawning tank.
	spawnData->Position = normBdir * ((float)m_barrelSprite->getTextureRect().width+18) + m_barrelSprite->getPosition();
	spawnData->Direction = normBdir;
	//Fire the shot
	Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY, this->ID(), m_mapEntID, message_type::SPAWN_BULLET, spawnData);
	//Reload!
	Dispatcher->DispatchMsg(MIN_TIME_BETWEEN_SHOTS, this->ID(), this->ID(), message_type::BULLETREADY, nullptr);
	// only send the reload magazine message if there are 5 shots in magazine when first is fired
	if (m_magazine == 5)
	{
		Dispatcher->DispatchMsg(MAGAZINE_RELOAD_TIME, this->ID(), this->ID(), message_type::RELOAD, nullptr);
	}
	m_magazine--;

}
//void PaddleTankGameEntity::HealthStat(int health) {
//	if (health >= 0) {
//		~PADDLE_TANK_ENTITY;
//	}
//}