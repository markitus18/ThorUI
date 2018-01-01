#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "Mat3x3.h"
#include "Vec2.h"
#include <vector>

class Transform
{
public:
	Transform();
	Transform(Vec2 pos, Vec2 scale, float rotation);

	void SetPos(Vec2 pos);
	void SetScale(Vec2 scale);
	void SetRotationDeg(float rotation);

	void SetGlobalTransform(Mat3x3 mat);
	void UpdateGlobalTransform();

	void SetParent(Transform* parent, bool worldPosStays = true);
	void AddChild(Transform* child);
	void RemoveChild(Transform* child);

public: //TODO: switch to private

	//Local position, rotation and scale
	Vec2 pos;
	Vec2 scale;
	float rotation = 0;

	//Resulting matrix from pos, scale and rotation
	Mat3x3 local_m;
	Mat3x3 global_m;

	Transform* parent;
	std::vector<Transform*> children;
};
#endif // !__TRANSFORM_H__

