#include "UI_Item.h"

void UI_Item::SetPos(float x, float y)
{
	pos = Vec2(x, y);
	UpdateGlobalPos();
}

void UI_Item::SetPos(Vec2 pos)
{
	this->pos = pos;
	UpdateGlobalPos();
}

void UI_Item::SetSize(float w, float h)
{
	size = Vec2(w, h);
}

void UI_Item::SetName(const char* name)
{
	this->name = name;
}

void UI_Item::SetParent(UI_Item* parent)
{
	if (this->parent != nullptr) this->parent->RemoveChild(this);

	this->parent = parent;
	parent->children.push_back(this);

}

void UI_Item::RemoveChild(UI_Item* child)
{
	for (std::vector<UI_Item*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		if ((*it) == child)
		{
			children.erase(it);
			return;
		}
	}
}

void UI_Item::UpdateGlobalPos()
{
	global_pos = (parent ? parent->GetPos() : Vec2()) + pos;

	//TODO: iterative method for full optimization?
	for (std::vector<UI_Item*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->UpdateGlobalPos();
	}
}

Vec2 UI_Item::GetPos() const
{
	return pos;
}

Vec2 UI_Item::GetSize() const
{
	return size;
}

Item_Event UI_Item::GetLastEvent() const
{
	return last_event;
}

UI_Item* UI_Item::GetParent() const
{
	return parent;
}

const char* UI_Item::GetName() const
{
	return name.c_str();
}

uint UI_Item::GetChildCount() const
{
	return children.size();
}

const std::vector<UI_Item*> UI_Item::GetChildren() const
{
	return children;
}

Item_Type UI_Item::GetType() const
{
	return type;
}