#ifndef OBJPATH_H
#define OBJPATH_H

//Object Creation State

#include "State.h"
#include "../Messager/Messager.h"

//temp holder for Object so i don't have recursive includes
class Object;

class ObjPath : public State<Object>
{
private:
	ObjPath(){}
	//test if path is completed
	bool finished;
public:
	static ObjPath* Instance();
	virtual void Enter(Object* obj);
	virtual void Execute(Object* obj);
	virtual void Exit(Object* obj);
	virtual bool OnMsg(Object* obj, const Telegram& msg);

};



#endif