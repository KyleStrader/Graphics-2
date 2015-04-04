#include "3DClasses/BaseObject3D.h"

class Cylinder3D : public BaseObject3D
{
public:
	Cylinder3D();
	Cylinder3D(float bottomRadius, float topRadius, float length, int sliceCount, int stackCount);
	~Cylinder3D();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void buildTexCoords(IDirect3DDevice9* gd3dDevice);
	void Update();

private:
	float mLength;
	float mRadiusTop;
	float mRadiusBottom;

	int mStackCount;
	int mSliceCount;

	int mNumVertices;
	int mNumTriangles;
};