#ifndef OBJMAN_H
#define OBJMAN_H

//Object Manager class - singleton class, holds an instance of an object
//in a hash table for easy access (map)

#include <assert.h>
#include <map>
#include "ObjectBase.h"
#include "Messager\Messager.h"

using namespace std;

#define ObjMan ObjectManager::Instance()

class ObjectManager
{
private:
	ObjectManager(){}
	//map to store object in
	map<int, ObjectBase*> m_Objects;

public:
	static ObjectManager* Instance();
	//add/remove objects from the map
	void Add(ObjectBase* obj);
	void Del(ObjectBase* obj);
	//getter functions
	ObjectBase* GetObj(int id)const;

	//update function.. will call all messaging updates for each object
	void Update();

};



#endif