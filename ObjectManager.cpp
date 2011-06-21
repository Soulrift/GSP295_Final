#include "ObjectManager.h"
#include "ObjectBase.h"


ObjectManager* ObjectManager::Instance()
{
	static ObjectManager temp;
	return &temp;
}

ObjectBase* ObjectManager::GetObj(int id)const
{
	map<int, ObjectBase*>::const_iterator i = m_Objects.find(id);
	//verify that i'm not returning something out of bounds... no bueno
	assert(i != m_Objects.end());
	//return the object component of the map
	return i->second;
}

void ObjectManager::Del(ObjectBase* obj)
{
	//erase the object... pulls id using GetID function inside objbase
	m_Objects.erase(m_Objects.find(obj->GetID()));
}

void ObjectManager::Add(ObjectBase* obj)
{
	//add an object to the map
	m_Objects.insert(make_pair(obj->GetID(), obj));
}

void ObjectManager::Update()
{
	MSGR->DelayMsg();
	map<int, ObjectBase*>::const_iterator i;
	for(i = m_Objects.begin(); i != m_Objects.end(); i++)
		(*i).second->Update();
}