#include "Box3D.h"
#include "3DClasses/Vertex.h"
#include <math.h>
#include "GfxStats.h"

Box3D::Box3D()
:BaseObject3D()
{

}

Box3D::Box3D(float width, float height, float depth)
: BaseObject3D()
{
	mWidth = width;
	mHeight = height;
	mDepth = depth;

	mpMaterial = new BaseMaterial(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 8.0f);
}

Box3D::~Box3D()
{
	delete mpMaterial;
}

void Box3D::Create(IDirect3DDevice9* gd3dDevice)
{
	HR(D3DXCreateBox(gd3dDevice, mWidth, mHeight, mDepth, &mpMesh, NULL));
	mNumVertices = mpMesh->GetNumVertices();
	mNumTriangles = mpMesh->GetNumFaces();

	mpMesh->GetVertexBuffer(&m_VertexBuffer);
	mpMesh->GetIndexBuffer(&m_IndexBuffer);
	buildTexCoords(gd3dDevice);
}

void Box3D::buildTexCoords(IDirect3DDevice9* gd3dDevice)
{
	D3DVERTEXELEMENT9 elements[64];
	UINT numElements = 0;
	VertexPos::Decl->GetDeclaration(elements, &numElements);

	ID3DXMesh* temp = 0;
	HR(mpMesh->CloneMesh(D3DXMESH_SYSTEMMEM,
		elements, gd3dDevice, &temp));

	ReleaseCOM(mpMesh);

	// Now generate texture coordinates for each vertex.
	VertexPos* vertices = 0;
	HR(temp->LockVertexBuffer(0, (void**)&vertices));

	for (int i = 0; i < (int)temp->GetNumVertices(); i++)
	{
		if (i < 4 || (i > 7 && i < 12))
		{
			vertices[i].mTexCoord.x = (float)vertices[i].mPos.y;
			vertices[i].mTexCoord.y = (float)vertices[i].mPos.z;
		}
		else if (i < 24 && i > 15)
		{
			vertices[i].mTexCoord.x = (float)vertices[i].mPos.x;
			vertices[i].mTexCoord.y = (float)vertices[i].mPos.y;
		}
		else
		{
			vertices[i].mTexCoord.x = (float)vertices[i].mPos.x;
			vertices[i].mTexCoord.y = (float)vertices[i].mPos.z;
		}

		if (vertices[i].mTexCoord.x < 1.0f)
			vertices[i].mTexCoord.x = 1.0f;
		if (vertices[i].mTexCoord.y < 1.0f)
			vertices[i].mTexCoord.y = 1.0f;
	}

	HR(temp->UnlockVertexBuffer());

	HR(temp->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY,
		elements, gd3dDevice, &mpMesh));

	ReleaseCOM(temp);
}

void Box3D::Update()
{

}