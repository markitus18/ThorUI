#ifndef __VEC2_H__
#define __VEC2_H__

#include <math.h>

struct Vec2
{
	//Methods
	Vec2() {};
	Vec2(const Vec2& vec) { x = vec.x; y = vec.y; }
	Vec2(float x, float y) : x(x), y(y) {}

	void Set(float x, float y) { this->x = x; this->y = y; }
	float* Ptr() { return &x; }

	Vec2 FitInRect(const Vec2& rect)
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

	float* operator&() { return (float*)this; }

	//Operators
	Vec2 operator+(const Vec2& other) { return Vec2(x + other.x, y + other.y); }
	Vec2 operator+=(const Vec2& other) { return *this = Vec2(x + other.x, y + other.y); }

	Vec2 operator-(const Vec2& other) { return Vec2(x - other.x, y - other.y); }
	Vec2 operator-=(const Vec2& other) { return *this = Vec2(x - other.x, y - other.y); }

	Vec2 operator*(const Vec2& other) { return Vec2(x * other.x, y * other.y); }
	Vec2 operator*=(const Vec2& other) { return *this = Vec2(x * other.x, y * other.y); }

	Vec2 operator*(const float& factor) { return Vec2(x * factor, y * factor); }
	Vec2 operator*=(const float& factor) { return *this = Vec2(x * factor, y * factor); }

	Vec2 operator/(const Vec2& other) { return Vec2(x / other.x, y / other.y); }
	Vec2 operator/=(const Vec2& other) { return *this = Vec2(x / other.x, y / other.y); }

	Vec2 operator/(const float& factor) { return Vec2(x / factor, y / factor); }
	Vec2 operator/=(const float& factor) { return *this = Vec2(x / factor, y / factor); }

	static Vec2 zero() { return Vec2(0, 0); };
	static Vec2 one() { return Vec2(1, 1); };

	float Lenght() { return sqrt(x * x + y * y); };
	float Angle() { return atan2(y, x); }

	//*Perform a rotation of the current point
	//		*pivot - center of rotation
	//		*angle - angle of the rotation in radians
	void Rotate(Vec2 pivot, float angle)
	{
		Vec2 rot_vec = *this - pivot;

		float new_angle = rot_vec.Angle() + angle;
		float lenght = rot_vec.Lenght();

		x = pivot.x + cos(new_angle) * lenght;
		y = pivot.y + sin(new_angle) * lenght;
	}

	//Variables
	float x = 0, y = 0;
};

#endif