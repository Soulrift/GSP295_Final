#ifndef INSTOBJ_H
#define INSTOBJ_H

#include "Object.h"
#include "ObjectManager.h"

#define OGEN InstanceObject::Instance()

class InstanceObject
{
public:

	static InstanceObject* Instance()
	{
		static InstanceObject temp;

		return &temp;
	}

	InstanceObject(){}
	~InstanceObject(){}

	Object *makeObject()
	{
		Object *obj = new Object();
		ObjMan->Add(obj);
		obj->Start();
		return obj;
	}
};

#endif