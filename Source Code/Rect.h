#ifndef __RECT_H__
#define __RECT_H__

#include "Vec2.h"

struct Rect
{
	Rect() {};
	Rect(Vec2 min, Vec2 max, float angle = 0) : min(min), max(max), angle(angle)
	{

	}

	float angle;
	Vec2 min;
	Vec2 max;
};

#endif // !__RECT_H__
