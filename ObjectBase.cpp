#include "ObjectBase.h"

//set initial object to 0
int ObjectBase::m_iNextID = 0;

void ObjectBase::NewID()
{
	//make the id of the object equal to the "next id", then increment
	m_iObjID = m_iNextID;
	m_iNextID = m_iObjID + 1;
}

ObjectBase::ObjectBase()
{
	//force the object to get a new unique id
	NewID();
}

int ObjectBase::GetID()
{
	return m_iObjID;
}