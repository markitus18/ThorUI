#ifndef __RECT_H__
#define __RECT_H__

#include "Shape.h"

struct Rect : public Shape
{
	Rect() {};
	Rect(float x, float y, float w, float h, float angle = 0) : Shape(Vec2(x, y), angle), size(x, y)
	{

	}
	Rect(Vec2 pos, Vec2 size, float angle = 0) : Shape(pos, angle), size(size)
	{

	}

	bool Contains(Vec2 point) override
	{
		return (point.x >= pos.x && point.x <= pos.x + size.x &&
				point.y >= pos.y && point.y <= pos.y + size.y);
	}

	Vec2 size;
};

#endif // !__RECT_H__
