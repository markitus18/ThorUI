#include "Mat3x3.h"
#include "Math.h"

void Mat3x3::Set(float _00, float _01, float _02,
	float _10, float _11, float _12,
	float _20, float _21, float _22)
{
	m[0][0] = _00; m[0][1] = _01; m[0][2] = _02;
	m[1][0] = _10; m[1][1] = _11; m[1][2] = _12;
	m[2][0] = _20; m[2][1] = _21; m[2][2] = _22;
}

void Mat3x3::SetIdentity()
{
	Set(1, 0, 0,
		0, 1, 0,
		0, 0, 1);
}

float* Mat3x3::Ptr()
{
	return &m[0][0];
}
const float* Mat3x3::Ptr() const
{
	return &m[0][0];
}

const float* Mat3x3::ToOpenGL()
{
	gl_m[0][0] = m[0][0]; gl_m[0][1] = m[1][0]; gl_m[0][2] = 0;			gl_m[0][3] = 0;
	gl_m[1][0] = m[0][1]; gl_m[1][1] = m[1][1]; gl_m[1][2] = 0;			gl_m[1][3] = 0;
	gl_m[2][0] = 0;		  gl_m[2][1] = 0;		gl_m[2][2] = 1;			gl_m[2][3] = 0;
	gl_m[3][0] = m[0][2]; gl_m[3][1] = m[1][2];	gl_m[3][2] = 0;			gl_m[3][3] = 1;

	return &gl_m[0][0];
}

MToV<4>& Mat3x3::operator[] (int row)
{
	return *reinterpret_cast<MToV<4>*>(m[row]);
}

const MToV<4>& Mat3x3::operator[] (int row) const
{
	return *reinterpret_cast<const MToV<4>*>(m[row]);
}

Mat3x3 Mat3x3::operator*(const Mat3x3& mat) const
{
	Mat3x3 res;
	res[0][0] = DOT3_STRIDED(m[0], mat.Ptr() + 0, 3);
	res[0][1] = DOT3_STRIDED(m[0], mat.Ptr() + 1, 3);
	res[0][2] = DOT3_STRIDED(m[0], mat.Ptr() + 2, 3);

	res[1][0] = DOT3_STRIDED(m[1], mat.Ptr() + 0, 3);
	res[1][1] = DOT3_STRIDED(m[1], mat.Ptr() + 1, 3);
	res[1][2] = DOT3_STRIDED(m[1], mat.Ptr() + 2, 3);

	res[2][0] = DOT3_STRIDED(m[2], mat.Ptr() + 0, 3);
	res[2][1] = DOT3_STRIDED(m[2], mat.Ptr() + 1, 3);
	res[2][2] = DOT3_STRIDED(m[2], mat.Ptr() + 2, 3);

	return res;
}

void Mat3x3::Translate(Vec2 tr)
{
	float vec[3] = { tr.x, tr.y, 1 };

	m[0][2] = DOT3(m[0], vec);
	m[1][2] = DOT3(m[1], vec);
}

void Mat3x3::Scale(Vec2 scale)
{
	m[0][0] *= scale.x; m[0][1] *= scale.y;
	m[1][0] *= scale.x; m[1][1] *= scale.y;
}
