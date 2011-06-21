#ifndef OBJKILL_H
#define OBJKILL_H

//Object Creation State

#include "State.h"
#include "../Messager/Messager.h"

//temp holder for Object so i don't have recursive includes
class Object;

class ObjKill : public State<Object>
{
private:
	ObjKill(){}
public:
	static ObjKill* Instance();
	virtual void Enter(Object* obj);
	virtual void Execute(Object* obj);
	virtual void Exit(Object* obj);
	virtual bool OnMsg(Object* obj, const Telegram& msg);


};



#endif