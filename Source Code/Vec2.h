#ifndef __VEC2_H__
#define __VEC2_H__

struct Vec2
{
	//Methods
	Vec2() {};
	Vec2(const Vec2& vec) { x = vec.x; y = vec.y; }
	Vec2(float x, float y) : x(x), y(y) {}

	void Set(float x, float y) { this->x = x; this->y = y; }

	//Variables
	float x = 0, y = 0;
};

#endif