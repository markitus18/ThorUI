#include "Transform.h"

Transform::Transform(Vec2 pos, Vec2 scale, float rotation): pos(pos), scale(scale), rotation(rotation)
{
	matrix.FromTRS(pos, scale, rotation);
}

void Transform::SetPos(Vec2 pos)
{
	this->pos = pos;
	matrix.SetTranslation(pos);
}

void Transform::SetScale(Vec2 scale)
{

}

void Transform::SetRotation(float rotation)
{

}