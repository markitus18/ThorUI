#ifndef  __CIRCLE_H__
#define __CIRCLE_H__

#include "Shape.h"

struct Circle : public Shape
{
public:
	THORUI_API Circle() {};
	THORUI_API Circle(Vec2 pos, float radius) : Shape(pos, 0), radius(radius)
	{

	}

	THORUI_API bool Contains(Vec2 point) override
	{
		Vec2 dst = pos - point;
		return dst.Lenght() < radius;
	}

	float radius;
};


#endif // ! __CIRCLE_H__
