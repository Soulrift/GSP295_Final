#include "DXProjection.h"

DXProjection::DXProjection() {

}

DXProjection::~DXProjection() {

}

DXProjection* DXProjection::Instance() {
	static DXProjection *ptr = new DXProjection();
	return ptr;
}

void DXProjection::set(float aspect, float fov, float nearPlane, float farPlane, bool build) {
	mAspectRatio = aspect;
	mFOV = fov;
	mNearPlane = nearPlane;
	mFarPlane = farPlane;

	if(build) { //build project after setting if true
		buildProj();
	}
}

void DXProjection::buildProj() {
	D3DXMatrixPerspectiveFovLH(&mProjMatrix, mFOV, mAspectRatio, mNearPlane, mFarPlane);
}

D3DXMATRIX DXProjection::getProj() {
	return mProjMatrix;
}