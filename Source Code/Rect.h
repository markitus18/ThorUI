#ifndef __RECT_H__
#define __RECT_H__

#include "Vec2.h"

struct Rect
{
	Rect() {};
	Rect(float x, float y, float w, float h, float angle = 0) : pos(x, y), size(x, y), angle(angle)
	{

	}
	Rect(Vec2 pos, Vec2 size, float angle = 0) : pos(pos), size(size), angle(angle)
	{

	}

	bool Contains(Vec2 point)
	{
		return (point.x >= pos.x && point.x <= pos.x + size.x &&
				point.y >= pos.y && point.y <= pos.y + size.y);
	}

	float angle;
	Vec2 pos;
	Vec2 size;
};

#endif // !__RECT_H__
