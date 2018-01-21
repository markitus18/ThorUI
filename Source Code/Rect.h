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

	void SetPos(Vec2 pos)
	{
		this->pos = pos;
		CalcCorners();
	}

	void SetSize(Vec2 size)
	{
		this->size = size;
		CalcCorners();
	}

	void SetAngle(float angle)
	{
		this->angle = angle;
		CalcRotCorners();
	}

	void CalcCorners()
	{
		corners[0] = (pos - (pivot * size));
		corners[1] = pos + Vec2(size.x - pivot.x * size.x, -pivot.y * size.y);
		corners[2] = pos + (size - pivot * size);
		corners[3] = pos + Vec2(-pivot.x * size.x, size.y - pivot.y * size.y);
		CalcRotCorners();
	}

	void CalcRotCorners()
	{
		for (int i = 0; i < 4; ++i)
		{
			rot_corners[i] = corners[i];
			rot_corners[i].Rotate(pos, angle);
		}
	}

	Vec2 GetCenterPos() //TODO: adding const -> operator "-" const
	{
		return pos + (Vec2(0.5, 0.5) - pivot) * size;
	}

	Vec2 size;
	Vec2 corners[4];
	Vec2 rot_corners[4];
};

#endif // !__RECT_H__
