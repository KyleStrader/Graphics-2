#include "Sphere3D.h"
#include "3DClasses\Vertex.h"
#include "GfxStats.h"

Sphere3D::Sphere3D()
{

}

Sphere3D::Sphere3D(float radius, int longitude, int latitude)
:BaseObject3D()
{
	mRadius = radius;
	mSlices = longitude;
	mStacks = latitude;

	mpMaterial = new BaseMaterial(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 8.0f);
}

Sphere3D::~Sphere3D()
{
	delete mpMaterial;
}

void Sphere3D::Create(IDirect3DDevice9* gd3dDevice)
{
	HR(D3DXCreateSphere(gd3dDevice, mRadius, mSlices, mStacks, &mpMesh, NULL));
	GenerateTBNData();
	mNumVertices = mpMesh->GetNumVertices();
	mNumTriangles = mpMesh->GetNumFaces();

	mpMesh->GetVertexBuffer(&m_VertexBuffer);
	mpMesh->GetIndexBuffer(&m_IndexBuffer);
	buildTexCoords(gd3dDevice);
}

void Sphere3D::buildTexCoords(IDirect3DDevice9* gd3dDevice)
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
		// Convert to spherical coordinates.
		D3DXVECTOR3 p = vertices[i].mPos;

		float theta = atan2f(p.z, p.x);
		float phi = acosf(p.y / sqrtf(p.x*p.x + p.y*p.y + p.z*p.z));

		// Phi and theta give the texture coordinates, but are not in 
		// the range [0, 1], so scale them into that range.

		float u = theta / (2.0f*D3DX_PI);
		float v = phi / D3DX_PI;

		// Save texture coordinates.

		vertices[i].mTexCoord.x = u;
		vertices[i].mTexCoord.y = v;
	}

	HR(temp->UnlockVertexBuffer());

	HR(temp->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY,
		elements, gd3dDevice, &mpMesh));

	ReleaseCOM(temp);
}

void Sphere3D::Update()
{

}