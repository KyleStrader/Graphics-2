//=============================================================================
//                              SkyboxEffect
//
// Writen by Adi Bar-Lev, 2013
// EGP 300-101, Graphics Programming II  - skeleton project
//
// Base class that contains the most intrinsic parameters to implement per model
// lighting / shaders effects.
//=============================================================================
#pragma once
#include "d3dApp.h"
#include "BaseMaterial.h"
//=============================================================================
class SkyboxEffect : public BaseMaterial
{
protected:
	//------------- Texture --------------

	IDirect3DCubeTexture9* mCubeMap;
	

	//-------------- Effect --------------

	ID3DXEffect*        m_Effect;               // the shader associate effect file

	//-------- Material Parameters -------

	D3DXMATRIX			m_WVP_Mat;

	//---------- Shader Handles ----------
	// Generic shader handles
	D3DXHANDLE			m_WVP_Handle;
	D3DXHANDLE			m_TechHandle;
	D3DXHANDLE			m_CubeMapHandle;

public:
	SkyboxEffect(void);
	SkyboxEffect(D3DXCOLOR diffuseColor, float shininess);
	virtual ~SkyboxEffect(void);

	void ConnectToEffect(ID3DXEffect* effect);
	void ConnectToTexture(IDirect3DDevice9* gd3dDevice, std::string sourceFile);
	void Render(ID3DXMesh* mesh, D3DXMATRIX& worldMat, D3DXMATRIX& viewMat, D3DXMATRIX& projMat);

	ID3DXEffect* GetEffect() { return m_Effect; };
};
//=============================================================================

