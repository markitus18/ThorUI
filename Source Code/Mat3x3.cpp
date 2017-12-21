#include "Mat3x3.h"

void Mat3x3::Set(float _00, float _01, float _02,
	float _10, float _11, float _12,
	float _20, float _21, float _22)
{

}

void Mat3x3::SetIdentity()
{
	Set(1, 0, 0,
		0, 1, 0,
		0, 0, 0);
}

float* Mat3x3::operator[] (int row)
{
	return *v + (row * 3);
}

Mat3x3 Mat3x3::operator*(const Mat3x3& mat) const
{
	float _00 = v[0][0] * mat[0][0] + v[0][1] * mat[1][0] + v[0][2] * mat[2][0];
}
