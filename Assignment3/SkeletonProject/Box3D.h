#include "3DClasses/BaseObject3D.h"
#include "d3dUtil.h"

class Box3D : public BaseObject3D
{
public:
	Box3D();
	Box3D(float width, float height, float depth);
	~Box3D();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void buildTexCoords(IDirect3DDevice9* gd3dDevice);
	void Update();

private:

	float mWidth;
	float mHeight;
	float mDepth;
};