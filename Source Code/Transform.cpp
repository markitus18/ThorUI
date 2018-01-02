#include "Transform.h"
#include "Math.h"

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

void Transform::SetGlobalPos(Vec2 pos)
{
	global_m.SetTranslation(pos);
	SetGlobalTransform(global_m);
}

void Transform::SetScale(Vec2 scale)
{
	this->scale = scale;
	local_m.FromTRS(pos, scale, rotation); //TODO: only change scale and rotation part
	UpdateGlobalTransform();
}

void Transform::SetRotationDeg(float rotation)
{
	this->rotation = rotation;
	local_m.FromTRS(pos, scale, rotation);  //TODO: only change scale and rotation part
	UpdateGlobalTransform();
}

void Transform::SetPivot(Vec2 position, bool pivotStays)
{
	Vec2 piv_delta = pivot_offset - position;

	if (pivotStays == false)
	{
		local_m.Translate(piv_delta);
		pos = local_m.GetTranslation();
		scale = local_m.GetScale();
		rotation = local_m.GetRotation() * RADTODEG;

	}
	UpdateGlobalTransform();
	pivot_offset = position;

	Mat3x3 pivot_tr = Mat3x3::identity;
	pivot_tr.SetTranslation(pivot_offset);
	center_m = global_m * pivot_tr;
	UpdateGlobalTransform();
}

void Transform::SetGlobalTransform(Mat3x3 mat)
{
	global_m = mat;
	local_m = parent->center_m.Inverted() * mat;

	pos = local_m.GetTranslation();
	scale = local_m.GetScale();
	rotation = local_m.GetRotation() * RADTODEG;

	Mat3x3 pivot_tr = Mat3x3::identity;
	pivot_tr.SetTranslation(pivot_offset);
	center_m = global_m * pivot_tr;

	for (std::vector<Transform*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->UpdateGlobalTransform();
	}
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

Vec2 Transform::GetPivot() const
{
	return pivot_offset;
}