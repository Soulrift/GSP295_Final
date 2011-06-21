//////////////////////////////////////////////////////////////////
//File: DXCamera.h
//Contents: Camera Class
//Programmer: Cameron Ferguson
//Date: August 2010
//Last Edit: 8/25/10
//////////////////////////////////////////////////////////////////

#define DXCI DXCamera::Instance()

#ifndef DX_CAMERA_H
#define DX_CAMERA_H

#include "DirectInput.h" //include direct input for camera movement use
#include "DXProjection.h"
#include <d3dx9.h>
#include "d3dUtil.h"

enum CameraMode
{
	CameraModeFreeForm,
	CameraModeThirdPerson,
};

class DXCamera
{
public:
	static DXCamera* Instance(); //funcion to work the camera as a singleton

	void setCameraMode(CameraMode mode);

	void adjust(float dt); //function called to adjust the camera based off of input
	void buildView(); //build the view matrix
	void buildWorldFrustumPlanes();

	//bool isVisible(AABB& box);

	//camera mode adjustments
	void adjustFreeForm(float dt); //adjust for the freeform camera
	void rotateFreeForm(float dt); //rotate for the freeform camera
	
	void adjustThirdPerson(float dt);
	void rotateThirdPerson(float dt);

	//mutator methods
	void setPosition(D3DXVECTOR3 pos); //pointer to actively change the up vector
	void setTarget(D3DXVECTOR3 target);
	void setUpVec(D3DXVECTOR3 upVec);
	void setSpeed(float speed);
	void setSensitivity(float sens);

	void setThirdPersonTarget(D3DXVECTOR3* pos);
	void setThirdPersonCameraDistance(float dist);

	//accessor methods
	D3DXVECTOR3 getPosition();
	D3DXVECTOR3 getTarget();
	D3DXVECTOR3 getUp();
	D3DXVECTOR3 getForward();
	D3DXVECTOR3 getRight();
	D3DXVECTOR3 getCameraDirection();
	D3DXMATRIX getView();
	float getSpeed();
	float getSensitivity();

	D3DXVECTOR3 getThirdPersonTarget();
	float getThirdPersonCameraDistance();

	

	~DXCamera(); //destructor
private:
	DXCamera();				//private constructor to prevent multiple items being made

	void recalcVectors();	//recalculate the vectors when something is manaully changed like the target or the position
	void recalcThirdPersonCoords();

	//camera variables
	D3DXMATRIX mView;

	D3DXVECTOR3 mForward;	//target that we want to look at
	D3DXVECTOR3 mPosition;	//the position of the camera
	D3DXVECTOR3 mUpVec;		//the up vector
	D3DXVECTOR3 mRight;		//right axis to the camera

	D3DXPLANE m_xpFrustumPlanes[6];

	float mSpeed;			//movement speed from keyboard entries
	float mSensitivity;		//mouse sensativity

	CameraMode mCameraMode;

	D3DXVECTOR3* mThirdPersonTarget; //for third person view only
	D3DXVECTOR3 mThirdPersonPrevTargetPos;
	float mThirdPersonCameraDistance;

	float mThirdPersonTheta;  //Angle of rotation around the z-axis, where 0 degrees means parallel to the x-axis
	float mThirdPersonPhi;	  //The angle of declination from the z-axis, - degrees means you're directly over head, 90 degrees you're level with the object
	float mThirdPersonRho;    //The dirstance from the obuject the camera is targeting (raidus)
};

#endif