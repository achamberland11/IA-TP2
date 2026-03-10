//------------------------------------------------------------------------
//
//  Name:   State.h
//
//  Desc:   abstract base class to define an interface for a state
//
//  Author: Mat Buckland (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

#pragma once
#include <string>

struct Telegram;

template <class entity_type>
class State
{
public:

  virtual ~State() {
    StateName = "DefaultState";
  }

  //this will execute when the state is entered
  virtual void Enter(entity_type*)=0;

  //this is the states normal update function
  virtual void Execute(entity_type*)=0;

  //this will execute when the state is exited. 
  virtual void Exit(entity_type*)=0;

  std::string GetName()const{return StateName;}

  //this executes if the agent receives a message from the 
  //message dispatcher
  // virtual bool OnMessage(entity_type*, const Telegram&)=0;

protected:
  std::string StateName;
};