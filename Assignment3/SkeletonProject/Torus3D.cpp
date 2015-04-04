#include "Torus3D.h"
#include "3DClasses\Vertex.h"
#include "GfxStats.h"

Torus3D::Torus3D()
{
}

Torus3D::Torus3D(float innerRadius, float outerRadius, UINT numSides, UINT numRings)
:BaseObject3D()
{
	mpMaterial = new BaseMaterial(D3DXCOLOR(0.368f, 0.361f, 0.921f, 1.0f), 8.0f);

	mInnerRadius = innerRadius;
	mOuterRadius = outerRadius;
	mNumSides = numSides;
	mNumRings = numRings;
}

Torus3D::~Torus3D()
{
	delete mpMaterial;
}

void Torus3D::Create(IDirect3DDevice9* gd3dDevice)
{
	HR(D3DXCreateTorus(gd3dDevice, mInnerRadius, mOuterRadius, mNumSides, mNumRings, &mpMesh, NULL));
	mNumVertices = mpMesh->GetNumVertices();
	mNumTriangles = mpMesh->GetNumFaces();

	mpMesh->GetVertexBuffer(&m_VertexBuffer);
	mpMesh->GetIndexBuffer(&m_IndexBuffer);
	buildTexCoords(gd3dDevice);
}

void Torus3D::buildTexCoords(IDirect3DDevice9* gd3dDevice)
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

void Torus3D::Update()
{

}
