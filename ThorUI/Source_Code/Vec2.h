#ifndef __VEC2_H__
#define __VEC2_H__

#ifdef THORUI_EXPORTS
#define THORUI_API __declspec(dllexport) 
#else
#define THORUI_API __declspec(dllimport) 
#endif

#include <math.h>

class Mat3x3;

struct Vec2
{
	//Methods
	THORUI_API Vec2() {};
	THORUI_API Vec2(const Vec2& vec) { x = vec.x; y = vec.y; }
	THORUI_API Vec2(float x, float y) : x(x), y(y) {}

	THORUI_API void Set(float x, float y) { this->x = x; this->y = y; }
	THORUI_API float* Ptr() { return &x; }

	THORUI_API Vec2 FitInRect(const Vec2& rect);

	THORUI_API float* operator&() { return (float*)this; }

	//Operators
	THORUI_API Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
	THORUI_API Vec2 operator+=(const Vec2& other) { return *this = Vec2(x + other.x, y + other.y); }

	THORUI_API Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
	THORUI_API Vec2 operator-=(const Vec2& other) { return *this = Vec2(x - other.x, y - other.y); }
	THORUI_API Vec2 operator-() const { return Vec2(-x, -y); }

	THORUI_API Vec2 operator*(const Vec2& other) const { return Vec2(x * other.x, y * other.y); }
	THORUI_API Vec2 operator*=(const Vec2& other) { return *this = Vec2(x * other.x, y * other.y); }

	THORUI_API Vec2 operator*(const float& factor) const { return Vec2(x * factor, y * factor); }
	THORUI_API Vec2 operator*=(const float& factor) { return *this = Vec2(x * factor, y * factor); }

	THORUI_API Vec2 operator/(const Vec2& other) const { return Vec2(x / other.x, y / other.y); }
	THORUI_API Vec2 operator/=(const Vec2& other) { return *this = Vec2(x / other.x, y / other.y); }

	THORUI_API Vec2 operator/(const float& factor) const { return Vec2(x / factor, y / factor); }
	THORUI_API Vec2 operator/=(const float& factor) { return *this = Vec2(x / factor, y / factor); }

	THORUI_API bool operator==(const Vec2& other) const { return x == other.x && y == other.y; }
	THORUI_API bool operator!=(const Vec2& other) const { return x != other.x || y != other.y; }

	static THORUI_API Vec2 zero() { return Vec2(0, 0); };
	static THORUI_API Vec2 one() { return Vec2(1, 1); };

	THORUI_API float Lenght() const { return sqrt(x * x + y * y); }
	THORUI_API float Angle() const { return atan2(y, x); }

	THORUI_API void Normalize() { *this /= Lenght(); }
	THORUI_API Vec2 Normalized() const { return (*this / Lenght()); }
	//*Perform a rotation of the current point
	//		*pivot - center of rotation
	//		*angle - angle of the rotation in radians
	THORUI_API void Rotate(Vec2 pivot, float angle);
	THORUI_API void Transform(const Mat3x3& mat);
	static THORUI_API Vec2 Lerp(Vec2 begin, Vec2 end, float ratio);

	//Variables
	float x = 0, y = 0;
};

#endif