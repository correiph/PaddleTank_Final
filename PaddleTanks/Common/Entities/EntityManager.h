#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#pragma warning (disable:4786)
//------------------------------------------------------------------------
//
//  Name:   EntityManager.h
//
//  Desc:   Singleton class to handle the  management of Entities.
//			Entities are registered by their ID. Memory management of
//			entities is not the managers responsibility.
//
//  Author: Mat Buckland (fup@ai-junkie.com)
//	Modified: Kevin Forest 2015
//
//------------------------------------------------------------------------
#include <map>
#include <cassert>

class BaseGameEntity;

//provide easy access
#define EntityMgr EntityManager::Instance()


/// <summary>
/// A Manager for entities allowing access via entity IDs. To work properly
/// the ID for each entity should be unique.
/// </summary>
class EntityManager
{
private:

  typedef std::map<int, BaseGameEntity*> EntityMap;

private:
	EntityManager(){}

	//copy ctor and assignment should be private and unimplemented to 
	// ensure singleton-ness.
	EntityManager(const EntityManager&);
	EntityManager& operator=(const EntityManager&);

  //to facilitate quick lookup the entities are stored in a std::map, in which
  //pointers to entities are cross referenced by their identifying number
  EntityMap m_EntityMap;

public:

  static EntityManager* Instance();

  //this method stores a pointer to the entity in the std::vector
  //m_Entities at the index position indicated by the entity's ID
  //(makes for faster access)
  void            RegisterEntity(BaseGameEntity* NewEntity);

  //returns a pointer to the entity with the ID given as a parameter
  BaseGameEntity* GetEntityFromID(int id)const;

  //this method removes the entity from the list
  void            RemoveEntity(BaseGameEntity* pEntity);

  //clears all entities from the entity map
  void            Reset(){m_EntityMap.clear();}
};







#endif