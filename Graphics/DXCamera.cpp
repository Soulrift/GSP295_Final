//////////////////////////////////////////////////////////////////
//File: DXCamera.cpp
//Contents: Camera Class
//Programmer: Cameron Ferguson
//Date: August 2010
//Last Edit: 10/3/10
//////////////////////////////////////////////////////////////////

#include "DXCamera.h"

#define DXCAMERA_FREEFORM 0;

DXCamera::DXCamera() 
{
	D3DXMatrixIdentity(&mView);

	mPosition		= D3DXVECTOR3(0.0f, 0.0f, 25.0f);	//the position of the camera
	mRight			= D3DXVECTOR3(1.0f, 0.0f, 0.0f);	// the right axis from the camera's position
	mUpVec			= D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//the up vector, default to Y
	mForward		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//target that we want to look at

	//simple check to prevent bad math calculations
	if(mForward.x == mPosition.x && mForward.z == mPosition.z)
	{
		mPosition.x += 0.00001f; 
	}

	recalcVectors();									//calculate the vectors accordingly
	
	mSpeed			= 10.0f;				//camera's movement speed
	mSensitivity	= 1.0f;		//camera's mouse sensativity

	mCameraMode		= CameraModeFreeForm;				//camera mode

	mThirdPersonTarget = NULL;
	mThirdPersonTheta = 0.0f;
	mThirdPersonPhi = 0.0f;
	mThirdPersonRho = 1000.0f;

}

DXCamera::~DXCamera() 
{

}

DXCamera* DXCamera::Instance() 
{
	static DXCamera *ptr = new DXCamera();
	
	return ptr;
}

void DXCamera::adjust(float dt) 
{
	switch(mCameraMode) 
	{
		case CameraModeFreeForm:
			rotateFreeForm(dt);
			adjustFreeForm(dt);
			
		break;

		case CameraModeThirdPerson:
			adjustThirdPerson(dt);
			rotateThirdPerson(dt);
		break;
	}

	buildView();
	buildWorldFrustumPlanes();
}

//mouse rotation
void DXCamera::rotateFreeForm(float dt) 
{

	if(gDInput->mouseButtonDown(1)) //if right mouse button are down
	{ 
		
		//grab angles
		float pitch = gDInput->mouseDY() * dt; //get the change in the angle for moving the mouse up and down
		float yAngle = gDInput->mouseDX() * dt; //get the change in the angle for moving the mouse left and right
		
		//set sensativity
		pitch *= mSensitivity;
		yAngle *= mSensitivity;

		//rotate Up and Down
		D3DXMATRIX matRotAxis;
		D3DXMatrixRotationAxis(&matRotAxis, &mRight, pitch);
		D3DXVec3TransformCoord(&mForward, &mForward, &matRotAxis);
		D3DXVec3TransformCoord(&mUpVec, &mUpVec, &matRotAxis);

		//rotate left and right
		D3DXMATRIX matRotYAxis;
		D3DXMatrixRotationY(&matRotYAxis, yAngle);
		D3DXVec3TransformCoord(&mRight, &mRight, &matRotYAxis);
		D3DXVec3TransformCoord(&mUpVec, &mUpVec, &matRotYAxis);
		D3DXVec3TransformCoord(&mForward, &mForward, &matRotYAxis);
		
	}
}

//free form adjustment
void DXCamera::adjustFreeForm(float dt) 
{
	//check speed modifier
	float speedSet = mSpeed;
	if(gDInput->keyDown(DIK_LSHIFT)) 
	{
		speedSet *= 5;
	}

	D3DXVECTOR3 dir(0.0f, 0.0f, 0.0f);

	if(gDInput->keyDown(DIK_W))  //move forward
	{
		dir += mForward;
	}

	if(gDInput->keyDown(DIK_S)) //move backward
	{ 
		dir -= mForward;
	}

	if(gDInput->keyDown(DIK_D))  //strafe right
	{
		dir += mRight;
	}

	if(gDInput->keyDown(DIK_A)) //strafe left
	{ 
		dir -= mRight;
	}

	if(gDInput->keyDown(DIK_SPACE)) //move up in the y direction
	{
		dir.y += 1;
	}

	if(gDInput->keyDown(DIK_O))		//move down in y direction
	{
		dir.y -= 1;
	}

	D3DXVec3Normalize(&dir, &dir);
	mPosition += dir * speedSet * dt;

}

// adjustments for the third person camera
void DXCamera::adjustThirdPerson(float dt)
{


	//Mouse Wheel for zooming in and out from the target
	float deltZ = gDInput->mouseDZ();
	if(deltZ != 0.0f)
	{
		mThirdPersonRho -= gDInput->mouseDZ() * mSpeed * dt;

		if(mThirdPersonRho < 1.0f)
		{
			mThirdPersonRho = 1.0f;
		}
	}

	recalcThirdPersonCoords();

}

//rotation for the third person camera
void DXCamera::rotateThirdPerson(float dt)
{
	static float prevTheta = mThirdPersonTheta;
	if(gDInput->mouseButtonDown(1)) //if left mouse button down, and not right
	{ 
		//grab angles
		mThirdPersonTheta += gDInput->mouseDY() / 360; //get the change in the angle for moving the mouse up and down
		mThirdPersonPhi -= gDInput->mouseDX() / 360; //get the change in the angle for moving the mouse left and right
		
		//used to lock the "poles" of the sphere so you can't flip the camera over. You can look straight up and straight down without flipping the camera.
		if(cos(mThirdPersonTheta) < 0.0001f)
		{
			mThirdPersonTheta = prevTheta;
		}

		recalcThirdPersonCoords();
		prevTheta = mThirdPersonTheta;
	}
}

//build and return the view matrix for the game world
void DXCamera::buildView() 
{
	
	//Keep camera's axes orthogonal to each other and of unit length.
	D3DXVec3Normalize(&mForward, &mForward);
	//recalculate the up vector
	D3DXVec3Cross(&mUpVec, &mForward, &mRight);
	D3DXVec3Normalize(&mUpVec, &mUpVec);

	//D3DXVec3Cross(&mRight, &mUpVec, &mForward);  // seems pointless
	D3DXVec3Normalize(&mRight, &mRight);

	//Fill in the view matrix entries.
	float x = -D3DXVec3Dot(&mPosition, &mRight);
	float y = -D3DXVec3Dot(&mPosition, &mUpVec);
	float z = -D3DXVec3Dot(&mPosition, &mForward);

	mView(0,0) = mRight.x; 
	mView(1,0) = mRight.y; 
	mView(2,0) = mRight.z; 
	mView(3,0) = x;   

	mView(0,1) = mUpVec.x;
	mView(1,1) = mUpVec.y;
	mView(2,1) = mUpVec.z;
	mView(3,1) = y;  

	mView(0,2) = mForward.x; 
	mView(1,2) = mForward.y; 
	mView(2,2) = mForward.z; 
	mView(3,2) = z;   

	mView(0,3) = 0.0f;
	mView(1,3) = 0.0f;
	mView(2,3) = 0.0f;
	mView(3,3) = 1.0f;
}

void DXCamera::buildWorldFrustumPlanes()
{
	// Note: Extract the frustum planes in world space.

	D3DXMATRIX VP = mView * DXPI->getProj();

	D3DXVECTOR4 col0(VP(0,0), VP(1,0), VP(2,0), VP(3,0));
	D3DXVECTOR4 col1(VP(0,1), VP(1,1), VP(2,1), VP(3,1));
	D3DXVECTOR4 col2(VP(0,2), VP(1,2), VP(2,2), VP(3,2));
	D3DXVECTOR4 col3(VP(0,3), VP(1,3), VP(2,3), VP(3,3));

	// Planes face inward.
	m_xpFrustumPlanes[0] = (D3DXPLANE)(col2);        // near
	m_xpFrustumPlanes[1] = (D3DXPLANE)(col3 - col2); // far
	m_xpFrustumPlanes[2] = (D3DXPLANE)(col3 + col0); // left
	m_xpFrustumPlanes[3] = (D3DXPLANE)(col3 - col0); // right
	m_xpFrustumPlanes[4] = (D3DXPLANE)(col3 - col1); // top
	m_xpFrustumPlanes[5] = (D3DXPLANE)(col3 + col1); // bottom

	for(int i = 0; i < 6; i++)
	{
		D3DXPlaneNormalize(&m_xpFrustumPlanes[i], &m_xpFrustumPlanes[i]);
	}
}
/*
bool DXCamera::isVisible(AABB &box)
{
	D3DXVECTOR3 P;
	D3DXVECTOR3 Q;

	for(int i = 0; i < 6; ++i)
	{
		// For each coordinate axis x, y, z...
		for(int j = 0; j < 3; ++j)
		{
			// Make PQ point in the same direction as the plane normal on this axis.
			if( m_xpFrustumPlanes[i][j] >= 0.0f )
			{
				P[j] = box.minPt[j];
				Q[j] = box.maxPt[j];
			}
			else 
			{
				P[j] = box.maxPt[j];
				Q[j] = box.minPt[j];
			}
		}

		if( D3DXPlaneDotCoord(&m_xpFrustumPlanes[i], &Q) < 0.0f  ) // outside
			return false;
	}
	return true;
}
*/

void DXCamera::setCameraMode(CameraMode mode)
{
	mCameraMode = mode;
}

//set the camera's movement speed
void DXCamera::setSpeed(float speed) 
{
	mSpeed = speed;
}

//get the camera's movement speed
float DXCamera::getSpeed() 
{
	return mSpeed;
}

//set the camera's mouse sensitivity
void DXCamera::setSensitivity(float sens) 
{
	mSensitivity = sens;
}

//get the camera's mouse sensitivity
float DXCamera::getSensitivity() 
{
	return mSensitivity;
}

//set the camera's position manually
void DXCamera::setPosition(D3DXVECTOR3 pos) 
{
	mPosition = pos;
	recalcVectors();
}

void DXCamera::setThirdPersonTarget(D3DXVECTOR3* tar)
{
	if(tar == NULL)
	{
		return;
	}

	mThirdPersonTarget = tar;
	mThirdPersonPrevTargetPos = *tar;
	recalcThirdPersonCoords();
	recalcVectors();
}

void DXCamera::setThirdPersonCameraDistance(float dist)
{
	mThirdPersonRho = dist;
	recalcThirdPersonCoords();
	recalcVectors();
}

//get the camera's current position
D3DXVECTOR3 DXCamera::getPosition() 
{
	return mPosition;
}

//set the camera's target
void DXCamera::setTarget(D3DXVECTOR3 target) 
{
	mForward = target;
	recalcVectors();
}

//get the camera's current target
D3DXVECTOR3 DXCamera::getTarget() 
{
	return mForward;
}

//set the camera's UP vector (default to 0, 1, 0)
void DXCamera::setUpVec(D3DXVECTOR3 upVec) 
{
	mUpVec = upVec;
}

//get the camera's current up vector
D3DXVECTOR3 DXCamera::getUp() 
{
	return mUpVec;
}

D3DXVECTOR3 DXCamera::getForward()
{
	return mForward;
}

D3DXVECTOR3 DXCamera::getRight()
{
	return mRight;
}

D3DXVECTOR3 DXCamera::getThirdPersonTarget()
{
	return *mThirdPersonTarget;
}

float DXCamera::getThirdPersonCameraDistance()
{
	return mThirdPersonCameraDistance;
}

D3DXVECTOR3 DXCamera::getCameraDirection()
{
	//get camera diretion and return
	D3DXVECTOR3 dir = mForward - mPosition;
	D3DXVec3Normalize(&dir, &dir);
	return dir;
}

D3DXMATRIX DXCamera::getView() 
{
	return mView;
}

void DXCamera::recalcVectors()
{
	mRight	= D3DXVECTOR3(1.0f, 0.0f, 0.0f);	// the right axis from the camera's position
	mUpVec	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//the up vector, default to Y

	mForward = mForward - mPosition;
	D3DXVec3Normalize(&mForward, &mForward);

	D3DXVec3Cross(&mRight, &mUpVec, &mForward);
	D3DXVec3Cross(&mUpVec, &mForward, &mRight);
}

void DXCamera::recalcThirdPersonCoords()
{
	D3DXVECTOR3 pos;
	//figure out camera's new position based off of the mouse movement
	pos.x = mThirdPersonRho * cos(mThirdPersonTheta) * cos(mThirdPersonPhi);
	pos.y = mThirdPersonRho * sin(mThirdPersonTheta);
	pos.z = mThirdPersonRho * sin(mThirdPersonPhi) * cos(mThirdPersonTheta); // * cos(mThirdPersonPhi);

	if(mThirdPersonTarget != NULL)
	{
		mPosition = pos + *mThirdPersonTarget;
		mForward = *mThirdPersonTarget;
	}

	recalcVectors();
}