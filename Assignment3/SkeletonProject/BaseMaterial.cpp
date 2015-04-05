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
    //m_Effects = NULL;

	//Init Material Colors
	mLightVecW = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	mDiffuseMtrl = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mDiffuseLight = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mAmbientMtrl = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mAmbientLight = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	mSpecularMtrl = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	mSpecularLight = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	mSpecularPower = 8.0f;
	mReflectivity = .5f;
	mTempReflect = mReflectivity;
}

BaseMaterial::BaseMaterial(D3DXCOLOR diffuseColor, float shininess)
{
	//m_Effect = NULL;

	//Init Material Colors
	mLightVecW = D3DXVECTOR3(1.5f, 1.5f, 1.5f);
	mDiffuseMtrl = diffuseColor;
	mDiffuseLight = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mAmbientMtrl = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mAmbientLight = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	mSpecularMtrl = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	mSpecularLight = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	mSpecularPower = shininess;
	mReflectivity = .5f;
	mTempReflect = mReflectivity;
}

void BaseMaterial::ToggleReflection()
{
	if (mReflectivity != 0.0f)
	{
		mTempReflect = mReflectivity;
		mReflectivity = 0;
	}
	else
	{
		mReflectivity = mTempReflect;
	}
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
	std::map<EffectType, ID3DXEffect*>::iterator it;
	for (it = m_Effects.begin(); it != m_Effects.end(); it++)
	{
		ReleaseCOM(it->second);
	}
}

void BaseMaterial::setEyePos(D3DXVECTOR3 &pos)
{
	std::map<EffectType, ID3DXEffect*>::iterator it;
	for (it = m_Effects.begin(); it != m_Effects.end(); it++)
	{
		HR(it->second->SetValue(m_ViewerPosWHandle, &pos, sizeof(D3DXVECTOR3)));
	}
}

//-----------------------------------------------------------------------------
// Need to add here a code that will associate with your shader parameters and 
// register them.

void BaseMaterial::ConnectToEffect( EffectType type, ID3DXEffect* effect )
{
	m_Effects[type] = effect;
	ID3DXEffect* tmpEffect = effect;

	switch (type)
	{
	case TYPE_LIGHTING_DIFFUSE:
		ConnectToDiffuseLightingEffect(effect);
		break;
	case TYPE_ENVIORNMENT_MAPPING:
		ConnectToEnviornmentMappingEffect(effect);
		break;
	case TYPE_NORMAL_MAPPING:
		ConnectToNormalMappingEffect(effect);
		break;
	}
}

void BaseMaterial::ConnectToDiffuseLightingEffect(ID3DXEffect* effect)
{
	// Obtain handles
	m_TechHandle = effect->GetTechniqueByName("AmbientDiffuseSpecTech");
	m_WVP_Handle = effect->GetParameterByName(0, "gWVP");
	m_WorldInverseTransposeHandle = effect->GetParameterByName(0, "gWorldInverseTranspose");
	m_LightVecW_Handle = effect->GetParameterByName(0, "gLightVecW");
	m_DiffuseMtrlHandle = effect->GetParameterByName(0, "gDiffuseMtrl");
	m_DiffuseLightHandle = effect->GetParameterByName(0, "gDiffuseLight");
	m_AmbientMtrlHandle = effect->GetParameterByName(0, "gAmbientMtrl");
	m_AmbientLightHandle = effect->GetParameterByName(0, "gAmbientLight");
	m_SpecularMtrlHandle = effect->GetParameterByName(0, "gSpecularMtrl");
	m_SpecularLightHandle = effect->GetParameterByName(0, "gSpecularLight");
	m_SpecularPowerHandle = effect->GetParameterByName(0, "gSpecularPower");
	m_ViewerPosWHandle = effect->GetParameterByName(0, "gEyePosW");
	m_WorldMatHandle = effect->GetParameterByName(0, "gWorld");
	m_ReflectivityHandle = effect->GetParameterByName(0, "gReflectivity");
}

void BaseMaterial::setReflectivity(float reflect)
{
	mReflectivity = reflect;
}

void BaseMaterial::setSpecPower(float specPower)
{
	mSpecularPower = specPower;
}

void BaseMaterial::IncreaseReflectivity()
{
	if (mReflectivity < 1.0f)
	{
		mReflectivity += .1f;
	}
	else mReflectivity = 1.0f;
}

void BaseMaterial::DecreaseReflectivity()
{
	if (mReflectivity > 0.0f)
	{
		mReflectivity -= .1f;
	}
	else mReflectivity = 0.0f;
}

void BaseMaterial::ConnectToEnviornmentMappingEffect(ID3DXEffect* effect)
{
	//Nothing here yet!!
}

void BaseMaterial::ConnectToNormalMappingEffect(ID3DXEffect* effect)
{
	//HNothing here yet!!
}

void BaseMaterial::ConnectToTexture(EffectType type, IDirect3DDevice9* gd3dDevice, std::string sourceFile)
{
	HR(D3DXCreateTextureFromFile(gd3dDevice, sourceFile.c_str(), &m_Textures[type]));
}

void BaseMaterial::ConnectToCubeMap(IDirect3DDevice9* gd3dDevice, std::string sourceFile)
{
	HR(D3DXCreateCubeTextureFromFile(gd3dDevice, sourceFile.c_str(), &m_CubeMap));
}

void BaseMaterial::ConnectToNormalMap(IDirect3DDevice9* gd3dDevice, std::string sourceFile)
{
	HR(D3DXCreateTextureFromFile(gd3dDevice, sourceFile.c_str(), &m_NormalMap));
}

void BaseMaterial::Render(ID3DXMesh* mesh, D3DXMATRIX& worldMat, D3DXMATRIX& viewMat, D3DXMATRIX& projMat)
{
	std::map<EffectType, ID3DXEffect*>::iterator it;
	for (it = m_Effects.begin(); it != m_Effects.end(); it++)
	{
		switch (it->first)
		{
		case TYPE_LIGHTING_DIFFUSE:
			RenderDiffuseLightingEffect(TYPE_LIGHTING_DIFFUSE, it->second, mesh, worldMat, viewMat, projMat);
			break;
		case TYPE_ENVIORNMENT_MAPPING:
			RenderEnviornmentMappingEffect(TYPE_ENVIORNMENT_MAPPING, it->second, mesh, worldMat, viewMat, projMat);
			break;
		case TYPE_NORMAL_MAPPING:
			RenderNormalMappingEffect(TYPE_NORMAL_MAPPING, it->second, mesh, worldMat, viewMat, projMat);
			break;
		}
	}
}

void BaseMaterial::RenderDiffuseLightingEffect(EffectType type, ID3DXEffect* effect, ID3DXMesh* mesh, D3DXMATRIX& worldMat, D3DXMATRIX& viewMat, D3DXMATRIX& projMat)
{
	HR(effect->SetTexture("gTex", m_Textures[type]));
	HR(effect->SetTexture("gNormalMap", m_NormalMap));
	HR(effect->SetTexture("gCubeMap", m_CubeMap));
	HR(effect->SetMatrix(m_WVP_Handle, &(worldMat*viewMat*projMat)));
	D3DXMATRIX worldInverseTranspose;
	D3DXMatrixInverse(&worldInverseTranspose, 0, &worldMat);
	D3DXMatrixTranspose(&worldInverseTranspose, &worldInverseTranspose);
	HR(effect->SetMatrix(m_WorldInverseTransposeHandle, &worldInverseTranspose));
	HR(effect->SetValue(m_LightVecW_Handle, &mLightVecW, sizeof(D3DXVECTOR3)));
	HR(effect->SetValue(m_DiffuseMtrlHandle, &mDiffuseMtrl, sizeof(D3DXCOLOR)));
	HR(effect->SetValue(m_DiffuseLightHandle, &mDiffuseLight, sizeof(D3DXCOLOR)));
	HR(effect->SetValue(m_AmbientMtrlHandle, &mAmbientMtrl, sizeof(D3DXCOLOR)));
	HR(effect->SetValue(m_AmbientLightHandle, &mAmbientLight, sizeof(D3DXCOLOR)));
	HR(effect->SetValue(m_SpecularLightHandle, &mSpecularLight, sizeof(D3DXCOLOR)));
	HR(effect->SetValue(m_SpecularMtrlHandle, &mSpecularMtrl, sizeof(D3DXCOLOR)));
	HR(effect->SetFloat(m_SpecularPowerHandle, mSpecularPower));
	HR(effect->SetFloat(m_ReflectivityHandle, mReflectivity));
	HR(effect->SetMatrix(m_WorldMatHandle, &worldMat));
	HR(effect->CommitChanges());
}

void BaseMaterial::RenderEnviornmentMappingEffect(EffectType type, ID3DXEffect* effect, ID3DXMesh* mesh, D3DXMATRIX& worldMat, D3DXMATRIX& viewMat, D3DXMATRIX& projMat)
{
	//Nothing here yet!!
}

void BaseMaterial::RenderNormalMappingEffect(EffectType type, ID3DXEffect* effect, ID3DXMesh* mesh, D3DXMATRIX& worldMat, D3DXMATRIX& viewMat, D3DXMATRIX& projMat)
{
	//HNothing here yet!!
}

//=============================================================================
