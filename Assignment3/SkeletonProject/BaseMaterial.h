//=============================================================================
//                              BaseMaterial
//
// Writen by Adi Bar-Lev, 2013
// EGP 300-101, Graphics Programming II  - skeleton project
//
// Base class that contains the most intrinsic parameters to implement per model
// lighting / shaders effects.
//=============================================================================
#pragma once
#include "d3dApp.h"
//=============================================================================
class BaseMaterial
{
protected:
	//------------- Texture --------------

	IDirect3DTexture9* mTex;

	//-------------- Effect --------------
    
	ID3DXEffect*        m_Effect;               // the shader associate effect file

    //-------- Material Parameters -------

	D3DXVECTOR3			mLightVecW;
	D3DXCOLOR			mAmbientMtrl;
	D3DXCOLOR			mAmbientLight;
	D3DXCOLOR			mDiffuseMtrl;
	D3DXCOLOR			mDiffuseLight;
	D3DXCOLOR			mSpecularMtrl;
	D3DXCOLOR			mSpecularLight;
	float				mSpecularPower;

    D3DXMATRIX          m_WorldMat;
	D3DXMATRIX			m_WorldInverseTranspose;
	D3DXMATRIX          m_ViewProjMat;
	D3DXMATRIX          m_ProjMat;
	D3DXMATRIX			m_WVP_Mat;

    float               m_Shininess;            // specualr power


    //---------- Shader Handles ----------
    // Generic shader handles
    D3DXHANDLE          m_WorldMatHandle;		// World Matrix  
	D3DXHANDLE			m_WorldInverseTransposeHandle;
    D3DXHANDLE          m_ViewProjMatHandle;	// View Matrix
	D3DXHANDLE			m_WVP_Handle;

    D3DXHANDLE          m_LightPosWHandle;		// Position (spot/point) / Direction (directional)
	D3DXHANDLE          m_ViewerPosWHandle;		// Vector3

	D3DXHANDLE			m_TechHandle;
	D3DXHANDLE			m_LightVecW_Handle;

    // Material specific shader handles
	//-> Diffuse Component
    D3DXHANDLE          m_DIffuseColHandle;		// D3DX Color
	D3DXHANDLE			m_DiffuseMtrlHandle;	// D3DX Color
	D3DXHANDLE			m_DiffuseLightHandle;	// D3DX Color

	//->Specular Component
    D3DXHANDLE          m_SpecularColHandle;	// D3DX Color     
    D3DXHANDLE          m_ShininessHandle;		// D3DX Color
	D3DXHANDLE			m_SpecularMtrlHandle;	// D3DX Color
	D3DXHANDLE			m_SpecularLightHandle;	// D3DX Color
	D3DXHANDLE			m_SpecularPowerHandle;	// Float (power of specualrity)

	//->Ambient Component
	D3DXHANDLE          m_AmbientColHandle;		// D3DX Color
	D3DXHANDLE			m_AmbientMtrlHandle;	// D3DX Color
	D3DXHANDLE			m_AmbientLightHandle;	// D3DX Color

public:
    BaseMaterial(void);
	BaseMaterial(D3DXCOLOR diffuseColor, float shininess);
    virtual ~BaseMaterial(void);

	void setEyePos(D3DXVECTOR3 &pos);
    void ConnectToEffect( ID3DXEffect* effect);
	void ConnectToTexture(IDirect3DDevice9* gd3dDevice, std::string sourceFile);
    void Render( ID3DXMesh* mesh, D3DXMATRIX& worldMat, D3DXMATRIX& viewMat, D3DXMATRIX& projMat ); 

	void ToggleDiffuse();
	void ToggleAmbient();
	void ToggleSpecular();

	ID3DXEffect* GetEffect() { return m_Effect; };
};
//=============================================================================

