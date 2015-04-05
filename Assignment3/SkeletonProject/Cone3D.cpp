#include "Cone3D.h"
#include "3DClasses/Vertex.h"
#include <math.h>
#include "GfxStats.h"

Cone3D::Cone3D()
:BaseObject3D()
{

}

Cone3D::Cone3D(float bottomRadius, float length, int sliceCount, int stackCount)
:BaseObject3D()
{
	mRadiusBottom = bottomRadius;
	mRadiusTop = 0.0f;
	mLength = length;
	mSliceCount = sliceCount;
	mStackCount = stackCount;

	mpMaterial = new BaseMaterial(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 8.0f);
}

Cone3D::~Cone3D()
{

}

void Cone3D::Create(IDirect3DDevice9* gd3dDevice)
{
	HR(D3DXCreateCylinder(gd3dDevice, mRadiusBottom, mRadiusTop, mLength, mSliceCount, mStackCount, &mpMesh, NULL));
	GenerateTBNData();
	mNumVertices = mpMesh->GetNumVertices();
	mNumTriangles = mpMesh->GetNumFaces();

	mpMesh->GetVertexBuffer(&m_VertexBuffer);
	mpMesh->GetIndexBuffer(&m_IndexBuffer);

	buildTexCoords(gd3dDevice);
}

void Cone3D::buildTexCoords(IDirect3DDevice9* gd3dDevice)
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

	D3DXVECTOR3 maxPoint(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	D3DXVECTOR3 minPoint(FLT_MAX, FLT_MAX, FLT_MAX);

	for (UINT i = 0; i < temp->GetNumVertices(); ++i)
	{
		D3DXVec3Maximize(&maxPoint, &maxPoint, &vertices[i].mPos);
		D3DXVec3Minimize(&minPoint, &minPoint, &vertices[i].mPos);
	}

	float a = minPoint.z;
	float b = maxPoint.z;
	float h = b - a;

	for (int i = 0; i < (int)temp->GetNumVertices(); i++)
	{
		float x = vertices[i].mPos.x;
		float y = vertices[i].mPos.z;
		float z = vertices[i].mPos.y;

		float theta = atan2f(z, x);
		float y2 = y - b;

		float u = theta / (2.0f * D3DX_PI);
		float v = y2 / -h;

		vertices[i].mTexCoord.x = u;
		vertices[i].mTexCoord.y = v;
	}

	HR(temp->UnlockVertexBuffer());

	HR(temp->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY,
		elements, gd3dDevice, &mpMesh));

	ReleaseCOM(temp);
}

void Cone3D::Update()
{

}