//=============================================================================
//                          SkeletonClass.cpp
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
#include "DirectInput.h"
#include <crtdbg.h>
#include "GfxStats.h"
#include <list>

#include "SkeletonClass.h"
#include "3DClasses\BaseObject3D.h"
#include "3DClasses\Vertex.h"
#include "Cone3D.h"
#include "Cylinder3D.h"
#include "Box3D.h"
#include "Sphere3D.h"
#include "Teapot3D.h"
#include "Torus3D.h"
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	SkeletonClass app(hInstance, "Exercise Skeleton Project", D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);
	gd3dApp = &app;

	DirectInput di(DISCL_NONEXCLUSIVE|DISCL_FOREGROUND, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
	gDInput = &di;

	return gd3dApp->run();
}

void SkeletonClass::buildFx()
{
	// Phong
	// Create the FX from a .fx file.
	ID3DXBuffer* errors = 0;
	HR(D3DXCreateEffectFromFile(gd3dDevice, "Phong.fx",
	0, 0, D3DXSHADER_DEBUG, 0, &mPhongFX, &errors));
	if (errors)
	MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);

	// Obtain handles.
	mhPhongTech = mPhongFX->GetTechniqueByName("PhongTech");

	// Gouraud
	// Create the FX from a .fx file.
	errors = 0;
	HR(D3DXCreateEffectFromFile(gd3dDevice, "Gouraud.fx",
		0, 0, D3DXSHADER_DEBUG, 0, &mGouraudFX, &errors));
	if (errors)
		MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);

	// Obtain handles.
	mhGouraudTech = mGouraudFX->GetTechniqueByName("GouraudTech");

	switchFx();
}

void SkeletonClass::switchFx()
{
	if (phongRender)
	{
		mFX = mPhongFX;
		mhTech = mhPhongTech;
	}
	else
	{
		mFX = mGouraudFX;
		mhTech = mhGouraudTech;
	}
}

SkeletonClass::SkeletonClass(HINSTANCE hInstance, std::string winCaption, D3DDEVTYPE devType, DWORD requestedVP)
: D3DApp(hInstance, winCaption, devType, requestedVP)
{
	if(!checkDeviceCaps())
	{
		MessageBox(0, "checkDeviceCaps() Failed", 0, 0);
		PostQuitMessage(0);
	}

	mCameraRadius    = 15.0f;
	mCameraRotationY = 1.2 * D3DX_PI;
	mCameraRotationX = 0;
	mCameraHeight    = 5.0f;

	InitAllVertexDeclarations();

    // repleace or add to the following object creation
	m_Objects.push_back(new Box3D(4, 4, 4));
	m_Objects.push_back(new Sphere3D(2, 24, 24));
	m_Objects.push_back(new Cylinder3D(3, 3, 4, 16, 4));
	m_Objects.push_back(new Teapot3D());
	m_Objects.push_back(new Torus3D(1.0f, 3.0f, 100, 100));
	m_Objects.push_back(new Cone3D(3, 4, 16, 4));
	numObjs = m_Objects.size();
	
	buildFx();
	
	m_Objects[0]->ConnectEffect(mFX);
	m_Objects[1]->ConnectEffect(mFX);
	m_Objects[2]->ConnectEffect(mFX);
	m_Objects[3]->ConnectEffect(mFX);
	m_Objects[4]->ConnectEffect(mFX);
	m_Objects[5]->ConnectEffect(mFX);

	for (unsigned int obj = 0; obj<m_Objects.size(); obj++)
	{
		m_Objects[obj]->Create(gd3dDevice);
		m_Objects[obj]->ConnectEffect(mFX);
		m_Objects[obj]->ConnectToTexture(gd3dDevice, "./Textures/crate.jpg");
	}
	m_Objects[2]->RotateAroundAxis(D3DXVECTOR3(1.0f, 0.0f, 0.0f), 90);
	m_Objects[5]->RotateAroundAxis(D3DXVECTOR3(1.0f, 0.0f, 0.0f), -90);

	D3DXMatrixIdentity(&mWorld);

	onResetDevice();
}

SkeletonClass::~SkeletonClass()
{
    GfxStats::DeleteInstance();

    for ( unsigned int obj=0 ; obj<m_Objects.size() ; obj++ )
        delete m_Objects[obj];
    m_Objects.clear();

	DestroyAllVertexDeclarations();
}

bool SkeletonClass::checkDeviceCaps()
{
	return true;
}

void SkeletonClass::onLostDevice()
{
	GfxStats::GetInstance()->onLostDevice();
	HR(mFX->OnLostDevice());
}

void SkeletonClass::onResetDevice()
{
	GfxStats::GetInstance()->onResetDevice();
	HR(mFX->OnResetDevice());

	// The aspect ratio depends on the backbuffer dimensions, which can 
	// possibly change after a reset.  So rebuild the projection matrix.
	buildProjMtx();
}

void SkeletonClass::handleInput(float dt)
{
	// Get snapshot of input devices.
	gDInput->poll();

	if (gDInput->keyDown(DIK_D))
	{
		if (!dKeyDown)
		{
			for (unsigned int obj = 0; obj<m_Objects.size(); obj++)
			{
				m_Objects[obj]->ToggleDiffuse();
			}
			dKeyDown = true;
		}
	}
	else dKeyDown = false;

	if (gDInput->keyDown(DIK_S))
	{
		if (!sKeyDown)
		{
			for (unsigned int obj = 0; obj<m_Objects.size(); obj++)
			{
				m_Objects[obj]->ToggleSpecular();
			}
			sKeyDown = true;
		}
	}
	else sKeyDown = false;

	if (gDInput->keyDown(DIK_A))
	{
		if (!aKeyDown)
		{
			for (unsigned int obj = 0; obj<m_Objects.size(); obj++)
			{
				m_Objects[obj]->ToggleAmbient();
			}
			aKeyDown = true;
		}
	}
	else aKeyDown = false;

	if (gDInput->keyDown(DIK_O))
	{
		if (!oKeyDown)
		{
			currentObj++;
			if (currentObj >= numObjs) currentObj = 0;
			oKeyDown = true;
		}
	}
	else oKeyDown = false;

	if (gDInput->keyDown(DIK_W))
	{
		if (!wKeyDown)
		{
			solidRender = !solidRender;
			wKeyDown = true;
		}
	}
	else wKeyDown = false;

	if (gDInput->keyDown(DIK_P))
	{
		if (!pKeyDown)
		{
			phongRender = !phongRender;
			switchFx();

			for (unsigned int obj = 0; obj<m_Objects.size(); obj++)
			{
				m_Objects[obj]->ConnectEffect(mFX);
			}

			pKeyDown = true;
		}
	}
	else pKeyDown = false;


	// Divide by 50 to make mouse less sensitive. 
	mCameraRotationY += gDInput->mouseDX() / 100.0f;
	mCameraRotationX += gDInput->mouseDY() / 100.0f;
	mCameraRadius -= gDInput->mouseDZ() / 50.0f;

	// If we rotate over 360 degrees, just roll back to 0
	if (fabsf(mCameraRotationY) >= 2.0f * D3DX_PI)
		mCameraRotationY = 0.0f;

	// Don't let radius get too small.
	if (mCameraRadius < 5.0f)
		mCameraRadius = 5.0f;
}

void SkeletonClass::updateScene(float dt)
{
	// Reset the statistics for the scene - each object will add to it.
	GfxStats::GetInstance()->setVertexCount(0);
	GfxStats::GetInstance()->setTriCount(0);
	GfxStats::GetInstance()->update(dt);

	handleInput(dt);

	// The camera position/orientation relative to world space can 
	// change every frame based on input, so we need to rebuild the
	// view matrix every frame with the latest changes.
	buildViewMtx();
}


void SkeletonClass::drawScene()
{
	// Clear the backbuffer and depth buffer.
	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0));

	HR(gd3dDevice->BeginScene());

	if (solidRender){ HR(gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID)); }
	else { HR(gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME)); }

	HR(mFX->SetTechnique(mhTech));

	// Begin passes.
	UINT numPasses = 0;
	HR(mFX->Begin(&numPasses, 0));
	for (UINT i = 0; i < numPasses; ++i)
	{
		HR(mFX->BeginPass(i));
		m_Objects[currentObj]->Render(gd3dDevice, mView, mProj);
		HR(mFX->EndPass());
	}
	HR(mFX->End());


    // display the render statistics
    GfxStats::GetInstance()->display();

	HR(gd3dDevice->EndScene());

	// Present the backbuffer.
	HR(gd3dDevice->Present(0, 0, 0, 0));
}

void SkeletonClass::buildViewMtx()
{
	float x = mCameraRadius * cosf(mCameraRotationY);
	float z = mCameraRadius * sinf(mCameraRotationY);
	float y = mCameraRadius *  mCameraRotationX;
	D3DXVECTOR3 pos(x, y, z);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&mView, &pos, &target, &up);

	for (unsigned int obj = 0; obj<m_Objects.size(); obj++)
	{
		m_Objects[obj]->SetViewerPos(pos);
	}
}

void SkeletonClass::buildProjMtx()
{
	float w = (float)md3dPP.BackBufferWidth;
	float h = (float)md3dPP.BackBufferHeight;
	D3DXMatrixPerspectiveFovLH(&mProj, D3DX_PI * 0.25f, w/h, 1.0f, 5000.0f);
}