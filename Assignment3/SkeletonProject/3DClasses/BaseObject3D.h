//=============================================================================
//                              BaseObject3D
//
// Writen by Adi Bar-Lev, 2013
// EGP 300-101, Graphics Programming II  - skeleton project
//
// Base class that can handle 3D rendergin via Vertex and Index buffer
//=============================================================================
#ifndef _BASE_OBJECT_3D_H
#define _BASE_OBJECT_3D_H
//=============================================================================
#pragma once
//=============================================================================
#include <d3dx9.h>
#include "../BaseMaterial.h"
#include "../d3dUtil.h"
#include "../SkyboxEffect.h"
//=============================================================================
struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;
//=============================================================================
class BaseObject3D
{
protected:

	ID3DXMesh*					mpMesh;
    D3DXMATRIX                  m_World;

	IDirect3DVertexBuffer9*     m_VertexBuffer;
	IDirect3DIndexBuffer9*      m_IndexBuffer;

	BaseMaterial* mpMaterial;

	int mNumVertices;
	int mNumTriangles;
	DWORD* mpNormals;

protected:
    // Replace the code in the following methods
    virtual void buildVertexBuffer( IDirect3DDevice9* gd3dDevice );
    virtual void buildIndexBuffer( IDirect3DDevice9* gd3dDevice );
	virtual void buildTexCoords(IDirect3DDevice9* gd3dDevice);

public:
    BaseObject3D(void);
    ~BaseObject3D(void);

    // Replace or add to the following code as you progress with the material
    virtual void Create( IDirect3DDevice9* gd3dDevice );
    virtual void Render( IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection );

	void SetViewerPos(D3DXVECTOR3 &pos);
	void ConnectEffect(EffectType type, ID3DXEffect* fx);
	void ConnectToTexture(EffectType type, IDirect3DDevice9* gd3dDevice, std::string sourceFile);
	void ConnectToCubeMap(IDirect3DDevice9* gd3dDevice, std::string sourceFile);
	void MoveToPoint(float x, float y, float z);
	void RotateAroundAxis(D3DXVECTOR3 axis, float degrees);

	ID3DXEffect* GetEffect(EffectType type) { return mpMaterial->GetEffect(type); };
	void SetReflectivity(float reflectivity);

	void ToggleDiffuse() { return mpMaterial->ToggleDiffuse(); };
	void ToggleAmbient() { return mpMaterial->ToggleAmbient(); };
	void ToggleSpecular() { return mpMaterial->ToggleSpecular(); };
	void ToggleReflection() { return mpMaterial->ToggleReflection();};
};
//=============================================================================
#endif // _BASE_OBJECT_3D_H

