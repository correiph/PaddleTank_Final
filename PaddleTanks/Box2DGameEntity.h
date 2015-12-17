#pragma once
#include "Entities/BaseGameEntity.h"

#include <SFML/Graphics/Drawable.hpp>

#include <string>

class TextureAtlas;

class b2Body;
namespace sf {
	class Sprite;
}

/// <summary>
/// Box2DGameEntity encapsulates an entity that is rendered using SFML
/// and is simulated using Box2D. Due to the fact that it contains
/// a b2Body and that it deletes it in the destructor it is vital
/// that entities of this type not be deleted during the simulation
/// of the box2d world. i.e. any code deleting a Box2DGameEntity should
/// occur not be in the same call stack as b2World::Step().
/// Be aware that the b2Body needs to be created outside the entity and
/// that the entity stores a reference to that create body. You should
/// also be aware that the constructor will create a fixture that corresponds
/// in size to the sprite drawn from the texture atlas. The shape of the fixture
/// will either be rectangular or circular depending on the fixture type
/// value passed in.
/// Be aware of your units!
/// Ensure that your centroids (box2d) and origins (sfml) line up.
/// </summary>
/// <author>
/// Kevin Forest 2015
/// </author>
class Box2DGameEntity :
	public BaseGameEntity, public sf::Drawable
{
public:
	//Fixture type is needed so that the body knows
	// what shape of fixture needs to be attached
	// to the body.
	enum FixtureType {NONE, RECT, CIRCLE};
public:	
	/// <summary>
	/// Initializes a new instance of the <see cref="Box2DGameEntity"/> class. Creates a fixture and
	/// a sprite and attaches them to the body.
	/// </summary>
	/// <param name="body">The body.</param>
	/// <param name="atlas">The texture atlas that contains the sprite.</param>
	/// <param name="spriteName">Name of the sprite in the atlas.</param>
	/// <param name="type">The shape the fixture will take.</param>
	/// <param name="density">The density of the resulting object.</param>
	Box2DGameEntity(b2Body &body, TextureAtlas &atlas, std::string const &spriteName, FixtureType type = NONE, float density = 1.0f, float restitution = 0.0f, bool lockRotationToDirection = false);
	virtual ~Box2DGameEntity();

	//Update this entity. Note that the position of the entity will
	// change automatically based on the forces being applied to it
	// as part of the physics simulation. This method should be used for
	// getting user input, applying external forces, and reacting to events.
	virtual void Update(float delta);

	virtual sf::Vector2f GetPosition();

protected:
	//Render the sprite.
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	//The game entities representation in the physics world and
	// the visible world. 
	b2Body *m_body;
	sf::Sprite *m_sprite;
	bool m_lockRotationToDirection;
	//In case you need to grab a different sprite. If you do don't forget
	// to set the origin properly and change the fixture size if necessary.
	TextureAtlas *m_atlas;

};

