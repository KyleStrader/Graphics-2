//=============================================================================
//              Adopted from Franc Luna's CubeDemo sample project.
//
// Please use this file to add classes / inherit and create various vertex classes
//=============================================================================
#ifndef VERTEX_H
#define VERTEX_H
//=============================================================================
#include <d3dx9.h>
//=============================================================================
// Call in constructor and destructor, respectively, of derived application class.
void InitAllVertexDeclarations();
void DestroyAllVertexDeclarations();
//=============================================================================

struct VertexPos
{
	VertexPos() :mPos(0.0f, 0.0f, 0.0f),
		mNormal(0.0f, 0.0f, 0.0f),
		mTexCoord(0.0f, 0.0f){}
	
	VertexPos(float x, float y, float z) :mPos(x, y, z), 
		mTexCoord(0.0f, 0.0f), 
		mNormal(0.0f, 0.0f, 0.0f){}
	
	VertexPos(D3DXVECTOR3& pos, D3DXVECTOR3& texCoord, D3DXVECTOR3& normal/*, D3DXVECTOR3 binormal, D3DXVECTOR3 tangent*/) :mPos(pos),
		mTexCoord(texCoord),
		mNormal(normal)/*,
		mBinormal(binormal),
		mTangent(tangent)*/{}
	
	VertexPos(const D3DXVECTOR3& pos):mPos(pos){}

	D3DXVECTOR3 mPos;
	D3DXVECTOR3 mTangent;
	D3DXVECTOR3 mBinormal;
	D3DXVECTOR3 mNormal;
	D3DXVECTOR2 mTexCoord;
	static IDirect3DVertexDeclaration9* Decl;
};


//=============================================================================
#endif // VERTEX_H