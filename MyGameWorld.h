#ifndef MY_GAME_WORLD_H
#define MY_GAME_WORLD_H


#include "Graphics/GameWorld.h"
#include "Graphics/DXCamera.h"
#include "Graphics/DXProjection.h"

class MyGameWorld :
	public GameWorld
{
public:

	//The following methods should not be deleted, nor should their
	//  input parameters be changed.
	MyGameWorld(void) : GameWorld(){}
	~MyGameWorld(void);

	void KeyPress(WPARAM keyCode);

	void Initialize(void);
	void Update(float time_elapsed);
	
	//Put all your public methods and variables here



private:
	//Put all your private methods and variables here



};

extern MyGameWorld* gMyGameWorld;

#endif