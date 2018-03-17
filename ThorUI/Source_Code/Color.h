#ifndef __COLOR_H__
#define __COLOR_H__

#include "SDL2-2.0.6\include\SDL_pixels.h"

struct Color
{
	float r = 1.0f, g = 1.0f, b = 1.0f, a = 1.0f;

	THORUI_API Color() {};
	THORUI_API Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {};
	THORUI_API float* ptr() { return (float*)this; };
	THORUI_API SDL_Color ToSDL() { SDL_Color ret; ret.r = r*255; ret.g = g*255; ret.b = b*255; ret.a = a*255; return ret; }
	THORUI_API bool operator==(const Color& other) { return (r == other.r && g == other.g && b == other.b && a == other.a); }
	THORUI_API bool operator!=(const Color& other) { return !(r == other.r && g == other.g && b == other.b && a == other.a); }

	static THORUI_API Color White()	{ return Color(1.0f, 1.0f, 1.0f, 1.0f); };
	static THORUI_API Color Red()		{ return Color(1.0f, 0.05f, 0.2f, 1.0f); };
	static THORUI_API Color Green()	{ return Color(0.235f, 0.7f, 0.29f, 1.0f); }
	static THORUI_API Color Blue()		{ return Color(0.f, 0.50f, 0.78f, 1.0f); };
	static THORUI_API Color Yellow()	{ return Color(1.0f, 0.88f, 0.09f, 1.0f); }
	static THORUI_API Color Orange()	{ return Color(0.96f, 0.50f, 0.17f, 1.0f); }
	static THORUI_API Color Purple()	{ return Color(0.568f, 0.27f, 0.7f, 1.0f); }
	static THORUI_API Color Cyan()		{ return Color(0.274f, 0.94f, 0.94f, 1.0f); }
	static THORUI_API Color Magenta()	{ return Color(0.94f, 0.19f, 0.90f, 1.0f); }
	static THORUI_API Color Lime()		{ return Color(0.82f, 0.96f, 0.235f, 1.0f); }
	static THORUI_API Color Pink()		{ return Color(0.98f, 0.745f, 0.745f, 1.0f); }
	static THORUI_API Color Teal()		{ return Color(0.0f, 0.501f, 0.501f, 1.0f); }
	static THORUI_API Color Lavender() { return Color(0.90f, 0.74f, 1.0f, 1.0f); }
	static THORUI_API Color Brown()	{ return Color(0.66f, 0.43f, 0.156f, 1.0f); }
	static THORUI_API Color Beige()	{ return Color(1.0f, 0.98f, 0.78f, 1.0f); }
	static THORUI_API Color Maroon()	{ return Color(0.50f, 0.0f, 0.0f, 1.0f); }
	static THORUI_API Color Mint()		{ return Color(0.66f, 1.0f, 0.76f, 1.0f); }
	static THORUI_API Color Olive()	{ return Color(0.50f, 0.50f, 0.0f, 1.0f); }
	static THORUI_API Color Coral()	{ return Color(1.0f, 0.84f, 0.70f, 1.0f); }
	static THORUI_API Color DarkBlue() { return Color(0.0f, 0.0f, 0.50f, 1.0f); }
	static THORUI_API Color Grey()		{ return Color(0.50f, 0.50f, 0.50f, 1.0f); }
	static THORUI_API Color Black()	{ return Color(0.0f, 0.0f, 0.0f, 1.0f); }
	static THORUI_API Color Fuchsia()	{ return Color(1.0f, 0.0f, 1.0f, 1.0f); }



};
#endif
