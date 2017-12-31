#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "Mat3x3.h"
#include "Vec2.h"

class Transform
{
public:
	Transform() { matrix.SetIdentity(); scale.Set(1, 1); }
	Transform(Vec2 pos, Vec2 scale, float rotation);

	void SetPos(Vec2 pos);
	void SetScale(Vec2 scale);
	void SetRotation(float rotation);

public: //TODO: switch to private
	Vec2 pos;
	Vec2 scale;
	float rotation = 0;

	Mat3x3 matrix;
};
#endif // !__TRANSFORM_H__

