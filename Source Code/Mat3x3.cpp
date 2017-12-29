#include "Mat3x3.h"
#include "Math.h"

void Mat3x3::Set(float _00, float _01, float _02,
	float _10, float _11, float _12,
	float _20, float _21, float _22)
{
	v[0][0] = _00; v[0][1] = _01; v[0][2] = _02;
	v[1][0] = _10; v[1][1] = _11; v[1][2] = _12;
	v[2][0] = _20; v[2][1] = _21; v[2][2] = _22;
}

void Mat3x3::SetIdentity()
{
	Set(1, 0, 0,
		0, 1, 0,
		0, 0, 0);
}

float* Mat3x3::Ptr()
{
	return &v[0][0];
}
const float* Mat3x3::Ptr() const
{
	return &v[0][0];
}

MToV<4>& Mat3x3::operator[] (int row)
{
	return *reinterpret_cast<MToV<4>*>(v[row]);
}

const MToV<4>& Mat3x3::operator[] (int row) const
{
	return *reinterpret_cast<const MToV<4>*>(v[row]);
}

Mat3x3 Mat3x3::operator*(const Mat3x3& mat) const
{
	Mat3x3 res;
	res[0][0] = DOT3_STRIDED(v[0], mat.Ptr() + 0, 3);
	res[0][1] = DOT3_STRIDED(v[0], mat.Ptr() + 1, 3);
	res[0][2] = DOT3_STRIDED(v[0], mat.Ptr() + 2, 3);

	res[1][0] = DOT3_STRIDED(v[1], mat.Ptr() + 0, 3);
	res[1][1] = DOT3_STRIDED(v[1], mat.Ptr() + 1, 3);
	res[1][2] = DOT3_STRIDED(v[1], mat.Ptr() + 2, 3);

	res[2][0] = DOT3_STRIDED(v[2], mat.Ptr() + 0, 3);
	res[2][1] = DOT3_STRIDED(v[2], mat.Ptr() + 1, 3);
	res[2][2] = DOT3_STRIDED(v[2], mat.Ptr() + 2, 3);

	return res;
}

void Mat3x3::Translate(Vec2 tr)
{
	float vec[3] = { tr.x, tr.y, 1 };

	v[0][2] = DOT3(v[0], vec);
	v[1][2] = DOT3(v[1], vec);
}

void Mat3x3::Scale(Vec2 scale)
{
	v[0][0] *= scale.x; v[0][1] *= scale.y;
	v[1][0] *= scale.x; v[1][1] *= scale.y;
}
