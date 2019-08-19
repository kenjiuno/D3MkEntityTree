
#include "StdAfx.h"
#include "My3DMath.h"
#include <math.h>
#include <float.h>

namespace My3DMath
{

float Vtx3::Len() const
{
	return sqrt(x * x + y * y + z * z);
};

void BBox3::Empty()
{
	vmin.x = vmin.y = vmin.z = +18000;
	vmax.x = vmax.y = vmax.z = -18000;
}

Quatern &Quatern::Identify(const Vtx3 &v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	float t = 1.0f -x*x -y*y -z*z;
	w = (t < 0)
		? (float)0
		: (float)-sqrt(t);

	return *this;
}

};
