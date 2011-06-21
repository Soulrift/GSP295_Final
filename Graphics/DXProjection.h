//Created by Cameron Ferguson
//Singleton class for the projection of the application
//Created June 2, 2010

#define DXPI DXProjection::Instance()

#ifndef DX_PROJECTION_H
#define DX_PROJECTION_H

#include "d3dx9.h"
#include "d3dApp.h"

class DXProjection {

public:
	static DXProjection* Instance();

	void set(float aspect, float fov, float nearPlane, float farPlane, bool build);

	float getAspectRatio();
	float getFOV();
	float getNearPlane();
	float getFarPlane();

	void buildProj();

	D3DXMATRIX getProj();

	~DXProjection();
private:
	DXProjection();

	float mAspectRatio;
	float mFOV;
	float mNearPlane;
	float mFarPlane;

	D3DXMATRIX mProjMatrix;
};

#endif