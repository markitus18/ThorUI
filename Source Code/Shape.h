#ifndef __SHAPE_H__
#define __SHAPE_H__

#include "Vec2.h"

struct Shape
{
public:
	Shape() {};
	Shape(Vec2 pos, float angle = 0) : pos(pos), angle(angle) {};
	virtual bool Contains(Vec2 point) = 0;

	void SetAngle(float angle)
	{
		while (angle > 360)
			angle -= 360;
		while (angle < -360)
			angle += 360;
		this->angle = angle;
	}
public:
	Vec2 pos;
	Vec2 pivot;
	float angle;
};

#endif // !

