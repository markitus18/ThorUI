#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "Mat3x3.h"
#include "Vec2.h"
#include <vector>

class Transform
{
public:
	THORUI_API Transform(void* container);
	THORUI_API Transform(void* container, Vec2 pos, Vec2 scale, float rotation);

	THORUI_API void SetPos(Vec2 pos);
	THORUI_API void SetGlobalPos(Vec2 pos);
	THORUI_API void SetScale(Vec2 scale);
	THORUI_API void SetRotationDeg(float rotation);
	THORUI_API void SetPivot(Vec2 position, bool pivotStays = false);

	THORUI_API void SetGlobalTransform(Mat3x3 mat);
	THORUI_API void UpdateGlobalTransform();

	//Parenting ----
	THORUI_API void SetParent(Transform* parent, Transform* next_child = nullptr, bool worldPosStays = true);
	THORUI_API void AddChild(Transform* child, Transform* next_child);
	THORUI_API void RemoveChild(Transform* child);

	THORUI_API Transform* GetParent() const;
	THORUI_API const std::vector<Transform*>& GetChildren() const;
	//-------------

	THORUI_API Vec2 GetPos() const;
	THORUI_API Vec2 GetScale() const;
	//Returns rotation in degrees
	THORUI_API float GetRotation() const;
	THORUI_API Vec2 GetPivot() const;

	THORUI_API const Mat3x3& Local() const;
	THORUI_API const Mat3x3& Global() const;
	THORUI_API const Mat3x3& Center() const;

	//Warning: Skips any security checks, be sure to match the class types
	template<typename C>
	C* Container() { return static_cast<C*>(container); }

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

