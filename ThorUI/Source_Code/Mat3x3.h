#ifndef __MAT3X3_H__
#define __MAT3X3_H__

#ifdef THORUI_EXPORTS
#define THORUI_API __declspec(dllexport) 
#else
#define THORUI_API __declspec(dllimport) 
#endif

#include "Vec2.h"
#include "MToV.h"

struct Vec2;

class Mat3x3
{
public:

	static const Mat3x3 identity;

	THORUI_API Mat3x3() {};
	THORUI_API Mat3x3(float _00, float _01, float _02,
			float _10, float _11, float _12,
			float _20, float _21, float _22);
	THORUI_API ~Mat3x3() {};

	THORUI_API void Set(float _00, float _01, float _02,
			float _10, float _11, float _12,
			float _20, float _21, float _22);
	THORUI_API void SetIdentity();

	THORUI_API const float* Ptr() const;
	THORUI_API const float* OpenGLPtr() const;

	THORUI_API Vec2 GetTranslation() const;
	THORUI_API Vec2 GetScale() const;
	//Returns rotation in radians
	THORUI_API float GetRotation() const;

	THORUI_API const MToV<4>& operator[] (int row) const;

	THORUI_API Mat3x3 operator*(const Mat3x3& mat) const;

	THORUI_API void FromTRS(Vec2 tr, Vec2 scale, float angle);

	THORUI_API void Translate(Vec2 tr);
	THORUI_API void SetTranslation(Vec2 tr);
	THORUI_API void Scale(Vec2 scale);
	THORUI_API void RotateDeg(float angle);

	THORUI_API Mat3x3 Inverted() const;
	THORUI_API float Determinant() const;

private:
	MToV<4>& operator[] (int row);
	void ToOpenGL();

private:
	float m[3][3];
	float gl_m[4][4]; //TODO: change for a better way
};

#endif // !__MAT3X3_H__

