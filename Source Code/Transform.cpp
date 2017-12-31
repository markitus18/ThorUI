#include "Transform.h"

Transform::Transform()
{
	local_m.SetIdentity();
	global_m.SetIdentity();
	scale.Set(1, 1);
}

Transform::Transform(Vec2 pos, Vec2 scale, float rotation): pos(pos), scale(scale), rotation(rotation)
{
	local_m.FromTRS(pos, scale, rotation);
}

void Transform::SetPos(Vec2 pos)
{
	this->pos = pos;
	local_m.SetTranslation(pos);
	UpdateGlobalTransform();
}

void Transform::SetScale(Vec2 scale)
{
	UpdateGlobalTransform();
}

void Transform::SetRotation(float rotation)
{
	UpdateGlobalTransform();
}

void Transform::SetGlobalTransform(Mat3x3 mat)
{
	global_m = mat;
	local_m = parent->global_m.Inverted() * mat;
}

void Transform::UpdateGlobalTransform()
{
	global_m = (parent ? parent->global_m : Mat3x3::identity) * local_m;
	for (std::vector<Transform*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->UpdateGlobalTransform();
	}
}

void Transform::SetParent(Transform* parent, bool worldPosStays)
{
	if (this->parent)
		this->parent->RemoveChild(this);

	this->parent = parent;
	parent->AddChild(this);

	worldPosStays ? SetGlobalTransform(global_m) : UpdateGlobalTransform();
}

void Transform::AddChild(Transform* child)
{
	children.push_back(child);
}

void Transform::RemoveChild(Transform* child)
{
	for (std::vector<Transform*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		if ((*it) == child)
		{
			children.erase(it);
			return;
		}
	}
}