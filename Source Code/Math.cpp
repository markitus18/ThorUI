#include "Math.h"

namespace Math
{
	void NormalizeAngle(float& a)
	{
		while (a >= 360) a -= 360;
		while (a <= -360) a += 360;
	}

	float AngleDegDelta(float a1, float a2)
	{
		float d = a1 - a2;
		NormalizeAngle(d);
		return d < -180 || d > 180 ? a1 - a2 : a2 - a1;
	}
}