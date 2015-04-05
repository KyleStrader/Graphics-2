//=============================================================================
//                          SkeletonClass.h
//
// Adopted from CubeDemo.cpp by Frank Luna (C) 2005 All Rights Reserved.
//
// Edited and changed by Adi Bar-Lev, 2013
// EGP-300-101 - Computer Graphics II, Spring 2013.
//
// This project creates a skeleton workspace and base classes for the student to be 
// able to start working with sample classes and develop evolve it according to the 
// given exercises
//
// Controls: Use mouse to orbit and zoom; use the 'W' and 'S' keys to 
//           alter the height of the camera.
//=============================================================================
#ifndef _SKELETON_CLASS_H
#define _SKELETON_CLASS_H
//=============================================================================
#pragma once
//=============================================================================
#include "d3dApp.h"

#include <vector>
//=============================================================================
class BaseObject3D;
class SkyboxObject;
//=============================================================================
class SkeletonClass : public D3DApp
{
public:
	SkeletonClass(HINSTANCE hInstance, std::string winCaption, D3DDEVTYPE devType, DWORD requestedVP);
	~SkeletonClass();

	bool checkDeviceCaps();
	void onLostDevice();
	void onResetDevice();
	void updateScene(float dt);
	void handleInput(float dt);
	void drawScene();
	void buildFx();
	void switchFx();

	// Helper methods
	void buildViewMtx();
	void buildProjMtx();

private:
	float mCameraRotationY;
	float mCameraRotationX;
	float mCameraRadius;
	float mCameraHeight;

	bool dKeyDown = false;
	bool sKeyDown = false;
	bool aKeyDown = false;
	bool oKeyDown = false;
	bool wKeyDown = false;
	bool pKeyDown = false;
	bool rKeyDown = false;
	bool solidRender = true;
	bool phongRender = true;
	int currentObj = 0;
	int numObjs;

	D3DXMATRIX mWorld;
	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	ID3DXEffect* mFX;
	ID3DXEffect* mPhongFX;
	ID3DXEffect* mGouraudFX;
	ID3DXEffect* mSkyboxFX;

	D3DXHANDLE   mhTech;
	D3DXHANDLE   mhPhongTech;
	D3DXHANDLE   mhGouraudTech;
	D3DXHANDLE   mhSkyboxTech;

    std::vector<BaseObject3D*>      m_Objects;
};
//=============================================================================
#endif // _SKELETON_CLASS_H_