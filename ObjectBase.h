#ifndef OBJBASE_H
#define OBJBASE_H

#include "Messager\Telegram.h"

class ObjectBase
{
private:
	//unique id for object
	int m_iObjID;
	//create a unique id for the player... called in ctor
	void NewID();

public:
	//keep track of what id's have been used:
	static int m_iNextID;
	//ctor
	ObjectBase();

	//getters
	int GetID();
	
	//virtual functions for children
	virtual void Update() = 0;
	virtual bool Handler(const Telegram& msg) = 0;
};

#endif