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
	void SetGlobalPos(Vec2 pos);
	void SetScale(Vec2 scale);
	void SetRotationDeg(float rotation);
	void SetPivot(Vec2 position, bool pivotStays = false);

	void SetGlobalTransform(Mat3x3 mat);
	void UpdateGlobalTransform();

	void SetParent(Transform* parent, bool worldPosStays = true);
	void AddChild(Transform* child);
	void RemoveChild(Transform* child);

	Vec2 GetPos() const;
	Vec2 GetScale() const;
	float GetRotation() const;
	Vec2 GetPivot() const;

	const Mat3x3& Local() const;
	const Mat3x3& Global() const;
	const Mat3x3& Center() const;

private:
	//Local position, rotation and scale
	Vec2 pos;
	Vec2 scale;
	float rotation = 0;
	Vec2 pivot_offset;

	//Resulting matrix from pos, scale and rotation
	Mat3x3 local_m;
	//Transformation of the pivot in global_space
	Mat3x3 global_m;
	//Transformation of the center of the item in global_space
	Mat3x3 center_m;

	Transform* parent; //TODO: clean transform-item parenting
	std::vector<Transform*> children;
};
#endif // !__TRANSFORM_H__

