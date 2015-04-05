//=============================================================================
//                              SkyboxEffect
//
// Writen by Adi Bar-Lev, 2013
// EGP 300-101, Graphics Programming II  - skeleton project
//
// Base class that contains the most intrinsic parameters to implement per model
// lighting / shaders effects.
//=============================================================================
#include "SkyboxEffect.h"
#include "3DClasses\Vertex.h"
//=============================================================================



SkyboxEffect::SkyboxEffect(void)
{
	m_Effect = NULL;
}

SkyboxEffect::SkyboxEffect(D3DXCOLOR diffuseColor, float shininess)
{
	m_Effect = NULL;
}

//-----------------------------------------------------------------------------
// Relase shader, blah...
SkyboxEffect::~SkyboxEffect(void)
{
	ReleaseCOM(m_Effect);
}

//-----------------------------------------------------------------------------
// Need to add here a code that will associate with your shader parameters and 
// register them.

void SkyboxEffect::ConnectToEffect(ID3DXEffect* effect)
{
	m_Effect = effect;

	// Obtain handles
	m_TechHandle = m_Effect->GetTechniqueByName("SkyTech");
	m_WVP_Handle = m_Effect->GetParameterByName(0, "gWVP");
	m_CubeMapHandle = m_Effect->GetParameterByName(0, "gCubeMap");
}

void SkyboxEffect::ConnectToTexture(IDirect3DDevice9* gd3dDevice, std::string sourceFile)
{
	HR(D3DXCreateCubeTextureFromFile(gd3dDevice, sourceFile.c_str(), &mCubeMap));
}

void SkyboxEffect::Render(ID3DXMesh* mesh, D3DXMATRIX& worldMat, D3DXMATRIX& viewMat, D3DXMATRIX& projMat)
{
	HR(m_Effect->SetTexture("gCubeMap", mCubeMap));
	HR(m_Effect->SetMatrix(m_WVP_Handle, &(worldMat*viewMat*projMat)));
	HR(m_Effect->CommitChanges());
}

//=============================================================================
