#ifndef __VEC2_H__
#define __VEC2_H__

struct Vec2
{
	//Methods
	Vec2() {};
	Vec2(int x, int y) : x(x), y(y) {};

	void Set(int x, int y) { this->x = x; this->y = y; };

	//Variables
	int x = 0, y = 0;
};

#endif