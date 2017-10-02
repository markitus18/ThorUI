#ifndef __COLOR_H__
#define __COLOR_H__

struct Color
{
	float r, g, b, a;

	Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {};
	float* ptr() { return (float*)this; };

	static Color Red()		{ return Color(1.0f, 0.0f, 0.0f, 1.0f); };
	static Color Green()	{ return Color(0.0f, 1.0f, 0.0f, 1.0f);}
	static Color Blue()		{ return Color(0.0f, 0.0f, 1.0f, 1.0f);};

};
#endif
