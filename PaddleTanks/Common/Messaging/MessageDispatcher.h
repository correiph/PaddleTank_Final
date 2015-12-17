#ifndef MESSAGE_DISPATCHER_H
#define MESSAGE_DISPATCHER_H
#pragma warning (disable:4786)
//------------------------------------------------------------------------
//
//  Name:   MessageDispatcher.h
//
//  Desc:   A message dispatcher. Manages messages of the type Telegram.
//          Instantiated as a singleton.
//			
//
//  Author: Mat Buckland (fup@ai-junkie.com)
//	Modified: Kevin Forest 2015
//		SFML-afied
//
//------------------------------------------------------------------------
#include "Telegram.h"

#include <set>
#include <string>

class BaseGameEntity;

//to make life easier...
#define Dispatcher MessageDispatcher::Instance()

//to make code easier to read
const float SEND_MSG_IMMEDIATELY = 0.0f;
const int    NO_ADDITIONAL_INFO   = 0;
//Used for sending blind messages.
const int    SENDER_ID_IRRELEVANT = -1;

class MessageDispatcher
{
private:  
  
  //a std::set is used as the container for the delayed messages
  //because of the benefit of automatic sorting and avoidance
  //of duplicates. Messages are sorted by their dispatch time.
  std::set<Telegram> PriorityQ;

  //this method is utilized by DispatchMsg or DispatchDelayedMessages.
  //This method calls the message handling member function of the receiving
  //entity, pReceiver, with the newly created telegram
  void Discharge(BaseGameEntity* pReceiver, const Telegram& msg);

  MessageDispatcher() : m_lastDispatchTime(0.0f){}

  //Do not implement to ensure singleton-ness.
  MessageDispatcher(const MessageDispatcher&);
  MessageDispatcher& operator=(const MessageDispatcher&);
  float m_lastDispatchTime;
public:

  static MessageDispatcher* Instance();

  //send a message to another agent. Receiving agent is referenced by ID.
  void DispatchMsg(float      delay,
                   int         sender,
                   int         receiver,
                   int         msg,
                   void*       ExtraInfo);

  //send out any delayed messages. This method must be called each tick through   
  //the main game loop.
  void DispatchDelayedMessages(float delta);
};



#endif