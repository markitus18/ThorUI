#ifndef __MAT3X3_H__
#define __MAT3X3_H__

#include "Vec2.h"
#include "MToV.h"

struct Vec2;

class Mat3x3
{
public:

	static const Mat3x3 identity;

	Mat3x3() {};
	Mat3x3(float _00, float _01, float _02,
			float _10, float _11, float _12,
			float _20, float _21, float _22);
	~Mat3x3() {};

	void Set(float _00, float _01, float _02,
			float _10, float _11, float _12,
			float _20, float _21, float _22);
	void SetIdentity();

	const float* Ptr() const;
	const float* OpenGLPtr() const;

	Vec2 GetTranslation() const;
	Vec2 GetScale() const;
	//Returns rotation in radians
	float GetRotation() const;

	const MToV<4>& operator[] (int row) const;

	Mat3x3 operator*(const Mat3x3& mat) const;

	void FromTRS(Vec2 tr, Vec2 scale, float angle);

	void Translate(Vec2 tr);
	void SetTranslation(Vec2 tr);
	void Scale(Vec2 scale);
	void RotateDeg(float angle);

	Mat3x3 Inverted() const;
	float Determinant() const;

private:
	MToV<4>& operator[] (int row);
	void ToOpenGL();

private:
	float m[3][3];
	float gl_m[4][4]; //TODO: change for a better way
};

#endif // !__MAT3X3_H__

