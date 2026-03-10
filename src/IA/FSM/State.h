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

class GEntity;

template <class entity_type>
class State
{
public:

  virtual ~State() {}

  //this will execute when the state is entered
  virtual void Enter(GEntity *)=0;

  //this is the states normal update function
  virtual void Execute(GEntity*)=0;

  //this will execute when the state is exited. 
  virtual void Exit(GEntity*)=0;

  std::string GetName()const{return StateName;}

protected:
  std::string StateName = "DefaultState";
};
