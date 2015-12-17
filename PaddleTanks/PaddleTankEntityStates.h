#pragma once

#include "FSM/EntityState.h"

class PaddleTankGameEntity;

//You can use this area to create multiple states for the state machines. You are going to need
// an AI state at the very least. You can use a single AI state or you can have multiple ones and
// switch between them.
//If you decide that the bullets (or any other entity) need their own state machine I would create a separate
// BulletGameEntity class that subclasses Box2DGameEntity and give it a state machine, rather
// than trying to add a statemachine directly to Box2DGameEntity. I would then create entity
// states for the BulletGameEntity class.
//Remember that your states should be singletons, which means they cannot hold stateful information
// internally - this means you will have to provide the necessary getters and setters on your
// entities to allow the states the ability to manipulate the internal state of the
// entity objects.

//A state for allowing human control of the tank. Remember that all states are supposed
// to be singletons.
// This state ensures that the tank moves when keys are pressed, that the 
// barrel of the gun always follows the mouse, and that the tank fires when you
// press the space bar. It internally enforces the reload time on the tank gun.
// It also handles registering and deregistering the keys for controlling it.
class PaddleTankHumanControlledEntityState : public EntityState<PaddleTankGameEntity>
{
private:
	PaddleTankHumanControlledEntityState() { }
	PaddleTankHumanControlledEntityState(PaddleTankHumanControlledEntityState const &);
	PaddleTankHumanControlledEntityState operator=(PaddleTankHumanControlledEntityState const &);

public:
	static PaddleTankHumanControlledEntityState *Instance() {
		static PaddleTankHumanControlledEntityState instance;
		return &instance;
	}

	virtual ~PaddleTankHumanControlledEntityState(){}

	//this will execute when the state is entered
	// Registers Up, Down, and Space.
	virtual void Enter(PaddleTankGameEntity*);

	//this is the states normal update function
	virtual void Execute(PaddleTankGameEntity*);

	//this will execute when the state is exited. 
	// Deregisters Up, Down, and Space.
	virtual void Exit(PaddleTankGameEntity*);

	//this executes if the agent receives a message from the 
	//message dispatcher
	virtual bool OnMessage(PaddleTankGameEntity*, const Telegram&);
};