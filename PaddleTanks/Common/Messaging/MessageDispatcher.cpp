#include "MessageDispatcher.h"

#include "Entities/BaseGameEntity.h"
#include "Entities/EntityManager.h"

#include <iostream>

//uncomment below to send message info to the console
#define SHOW_MESSAGING_INFO

//--------------------------- Instance ----------------------------------------
//
//   this class is a singleton
//-----------------------------------------------------------------------------
MessageDispatcher* MessageDispatcher::Instance()
{
  static MessageDispatcher instance; 
  
  return &instance;
}

//----------------------------- Dispatch ---------------------------------
//  
//  see description in header
//------------------------------------------------------------------------
void MessageDispatcher::Discharge(BaseGameEntity* pReceiver, const Telegram& telegram)
{
  if (!pReceiver->HandleMessage(telegram))
  {
    //telegram could not be handled
    #ifdef SHOW_MESSAGING_INFO
    std::cout << "Message not handled" << "" << std::endl;
    #endif
  }
}

//---------------------------- DispatchMsg ---------------------------
//
//  given a message, a receiver, a sender and any time delay, this function
//  routes the message to the correct agent (if no delay) or stores
//  in the message queue to be dispatched at the correct time
//------------------------------------------------------------------------
void MessageDispatcher::DispatchMsg(float       delay,
                                    int          sender,
                                    int          receiver,
                                    int          msg,
                                    void*        AdditionalInfo = NULL)
{

  //get a pointer to the receiver
  BaseGameEntity* pReceiver = EntityMgr->GetEntityFromID(receiver);

  //make sure the receiver is valid
  if (pReceiver == NULL)
  {
    #ifdef SHOW_MESSAGING_INFO
	  std::cout << "\nWarning! No Receiver with ID of " << receiver << " found" << ""<< std::endl;
    #endif

    return;
  }
  
  //create the telegram
  Telegram telegram(0, sender, receiver, msg, AdditionalInfo);
  
  //if there is no delay, route telegram immediately                       
  if (delay <= 0.0)                                                        
  {
    #ifdef SHOW_MESSAGING_INFO
	  std::cout << "\nTelegram dispatched at time: " << m_lastDispatchTime
         << " by " << sender << " for " << receiver 
		 << ". Msg is " << msg << ""<< std::endl;
    #endif

    //send the telegram to the recipient
    Discharge(pReceiver, telegram);
  }

  //else calculate the time when the telegram should be dispatched
  else
  {
    //double CurrentTime = TickCounter->GetCurrentFrame(); 
    //telegram.DispatchTime = CurrentTime + delay;
	  telegram.DispatchTime = delay + m_lastDispatchTime;
    //and put it in the queue
    PriorityQ.insert(telegram);   

    #ifdef SHOW_MESSAGING_INFO
	std::cout << "\nDelayed telegram from " << sender << " recorded at time " 
		<< m_lastDispatchTime << " for " << receiver
			<< ". Msg is " << msg << ""<< std::endl;
    #endif
  }
}

//---------------------- DispatchDelayedMessages -------------------------
//
//  This function dispatches any telegrams with a timestamp that has
//  expired. Any dispatched telegrams are removed from the queue
//------------------------------------------------------------------------
void MessageDispatcher::DispatchDelayedMessages(float delta)
{ 
  //first get current time
  //double CurrentTime = TickCounter->GetCurrentFrame(); 
	m_lastDispatchTime += delta;
  //now peek at the queue to see if any telegrams need dispatching.
  //remove all telegrams from the front of the queue that have gone
  //past their sell by date
  while( !PriorityQ.empty() &&
	  (PriorityQ.begin()->DispatchTime < m_lastDispatchTime) &&
         (PriorityQ.begin()->DispatchTime > 0) )
  {
    //read the telegram from the front of the queue
    const Telegram& telegram = *PriorityQ.begin();

    //find the recipient
    BaseGameEntity* pReceiver = EntityMgr->GetEntityFromID(telegram.Receiver);

    #ifdef SHOW_MESSAGING_INFO
	std::cout << "\nQueued telegram ready for dispatch: Sent to " 
		<< pReceiver->ID() << ". Msg is "<< telegram.Msg << ""<< std::endl;
    #endif

    //send the telegram to the recipient
    Discharge(pReceiver, telegram);

	//remove it from the queue
    PriorityQ.erase(PriorityQ.begin());
  }
}



