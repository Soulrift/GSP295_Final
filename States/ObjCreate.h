#ifndef OBJCREATE_H
#define OBJCREATE_H

//Object Creation State

#include "State.h"
#include "../Messager/Messager.h"

//temp holder for Object so i don't have recursive includes
class Object;

class ObjCreate : public State<Object>
{
private:
	ObjCreate(){}
public:
	static ObjCreate* Instance();
	virtual void Enter(Object* obj);
	virtual void Execute(Object* obj);
	virtual void Exit(Object* obj);
	virtual bool OnMsg(Object* obj, const Telegram& msg);


};



#endif