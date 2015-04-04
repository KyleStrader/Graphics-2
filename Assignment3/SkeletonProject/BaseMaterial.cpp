//=============================================================================
//                              BaseMaterial
//
// Writen by Adi Bar-Lev, 2013
// EGP 300-101, Graphics Programming II  - skeleton project
//
// Base class that contains the most intrinsic parameters to implement per model
// lighting / shaders effects.
//=============================================================================
#include "BaseMaterial.h"
#include "3DClasses\Vertex.h"
//=============================================================================



BaseMaterial::BaseMaterial(void)
{
    m_Effect = NULL;

	//Init Material Colors
	mLightVecW = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	mDiffuseMtrl = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mDiffuseLight = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mAmbientMtrl = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mAmbientLight = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	mSpecularMtrl = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	mSpecularLight = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	mSpecularPower = 8.0f;
}

BaseMaterial::BaseMaterial(D3DXCOLOR diffuseColor, float shininess)
{
	m_Effect = NULL;

	//Init Material Colors
	mLightVecW = D3DXVECTOR3(0.5f, 0.5f, 1.5f);
	mDiffuseMtrl = diffuseColor;
	mDiffuseLight = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mAmbientMtrl = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mAmbientLight = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	mSpecularMtrl = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	mSpecularLight = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	mSpecularPower = shininess;
}

void BaseMaterial::ToggleDiffuse()
{
	if (mDiffuseLight == D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f))
	{
		mDiffuseLight = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else mDiffuseLight = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

void BaseMaterial::ToggleAmbient()
{
	if (mAmbientLight == D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f))
	{
		mAmbientLight = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	}
	else mAmbientLight = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

void BaseMaterial::ToggleSpecular()
{
	if (mSpecularLight == D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f))
	{
		mSpecularLight = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	}
	else mSpecularLight = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//-----------------------------------------------------------------------------
// Relase shader, blah...
BaseMaterial::~BaseMaterial(void)
{
	ReleaseCOM(m_Effect);
}

void BaseMaterial::setEyePos(D3DXVECTOR3 &pos)
{
	HR(m_Effect->SetValue(m_ViewerPosWHandle, &pos, sizeof(D3DXVECTOR3)));
}

//-----------------------------------------------------------------------------
// Need to add here a code that will associate with your shader parameters and 
// register them.

void BaseMaterial::ConnectToEffect( ID3DXEffect* effect )
{
	m_Effect = effect;

	// Obtain handles
	//m_TechHandle = m_Effect->GetTechniqueByName("PhongTech");
	m_TechHandle = m_Effect->GetTechniqueByName("AmbientDiffuseSpecTech");
	m_WVP_Handle = m_Effect->GetParameterByName(0, "gWVP");
	m_WorldInverseTransposeHandle = m_Effect->GetParameterByName(0, "gWorldInverseTranspose");
	m_LightVecW_Handle = m_Effect->GetParameterByName(0, "gLightVecW");
	m_DiffuseMtrlHandle = m_Effect->GetParameterByName(0, "gDiffuseMtrl");
	m_DiffuseLightHandle = m_Effect->GetParameterByName(0, "gDiffuseLight");
	m_AmbientMtrlHandle = m_Effect->GetParameterByName(0, "gAmbientMtrl");
	m_AmbientLightHandle = m_Effect->GetParameterByName(0, "gAmbientLight");
	m_SpecularMtrlHandle = m_Effect->GetParameterByName(0, "gSpecularMtrl");
	m_SpecularLightHandle = m_Effect->GetParameterByName(0, "gSpecularLight");
	m_SpecularPowerHandle = m_Effect->GetParameterByName(0, "gSpecularPower");
	m_ViewerPosWHandle = m_Effect->GetParameterByName(0, "gEyePosW");
	m_WorldMatHandle = m_Effect->GetParameterByName(0, "gWorld");
}

void BaseMaterial::ConnectToTexture(IDirect3DDevice9* gd3dDevice, std::string sourceFile)
{
	HR(D3DXCreateTextureFromFile(gd3dDevice, sourceFile.c_str(), &mTex));
}

void BaseMaterial::Render(ID3DXMesh* mesh, D3DXMATRIX& worldMat, D3DXMATRIX& viewMat, D3DXMATRIX& projMat)
{
	HR(m_Effect->SetTexture("gTex", mTex));
	HR(m_Effect->SetMatrix(m_WVP_Handle, &(worldMat*viewMat*projMat)));	
	D3DXMATRIX worldInverseTranspose;
	D3DXMatrixInverse(&worldInverseTranspose, 0, &worldMat);
	D3DXMatrixTranspose(&worldInverseTranspose, &worldInverseTranspose);
	HR(m_Effect->SetMatrix(m_WorldInverseTransposeHandle, &worldInverseTranspose));
	HR(m_Effect->SetValue(m_LightVecW_Handle, &mLightVecW, sizeof(D3DXVECTOR3)));
	HR(m_Effect->SetValue(m_DiffuseMtrlHandle, &mDiffuseMtrl, sizeof(D3DXCOLOR)));
	HR(m_Effect->SetValue(m_DiffuseLightHandle, &mDiffuseLight, sizeof(D3DXCOLOR)));
	HR(m_Effect->SetValue(m_AmbientMtrlHandle, &mAmbientMtrl, sizeof(D3DXCOLOR)));
	HR(m_Effect->SetValue(m_AmbientLightHandle, &mAmbientLight, sizeof(D3DXCOLOR)));
	HR(m_Effect->SetValue(m_SpecularLightHandle, &mSpecularLight, sizeof(D3DXCOLOR)));
	HR(m_Effect->SetValue(m_SpecularMtrlHandle, &mSpecularMtrl, sizeof(D3DXCOLOR)));
	HR(m_Effect->SetFloat(m_SpecularPowerHandle, mSpecularPower));
	HR(m_Effect->SetMatrix(m_WorldMatHandle, &worldMat));
	HR(m_Effect->CommitChanges());
}

//=============================================================================
