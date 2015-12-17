 #ifndef BASE_GAME_ENTITY_H
#define BASE_GAME_ENTITY_H
#pragma warning (disable:4786)
//------------------------------------------------------------------------
//
//  Name: BaseGameEntity.h
//
//  Desc: Base class to define a common interface for all game
//        entities
//		  Removed position related information - BaseGameEntity is
//		  intended solely to be a container for type information and
//		  a unique ID.
//		  For a moveable and renderable game entity please see the 
//		  derived class Box2DGameEntity.
//		  A base class can also be "tagged" for whatever purpose
//		  the programmer chooses - just be aware that since other
//		  processes and blocks of code may make use of the tag
//		  property your code should not be reliant upon it maintaining
//		  state once the code moves to another flow of execution.
//
//  Author: Mat Buckland (fup@ai-junkie.com)
//	Modified: Kevin Forest 2015
//		SFML-a-fied
//		Box2d-a-fied
//		Stripped of position information.
//
//------------------------------------------------------------------------

//#include "misc/utils.h"

#include <SFML/System/Vector2.hpp>

#include <vector>
#include <string>
#include <iosfwd>

struct Telegram;

class BaseGameEntity
{
public:
  
  enum {default_entity_type = -1, BOX2D_ENTITY = 1, PADDLE_TANK_ENTITY = 2, POWERUP_ENTITY = 3};

private:
  
  //each entity has a unique ID
  int         m_ID;

  //every entity has a type associated with it (health, troll, ammo etc)
  int         m_iType;

  //this is a generic flag. 
  bool        m_bTag;

  //this is the next valid ID. Each time a BaseGameEntity is instantiated
  //this value is updated
  static int  m_iNextValidID;

  //this must be called within each constructor to make sure the ID is set
  //correctly. It verifies that the value passed to the method is greater
  //or equal to the next valid ID, before setting the ID and incrementing
  //the next valid ID
  void SetID(int val);


protected:
  
  //its location in the environment
  //sf::Vector2f m_vPosition;

  //sf::Vector2f m_vScale;

  //the magnitude of this object's bounding radius
  //float    m_dBoundingRadius;

  
  BaseGameEntity(int ID);

public:

  virtual ~BaseGameEntity(){}

  virtual void Update(float delta){}; 

  virtual bool HandleMessage(const Telegram& msg){return false;}
  
  //use this to grab the next valid ID
  static int   GetNextValidID(){return m_iNextValidID;}
  
  //this can be used to reset the next ID
  static void  ResetNextValidID(){m_iNextValidID = 0;}
  /*
  sf::Vector2f     Pos()const{ return m_vPosition; }
  void         SetPos(sf::Vector2f new_pos){ m_vPosition = new_pos; }
  float       BRadius()const{return m_dBoundingRadius;}
  void         SetBRadius(float r){ m_dBoundingRadius = r; }
  */
  int          ID()const{return m_ID;}

  //Tagging can be used for a wide variety of purposes, but make sure
  // everyone on the team is aware and in agreement.
  bool         IsTagged()const{return m_bTag;}
  void         Tag(){m_bTag = true;}
  void         UnTag(){m_bTag = false;}
  /*
  sf::Vector2f     Scale()const{ return m_vScale; }
  void         SetScale(sf::Vector2f val){ m_dBoundingRadius *= MaxOf(val.x, val.y) / MaxOf(m_vScale.x, m_vScale.y); m_vScale = val; }
  void         SetScale(float val){ m_dBoundingRadius *= (val / MaxOf(m_vScale.x, m_vScale.y)); m_vScale = sf::Vector2f(val, val); }
  */
  //Get the entity type. Used for casting without having to 
  // rely on dynamic_cast.
  int          EntityType()const{return m_iType;}
  void         SetEntityType(int new_type){m_iType = new_type;}

};

#endif




