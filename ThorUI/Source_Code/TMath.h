#ifndef __MATH_H__
#define __MATH_H__

#ifdef THORUI_EXPORTS
#define THORUI_API __declspec(dllexport) 
#else
#define THORUI_API __declspec(dllimport) 
#endif

#define PI 3.14159265
#define DEGTORAD PI / 180
#define RADTODEG 180 / PI

#define DOT3(v1, v2) ((v1)[0] * (v2)[0] + (v1)[1] * (v2)[1] + (v1)[2] * (v2)[2])
#define DOT3_STRIDED(v1, v2, stride) ((v1)[0] * (v2)[0] + (v1)[1] * (v2)[stride] + (v1)[2] * (v2)[2*stride])

namespace Math
{
	//Returns angle in range (-360, 360)
	THORUI_API void NormalizeAngle360(float& a);
	//Returns the shortest distance to two angles in degrees
	THORUI_API float AngleDegDelta(float a1, float a2);
	//Returns a random value between min(inlcuded) and max(included)
	THORUI_API int Rand(int min, int max);
	//Returns a random value 0 and 4,294,967,295 (max unsigned int size)
	THORUI_API unsigned int RandUInt();
	//Returns the result of a linear interpolation between value 1 and value 2. Ratio range [0, 1]
	THORUI_API float Lerp(float v1, float v2, float ratio);
	//Returns the input value clamped between [min, max], both included
	THORUI_API float Clamp(float v, float min, float max);
}

							
#endif __MATH_H__
