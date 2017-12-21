#ifndef __MAT3X3_H__
#define __MAT3X3_H__

#include "Vec2.h"

class Mat3x3
{
public:
	Mat3x3() {};

	~Mat3x3() {};

	void Set(float _00, float _01, float _02,
			float _10, float _11, float _12,
			float _20, float _21, float _22);
	void SetIdentity();

	float* operator[] (int row);
	Mat3x3 operator*(const Mat3x3& mat) const;

private:
	float v[3][3];
};

#endif // !__MAT3X3_H__

