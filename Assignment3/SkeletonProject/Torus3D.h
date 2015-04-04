#pragma once
#include "C:\Users\Bob\Documents\Graphics 2\Assignment3\SkeletonProject\3DClasses\BaseObject3D.h"
class Torus3D :
public BaseObject3D
{
public:
	Torus3D();
	Torus3D(float innerRadius, float outerRadius, UINT numSides, UINT numRings);
	~Torus3D();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void buildTexCoords(IDirect3DDevice9* gd3dDevice);
	void Update();

private:
	float mInnerRadius;
	float mOuterRadius;
	UINT mNumSides;
	UINT mNumRings;

	float mNumVertices;
	float mNumTriangles;
};

