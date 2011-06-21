#include "MyGameWorld.h"
#include "Graphics/gspOut.h"
#include "ColorDefines.h"
#include "Global.h"
#include "ObjectManager.h"
#include "Algo\AStar.h"
#include "Algo\Graph.h"
#include "Object.h"
#include "InstanceObject.h"

#include <math.h>
#include <ctime>


// ***********************************************************************
// Credits for developing/enhancing this framework to DeVry students: 
// Nathan Harbour: the original 2D DirectX GameWorld code - 2007
// Heath Wickman: extended the framework into 3D - 2009
// Cameron Ferguson: added the "gspOut" functionality - 2009
//					 Updated Direct Input Capabilities, Improved Camera Functionality, and updated gspOut - Oct 2010
//					 Fixed Memory Leaks with squares and triangles that were not being deleted properly - Nov 2010
//					 Added capabilities for objects to change colors									- Nov 2010
//					 Updated gspOut to prevent breaking and scrolling issues when outputting text		- Feb 2011
// Jessica McClane: fixed an annoying error that occurred on exit - 2009
// Your name could be here!


MyGameWorld* gMyGameWorld = 0;
char message[100];
char message2[100];
char message3[100];
char message4[100];

//gspOut gDebugWindow(true, "My Debug Window");

//SquareEntity* mySquare;
//  Anything that needs to happen once at the beginning of the program goes here.
void MyGameWorld::Initialize(void)
{
	DXCI->setCameraMode(CameraModeFreeForm);				//set up a spectator, FPS camera
	DXCI->setPosition(D3DXVECTOR3(5.0f, 10.0f, -30.0f));		//set the camera's starting position
	DXCI->setTarget(D3DXVECTOR3(5.0f, 10.0f, 0.0f));			//set the current target for the camera
	DXCI->setSpeed(10.0f);									//set the camera's movement speed, thie higher the number, the faster it moves.
	DXCI->setSensitivity(1.2f);								//set the mouse sensativity. The higher the number, the more sensative it is.

	GLB->SetCount(0);
	srand(time(0));

	GLB->SetIds();

	GRP->Create();

	OGEN->makeObject();

	SetBackground(0, 60, 0);
	SetBackgroundBlue(100.0);
}

//Update method:  Anything that needs to happen every time through the animation loop goes here.
void MyGameWorld::Update(float time_elapsed)
{
	static double clock = 0.0f;
	GLB->SetTime(time_elapsed, clock);
	if (clock < 40)
	{
		ObjMan->Update();
	}else
	{
		exit(0);
	}
	clock += time_elapsed;
	 gDInput->poll();
	DXCI->adjust(time_elapsed);
	sprintf_s(message, "Clock:%f\n", clock);
	sprintf_s(message2, "Start Node:%f\n", GLB->as_start);
	sprintf_s(message3, "End Node:%f\n", GLB->as_end);
	MyGameWorld::ClearText();
	MyGameWorld::AddText(message);
	MyGameWorld::AddText(message2);
	MyGameWorld::AddText(message3);
}

//Destructor: Anything that needs to happen once at the end of the program goes here.
MyGameWorld::~MyGameWorld(void)
{
	DestroyTriangles();
	DestroySquares();
}

//*****************************************************
//				   KeyPress Event
//*****************************************************
void MyGameWorld::KeyPress(WPARAM keyCode)
{
	if ( keyCode == 'P' || keyCode == 'p' || keyCode == VK_LEFT )
		PauseWorld();
}