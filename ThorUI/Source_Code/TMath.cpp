#include "TMath.h"

#include <cstdlib>
#include <time.h>

namespace Math
{
	void NormalizeAngle360(float& a)
	{
		while (a >= 360) a -= 360;
		while (a < 0) a += 360;
	}

	float AngleDegDelta(float a1, float a2)
	{
		NormalizeAngle360(a1);
		NormalizeAngle360(a2);
		float d = a2 - a1;
		while (d > 180) d -= 360;
		while (d < -180) d += 360;
		return d;
	}

	int Rand()
	{
		srand(time(nullptr));
		return rand();
	}

	int Rand(int min, int max)
	{
		srand(time(nullptr));
		return rand() % (max - min + 1) + min;
	}
}