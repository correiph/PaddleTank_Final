#include <time.h> 
#include <stdlib.h>
#include "Globals.h"
#include "PowerUpEntity.h"
#include <Box2D/Box2D.h>

#include "TextureAtlas.h"
#include "Entities/EntityManager.h"

PowerUpEntity::PowerUpEntity(b2Body &body, TextureAtlas &atlas, std::string const &powerUpSpriteName, float density)
	: Box2DGameEntity(body, atlas, powerUpSpriteName, FixtureType::RECT, density)
{
	this->SetEntityType(POWERUP_ENTITY);
	EntityMgr->RegisterEntity(this);
	
	//Randomize the type of powerup
	srand(time(NULL));
	m_powerUpType = rand() % 2 + 1;
	b2Fixture* myFixture = m_body->GetFixtureList();
	myFixture->SetSensor(true);
}
PowerUpEntity::~PowerUpEntity()
{

}
void PowerUpEntity::ApplyLinearImpulse(b2Vec2 imp) {
	//Always apply to the center of the tank.
	if (m_powerUpType == GOOD_POWERUP){
		imp.x *= -1.0f;
		imp.y *= -1.0f;
	}
	m_body->ApplyLinearImpulse(imp, m_body->GetPosition(), true);
}

void PowerUpEntity::Action(){

}

int PowerUpEntity::GetPowerUpType(){
	return m_powerUpType;
}

void PowerUpEntity::Update(float delta) {
	//First update the base class, which among other things will set the
	// tank body sprites position based on the box2d bodies position. 
	Box2DGameEntity::Update(delta);
}

void PowerUpEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	Box2DGameEntity::draw(target, states);
}