#include "Mat3x3.h"
#include "TMath.h"

const Mat3x3 Mat3x3::identity = Mat3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);

Mat3x3::Mat3x3(float _00, float _01, float _02,
				float _10, float _11, float _12,
				float _20, float _21, float _22)
{
	Set(_00, _01, _02, _10, _11, _12, _20, _21, _22);
}

void Mat3x3::Set(float _00, float _01, float _02,
	float _10, float _11, float _12,
	float _20, float _21, float _22)
{
	m[0][0] = _00; m[0][1] = _01; m[0][2] = _02;
	m[1][0] = _10; m[1][1] = _11; m[1][2] = _12;
	m[2][0] = _20; m[2][1] = _21; m[2][2] = _22;
	ToOpenGL();
}

void Mat3x3::SetIdentity()
{
	Set(1, 0, 0,
		0, 1, 0,
		0, 0, 1);
	ToOpenGL();
}

const float* Mat3x3::Ptr() const
{
	return &m[0][0];
}

const float* Mat3x3::OpenGLPtr() const
{
	return &gl_m[0][0];
}

void Mat3x3::ToOpenGL()
{
	gl_m[0][0] = m[0][0]; gl_m[0][1] = m[1][0]; gl_m[0][2] = 0;			gl_m[0][3] = 0;
	gl_m[1][0] = m[0][1]; gl_m[1][1] = m[1][1]; gl_m[1][2] = 0;			gl_m[1][3] = 0;
	gl_m[2][0] = 0;		  gl_m[2][1] = 0;		gl_m[2][2] = 1;			gl_m[2][3] = 0;
	gl_m[3][0] = m[0][2]; gl_m[3][1] = m[1][2];	gl_m[3][2] = 0;			gl_m[3][3] = 1;
}

Vec2 Mat3x3::GetTranslation() const
{
	return Vec2(m[0][2], m[1][2]);
}

Vec2 Mat3x3::GetScale() const
{
	return Vec2(Vec2(m[0][0], m[1][0]).Lenght(), Vec2(m[0][1], m[1][1]).Lenght());
}

float Mat3x3::GetRotation() const
{
	Vec2 v(m[0][0], m[1][0]);
	v.Normalize();
	return atan2(v.y, v.x);
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
	res.ToOpenGL();
	return res;
}

void Mat3x3::FromTRS(Vec2 tr, Vec2 scale, float angle)
{
	SetIdentity();

	Translate(tr);
	RotateDeg(angle);
	Scale(scale);
}

void Mat3x3::Translate(Vec2 tr)
{
	float vec[3] = { tr.x, tr.y, 1 };
	m[0][2] = DOT3(m[0], vec);
	m[1][2] = DOT3(m[1], vec);

	ToOpenGL();
}

void Mat3x3::SetTranslation(Vec2 tr)
{
	m[0][2] = tr.x;
	m[1][2] = tr.y;
	ToOpenGL();
}

void Mat3x3::Scale(Vec2 scale)
{
	m[0][0] *= scale.x; m[0][1] *= scale.y;
	m[1][0] *= scale.x; m[1][1] *= scale.y;
	ToOpenGL();
}

void Mat3x3::RotateDeg(float angle)
{
	float r_angle = (GetRotation() + angle * DEGTORAD) ;
	float s = sin(r_angle);
	float c = cos(r_angle);
	Vec2 sc = GetScale();

	m[0][0] = c*sc.x; 	m[0][1] = -s*sc.y;
	m[1][0] = s*sc.x; 	m[1][1] = c*sc.y;
	ToOpenGL();
}

Mat3x3 Mat3x3::Inverted() const
{
	float det = Determinant();
	Mat3x3 ret;

	for (int c = 0; c < 3; ++c)
		for (int r = 0; r < 3; ++r)
			ret[c][r] = (m[(r + 1) % 3][(c + 1) % 3] * m[(r + 2) % 3][(c + 2) % 3] - m[(r + 1) % 3][(c + 2) % 3] * m[(r + 2) % 3][(c + 1) % 3]) / det;

	return ret;
}

float Mat3x3::Determinant() const
{
	float det = 0;
	for (int i = 0; i < 3; ++i)
	{
		det += m[0][i] * (m[1][(i + 1) % 3] * m[2][(i + 2) % 3] - m[1][(i + 2) % 3] * m[2][(i + 1) % 3]);
	}
	return det;
}


