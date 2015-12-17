#include "Globals.h"
#include "Box2DGameEntity.h"
#include "vector2_math_utils.h"
#include "TextureAtlas.h"
#include "Entities/EntityManager.h"

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

#include <iostream>

Box2DGameEntity::Box2DGameEntity(b2Body &body, TextureAtlas &atlas, std::string const &spriteName, FixtureType type, float density, float restitution, bool lockRotationToDirection)
	: BaseGameEntity(BaseGameEntity::GetNextValidID()), m_body(&body), m_sprite(nullptr), m_atlas(&atlas), m_lockRotationToDirection(lockRotationToDirection)
{
	//Set the entity type.
	this->SetEntityType(BOX2D_ENTITY);
	//Register the entity so it can be sent messages.
	EntityMgr->RegisterEntity(this);

	//Use copy constructor to get a copy of the sprite.
	m_sprite = new sf::Sprite(*m_atlas->GetSprite(spriteName));
	
	//Create a fixture based on the dimensions of the sprite.
	sf::IntRect r = m_sprite->getTextureRect();
	switch (type) {
	case RECT: {
		b2PolygonShape pShape;
		//Don't forget that setBox takes the half-width and half-height.
		pShape.SetAsBox(r.width * METERS_PER_PIXEL * 0.5f, r.height* METERS_PER_PIXEL * 0.5f);
		b2FixtureDef fd;
		fd.density = density;
		fd.restitution = restitution;
		fd.shape = &pShape;
		m_body->CreateFixture(&fd);
		break; }
	case CIRCLE: {
		b2CircleShape cShape;
		//If a circle is called for use the smallest dimension of the sprite as the diameter.
		cShape.m_radius = (r.width < r.height ? r.width : r.height) * METERS_PER_PIXEL * 0.5f;
		b2FixtureDef fd;
		fd.density = density;
		fd.restitution = restitution;
		fd.shape = &cShape;
		m_body->CreateFixture(&fd);
		break; }
	case NONE:
	default:
		//No fixture created here.
		break;
	}
	//Set the sprites origin to the center so it matches the box2d centroid.
	m_sprite->setOrigin(r.width * 0.5f, r.height * 0.5f);
	//Set the sprite to the correct position and orientation.
	m_sprite->setPosition(vec2utils::ConvertVectorType<b2Vec2, sf::Vector2f>(m_body->GetPosition()) * PIXELS_PER_METERS);
	m_sprite->setRotation(m_body->GetAngle() * DEGREES_PER_RADIANS);

	m_body->SetUserData(this);
}


Box2DGameEntity::~Box2DGameEntity()
{
	//Clean up the sprite
	delete m_sprite;
	m_sprite = nullptr;
	//Remove the body from the physics simulation
	b2World *world = m_body->GetWorld();
	world->DestroyBody(m_body);
	m_body = nullptr;
	//Deregister the entity so it no longer gets mail.
	EntityMgr->RemoveEntity(this);
}

void Box2DGameEntity::Update(float delta) {
	//Move the sprite to the same position and orientation as the b2Body.
	m_sprite->setPosition(vec2utils::ConvertVectorType<b2Vec2, sf::Vector2f>(m_body->GetPosition()) * PIXELS_PER_METERS);
	if (m_lockRotationToDirection) {
		b2Vec2 vel = m_body->GetLinearVelocity();
		m_sprite->setRotation(std::atan2(vel.y, vel.x) * DEGREES_PER_RADIANS);
	}
	else {
		m_sprite->setRotation(m_body->GetAngle() * DEGREES_PER_RADIANS);
	}
}

void Box2DGameEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(*m_sprite, states);
}

sf::Vector2f Box2DGameEntity::GetPosition() {
	return vec2utils::ConvertVectorType<b2Vec2, sf::Vector2f>(m_body->GetPosition()) * PIXELS_PER_METERS;
}