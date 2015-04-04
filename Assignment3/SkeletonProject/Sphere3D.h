#include "3DClasses/BaseObject3D.h"

class Sphere3D : public BaseObject3D
{
public:
	Sphere3D();
	Sphere3D(float radius, int slices, int stacks);
	~Sphere3D();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void buildTexCoords(IDirect3DDevice9* gd3dDevice);
	void Update();

private:
	float mRadius;
	int mSlices;
	int mStacks;

	int mNumVertices;
	int mNumTriangles;
};