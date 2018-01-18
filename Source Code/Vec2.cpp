#include "Vec2.h"
#include "Mat3x3.h"

Vec2 Vec2::FitInRect(const Vec2& rect)
{
	Vec2 ret(*this);
	if (ret.x > rect.x)
	{
		float ratio = ret.x / rect.x;
		ret /= ratio;
	}
	if (ret.y > rect.y)
	{
		float ratio = ret.y / rect.y;
		ret /= ratio;
	}
	return ret;
}

void Vec2::Rotate(Vec2 pivot, float angle)
{
	Vec2 rot_vec = *this - pivot;

	float new_angle = rot_vec.Angle() + angle;
	float lenght = rot_vec.Lenght();

	x = pivot.x + cos(new_angle) * lenght;
	y = pivot.y + sin(new_angle) * lenght;
}

void Vec2::Transform(const Mat3x3& mat)
{
	x = x * mat[0][0] + x * mat[0][1] + mat[0][2];
	y = y * mat[1][0] + y * mat[1][1] + mat[1][2];
}
