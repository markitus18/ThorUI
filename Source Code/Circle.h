#ifndef  __CIRCLE_H__
#define __CIRCLE_H__

#include "Shape.h"

struct Circle : public Shape
{
	Circle() {};
	Circle(Vec2 pos, float radius) : Shape(pos, 0), radius(radius)
	{

	}

	bool Contains(Vec2 point)
	{
		return pos.Distance(point) < radius;
	}

	float radius;
};


#endif // ! __CIRCLE_H__
