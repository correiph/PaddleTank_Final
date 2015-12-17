#ifndef STATE_H
#define STATE_H
//------------------------------------------------------------------------
//
//  Name:   EntityState.h
//
//  Desc:   abstract base class to define an interface for a state
//
//  Author: Mat Buckland (fup@ai-junkie.com)
//	Modified: Kevin Forest 2015
//
//------------------------------------------------------------------------
struct Telegram;

template <class entity_type>
class EntityState
{

public:
	
	virtual ~EntityState(){}

  //this will execute when the state is entered
  virtual void Enter(entity_type*)=0;

  //this is the states normal update function
  virtual void Execute(entity_type*, float delta)=0;

  //this will execute when the state is exited. 
  virtual void Exit(entity_type*)=0;

  //this executes if the agent receives a message from the 
  //message dispatcher
  virtual bool OnMessage(entity_type*, const Telegram&)=0;
};

#endif