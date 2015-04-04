#pragma once
#include ".\3DClasses\BaseObject3D.h"
class Teapot3D :
	public BaseObject3D
{
public:
	Teapot3D();
	~Teapot3D();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void buildTexCoords(IDirect3DDevice9* gd3dDevice);
	void Update();

private:
	int mNumVertices;
	int mNumTriangles;
};

