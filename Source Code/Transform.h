#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "Mat3x3.h"
#include "Vec2.h"
#include <vector>

class Transform
{
public:
	Transform(void* container);
	Transform(void* container, Vec2 pos, Vec2 scale, float rotation);

	void SetPos(Vec2 pos);
	void SetGlobalPos(Vec2 pos);
	void SetScale(Vec2 scale);
	void SetRotationDeg(float rotation);
	void SetPivot(Vec2 position, bool pivotStays = false);

	void SetGlobalTransform(Mat3x3 mat);
	void UpdateGlobalTransform();

	//Parenting ----
	void SetParent(Transform* parent, bool worldPosStays = true);
	void AddChild(Transform* child);
	void RemoveChild(Transform* child);

	Transform* GetParent() const;
	const std::vector<Transform*>& GetChildren() const;
	//-------------

	Vec2 GetPos() const;
	Vec2 GetScale() const;
	float GetRotation() const;
	Vec2 GetPivot() const;

	const Mat3x3& Local() const;
	const Mat3x3& Global() const;
	const Mat3x3& Center() const;

	//Warning: Skips any security checks, be sure to match the class types
	template<typename C>
	C* Container(){ return static_cast<C*>(container);}

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

	Transform* parent;
	std::vector<Transform*> children;

	void* container = nullptr; //TODO: void* or Transform<T> ?
};
#endif // !__TRANSFORM_H__

