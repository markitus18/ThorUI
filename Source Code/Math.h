#ifndef __MATH_H__
#define __MATH_H__

#define PI 3.14159265
#define DEGTORAD PI / 180
#define RADTODEG 180 / PI

#define DOT3(v1, v2) ((v1)[0] * (v2)[0] + (v1)[1] * (v2)[1] + (v1)[2] * (v2)[2])
#define DOT3_STRIDED(v1, v2, stride) ((v1)[0] * (v2)[0] + (v1)[1] * (v2)[stride] + (v1)[2] * (v2)[2*stride])

namespace Math
{
	//Returns angle in range (-360, 360)
	void NormalizeAngle360(float& a);
	//Returns the shortest distance to two angles in degrees
	float AngleDegDelta(float a1, float a2);
}

							
#endif __MATH_H__
