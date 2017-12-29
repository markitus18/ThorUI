#ifndef __MAT3X3_H__
#define __MAT3X3_H__

#include "Vec2.h"
#include "MToV.h"

struct Vec2;

class Mat3x3
{
public:
	Mat3x3() {};

	~Mat3x3() {};

	void Set(float _00, float _01, float _02,
			float _10, float _11, float _12,
			float _20, float _21, float _22);
	void SetIdentity();

	float* Ptr();
	const float* Ptr() const;

	MToV<4>& operator[] (int row);
	const MToV<4>& operator[] (int row) const;

	Mat3x3 operator*(const Mat3x3& mat) const;

	void Translate(Vec2 tr);
	void Scale(Vec2 scale);

private:
	float v[3][3];
};

#endif // !__MAT3X3_H__

