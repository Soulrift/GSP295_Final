#ifndef STATE_H
#define STATE_H

//pure virtual state wrapper

#include "../Messager/Telegram.h"

//since we're using multiple states, want to pass teh correct one
template <class type>

class State
{
public:
	virtual void Enter(type*) = 0;
	virtual void Execute(type*) = 0;
	virtual void Exit(type*) = 0;
	virtual bool OnMsg(type*, const Telegram&) = 0;

	virtual ~State(){}

};


#endif