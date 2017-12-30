#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "Mat3x3.h"
#include "Vec2.h"

class Transform
{
	Vec2 position;
	Vec2 scale;
	float rotation;

	Mat3x3 matrix;
};
#endif // !__TRANSFORM_H__

