#pragma once
#include "Box2DGameEntity.h"

struct b2Vec2;

class Map;
class PaddleTankGameEntity;

template<class entity_type>
class StateMachine;
template <class entity_type>
class EntityState;

/// <summary>
/// PaddleTankGameEntity encapsulates a paddle tank.
/// A PaddleTank has a turret that pivots around the tanks centroid.
/// Uses a statemachine for controls - a human control state machine is
/// provided. The state machine allows for global states although
/// none are currently implemented.
/// You control a tank by applying linear impulses to it - this can
/// be done as a result of keyboard input or based on a decision made
/// by an AI state machine or other controller.
/// </summary>
/// <author>
/// Kevin Forest 2015
/// </author>
class PaddleTankGameEntity :
	public Box2DGameEntity
{
public:
	//Create a tank.
	PaddleTankGameEntity(b2Body &body, TextureAtlas &atlas, std::string const &tankSpriteName, std::string const &barrelSpriteName, float density = 1.0f, EntityState<PaddleTankGameEntity> *startState = nullptr);
	virtual ~PaddleTankGameEntity();

	//Update the position of the tank and the tanks state machine.
	virtual void Update(float delta);

	//Handle telegrams. When implementing this there are generally 3 places a telegram can
	// be handled - directly in the class, by the state machine, and by a base class. It's up
	// to you to decide who, and in what order, gets a chance to process a message.
	virtual bool HandleMessage(const Telegram& msg);

	//Set the ID of the map, so that it can be sent messages.
	void SetMapEntityID(int eid) {
		m_mapEntID = eid;
	}

	//Get the state machine - used for changing states.
	StateMachine<PaddleTankGameEntity>* GetFSM() const { 
		return m_stateMachine; 
	}

	//Move the tank in this direction.
	void ApplyLinearImpulse(b2Vec2 const &impulse);

	//Set the turret to point in this direction.
	void SetTurretAngle(float angleRads);
	
	//Check if the tank can fire a shot.
	bool IsShotReady() {
		return m_shotReady;
	}
	bool BulletsInMagazine(){
		if (m_magazine == 0)
			return false;
		else
			return true;
	}

	//Fire a bullet from the tip of the barrel.
	// Note that this method does not enforce the restriction
	// imposed by m_shotReady - it will spawn a bullet if called
	// regardless of the value.
	void Shoot();

protected:
	StateMachine<PaddleTankGameEntity> *m_stateMachine;
	//Render the sprite.
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::Sprite *m_barrelSprite;
	
	//Id of the map that contains this entity. When you want to fire a bullet, send
	/// a message to the map via the dispatcher.
	int m_mapEntID;
	//Value that controls whether or not the tank is in a state in which it can 
	// shoot. Used to throttle the shot rate.
	bool m_shotReady;
	int m_magazine;
};

