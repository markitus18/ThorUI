#include "UI_Item.h"
#include "Config.h"

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

void UI_Item::SetSize(Vec2 size)
{
	this->size = size;
}

void UI_Item::SetName(const char* name)
{
	this->name = name;
}

void UI_Item::SetPivot(Vec2 pivot)
{
	Vec2 prev_pos = pos - (size * this->pivot);
	this->pivot = pivot;

	Vec2 new_pos = prev_pos + (size * this->pivot);
	SetPos(new_pos);
}

void UI_Item::SetActive(bool active)
{
	this->active = active;
}

void UI_Item::SetParent(UI_Item* parent, bool keep_global)
{
	if (this->parent != nullptr) this->parent->RemoveChild(this);

	this->parent = parent;
	parent->children.push_back(this);

	//Keeping global pos coordinates
	if (keep_global)
		pos = global_pos - (parent ? parent->GetPos() : Vec2());
	else
		UpdateGlobalPos();
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

void UI_Item::Save(Config& config)
{
	config.SetString("Name", name.c_str());
	config.SetNumber("Type", (int)type);
	config.SetArray("Position").AddVec2(pos);
	config.SetArray("Size").AddVec2(size);
}

void UI_Item::Load(Config& config)
{
	name = config.GetString("Name", "Undefined");
	pos = config.GetArray("Position").GetVec2(0);
	size = config.GetArray("Size").GetVec2(0);
	UpdateGlobalPos();
}

Vec2 UI_Item::GetPos() const
{
	return pos;
}

Vec2 UI_Item::GetSize() const
{
	return size;
}

Vec2 UI_Item::GetPivot() const
{
	return pivot;
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

bool UI_Item::IsActive() const
{
	return active;
}

bool UI_Item::IsParentActive() const
{
	if (parent == nullptr)
		return true;
	if (active == false || parent->IsActive() == false)
		return false;
	return parent->IsParentActive();
}