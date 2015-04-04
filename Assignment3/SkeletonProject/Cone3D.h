#include "3DClasses/BaseObject3D.h"
#include "d3dUtil.h"

class Cone3D: public BaseObject3D
{
public:
	Cone3D();
	Cone3D(float bottomRadius, float length, int sliceCount, int stackCount);
	~Cone3D();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	void buildTexCoords(IDirect3DDevice9* gd3dDevice);
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