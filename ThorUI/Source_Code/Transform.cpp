#include "Transform.h"
#include <math.h>
#include "TMath.h"

Transform::Transform()
{
	local_m.SetIdentity();
	global_m.SetIdentity();
	scale.Set(1, 1);
}

Transform::Transform(void* container) : container(container)
{
	local_m.SetIdentity();
	global_m.SetIdentity();
	scale.Set(1, 1);
}

Transform::Transform(void* container, Vec2 pos, Vec2 scale, float rotation): container(container), pos(pos), scale(scale), rotation(rotation)
{
	local_m.FromTRS(pos, scale, rotation);
}

void Transform::SetContainer(void* container)
{
	this->container = container;
}

void Transform::SetPos(Vec2 pos)
{
	this->pos = pos;
	local_m.SetTranslation(pos);
	UpdateGlobalTransform();
}

void Transform::SetGlobalPos(Vec2 pos)
{
	global_m.SetTranslation(pos);
	SetGlobalTransform(global_m);
}

void Transform::SetScale(Vec2 scale)
{
	local_m.Scale(scale / this->scale);
	this->scale = scale;
	UpdateGlobalTransform();
}

void Transform::SetRotationDeg(float rotation)
{
	float delta = Math::AngleDegDelta(this->rotation, rotation);
	local_m.RotateDeg(delta);
	this->rotation = rotation;
	UpdateGlobalTransform();
}

void Transform::SetPivot(Vec2 position, bool pivotStays)
{
	Vec2 piv_delta = pivot_offset - position;
	pivot_offset = position;

	if (pivotStays == false)
	{
		local_m.Translate(piv_delta);
		pos = local_m.GetTranslation();
		global_m = (parent ? parent->center_m : Mat3x3::identity) * local_m;
	}

	UpdateGlobalTransform();
}

void Transform::SetGlobalTransform(Mat3x3 mat)
{
	global_m = mat;
	local_m = parent ? parent->center_m.Inverted() * mat : mat;

	pos = local_m.GetTranslation();
	scale = local_m.GetScale();
	rotation = local_m.GetRotation() * RADTODEG;

	UpdateGlobalTransform();
}

void Transform::UpdateGlobalTransform()
{
	global_m = (parent ? parent->center_m : Mat3x3::identity) * local_m;
	
	Mat3x3 pivot_tr = Mat3x3::identity;
	pivot_tr.SetTranslation(pivot_offset);
	center_m = global_m * pivot_tr;

	for (std::vector<Transform*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->UpdateGlobalTransform();
	}
}

void Transform::SetParent(Transform* parent, Transform* next_child, bool worldPosStays)
{
	if (this->parent)
		this->parent->RemoveChild(this);

	this->parent = parent;
	if (parent) parent->AddChild(this, next_child);

	worldPosStays ? SetGlobalTransform(global_m) : UpdateGlobalTransform();
}

void Transform::AddChild(Transform* child, Transform* next_child)
{
	std::vector<Transform*>::iterator it = next_child ? std::find(children.begin(), children.end(), next_child) : children.end();
	children.insert(it, child);
}

Transform* Transform::GetParent() const
{
	return parent;
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

const std::vector<Transform*>& Transform::GetChildren() const
{
	return children;
}

Vec2 Transform::GetPos() const
{
	return pos;
}

Vec2 Transform::GetScale() const
{
	return scale;
}

float Transform::GetRotation() const
{
	return rotation;
}

Vec2 Transform::GetPivot() const
{
	return pivot_offset;
}

const Mat3x3& Transform::Local() const
{
	return local_m;
}

const Mat3x3& Transform::Global() const
{
	return global_m;
}

const Mat3x3& Transform::Center() const
{
	return center_m;
}