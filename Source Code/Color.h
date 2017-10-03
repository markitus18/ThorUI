#ifndef __COLOR_H__
#define __COLOR_H__

struct Color
{
	float r = 1.0f, g = 1.0f, b = 1.0f, a = 1.0f;

	Color() {};
	Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {};
	float* ptr() { return (float*)this; };

	static Color White()	{ return Color(1.0f, 1.0f, 1.0f, 1.0f); };
	static Color Red()		{ return Color(1.0f, 0.0f, 0.0f, 1.0f); };
	static Color Green()	{ return Color(0.0f, 1.0f, 0.0f, 1.0f); }
	static Color Blue()		{ return Color(0.0f, 0.0f, 1.0f, 1.0f); };

};
#endif
