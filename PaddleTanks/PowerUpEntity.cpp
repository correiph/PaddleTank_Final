#include <time.h> 
#include <stdlib.h>
#include "Globals.h"
#include "PowerUpEntity.h"


#include "TextureAtlas.h"
#include "Entities/EntityManager.h"

PowerUpEntity::PowerUpEntity(b2Body &body, TextureAtlas &atlas, std::string const &powerUpSpriteName, float density)
	: Box2DGameEntity(body, atlas, powerUpSpriteName, FixtureType::RECT, density)
{
	this->SetEntityType(POWERUP_ENTITY);
	EntityMgr->RegisterEntity(this);
	
	//Randomize the type of powerup
	m_powerUpType = rand() % 2 + 1;
}
PowerUpEntity::~PowerUpEntity()
{

}

void PowerUpEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	Box2DGameEntity::draw(target, states);
}