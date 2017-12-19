#include "UI_Item.h"
#include "Config.h"

UI_Item::~UI_Item()
{
	if (parent)
		parent->RemoveChild(this);
	DeleteChildren();
}

void UI_Item::SetPos(float x, float y)
{
	rect.SetPos(Vec2(x, y));
	UpdateGlobalTransform();
}

void UI_Item::SetPos(Vec2 pos)
{
	rect.SetPos(pos);
	UpdateGlobalTransform();
}

void UI_Item::SetGlobalPos(float x, float y)
{
	global_rect.SetPos(Vec2(x, y));
	rect.SetPos((global_rect.pos - (parent ? parent->GetGlobalPos() : Vec2())) / (parent ? parent->GetGlobalScale() : Vec2::one()));
	UpdateGlobalTransform();
}

void UI_Item::SetGlobalPos(Vec2 pos)
{
	global_rect.SetPos(pos);
	rect.SetPos((global_rect.pos - (parent ? parent->GetGlobalPos() : Vec2())) / (parent ? parent->GetGlobalScale() : Vec2::one()));
	UpdateGlobalTransform();
}

void UI_Item::SetSize(float w, float h)
{
	rect.size = Vec2(w, h);
}

void UI_Item::SetSize(Vec2 size)
{
	rect.size = size;
}

void UI_Item::SetID(int id)
{
	this->id = id;
}

void UI_Item::SetScale(float x, float y)
{
	scale.Set(x, y);
	UpdateGlobalTransform();
}

void UI_Item::SetScale(Vec2 scale)
{
	this->scale = scale;
	UpdateGlobalTransform();
}

void UI_Item::SetGlobalScale(Vec2 scale)
{
	global_scale = scale;
	this->scale = global_scale / (parent ? parent->GetGlobalScale() : Vec2(1, 1));
	UpdateGlobalTransform();
}

void UI_Item::SetRotation(float rotation)
{
	rect.SetAngle(rotation);
}

void UI_Item::SetName(const char* name)
{
	this->name = name;
}

void UI_Item::SetPivot(Vec2 pivot)
{
	Vec2 prev_pos = rect.pos - (rect.size * rect.pivot);
	rect.pivot = pivot;
	global_rect.pivot = pivot;

	Vec2 new_pos = prev_pos + (rect.size * rect.pivot);
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
	{
		rect.SetPos(global_rect.pos - (parent ? parent->GetPos() : Vec2()));
		scale = global_scale / (parent ? parent->GetGlobalPos() : Vec2());
	}
	else
		UpdateGlobalTransform();
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

void UI_Item::DeleteChildren()
{
	std::vector<UI_Item*>::iterator it = children.begin();
	while (it != children.end())
	{
		(*it)->DeleteChildren();
		delete (*it);
		it = children.erase(it);
	}
}

void UI_Item::UpdateGlobalTransform()
{
	global_rect.SetPos((parent ? parent->GetGlobalPos() : Vec2()) + rect.pos * (parent ? parent->GetGlobalScale() : Vec2()));
//	global_rect.SetAngle(parent ? parent->GetGlobalRect().angle : 0)
	global_scale = (parent ? parent->GetGlobalScale() : Vec2(1, 1)) * scale;
	global_rect.size = rect.size * global_scale;

	for (std::vector<UI_Item*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->UpdateGlobalTransform();
	}
}

void UI_Item::Save(Config& config)
{
	config.SetString("Name", name.c_str());
	config.SetNumber("Type", (int)type);
	config.SetArray("Position").AddVec2(rect.pos);
	config.SetArray("Size").AddVec2(rect.size);
	config.SetNumber("ID", id);
	config.SetNumber("Parent ID", parent ? parent->GetID() : -1);
	InternalSave(config);
}

void UI_Item::Load(Config& config)
{
	name = config.GetString("Name", "Undefined");
	rect.pos = config.GetArray("Position").GetVec2(0);
	rect.size = config.GetArray("Size").GetVec2(0);
	id = config.GetNumber("ID", -1);

	InternalLoad(config); //TODO: consider if updating pos before or after
	UpdateGlobalTransform();
}

Vec2 UI_Item::GetPos() const
{
	return rect.pos;
}

Vec2 UI_Item::GetGlobalPos() const
{
	return global_rect.pos;
}

Vec2 UI_Item::GetSize() const
{
	return rect.size;
}

Vec2 UI_Item::GetScale() const
{
	return scale;
}

Vec2 UI_Item::GetGlobalScale() const
{
	return global_scale;
}

Rect& UI_Item::GetRect()
{
	return rect;
}

Rect& UI_Item::GetGlobalRect()
{
	return global_rect;
}

Vec2 UI_Item::GetPivot() const
{
	return rect.pivot;
}

int UI_Item::GetID() const
{
	return id;
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

UI_Item* UI_Item::GetChild(uint index) const
{
	if (index >= children.size()) return nullptr;
	return children[index];
}

const std::vector<UI_Item*> UI_Item::GetChildren() const
{
	return children;
}

void UI_Item::CollectAllChildren(std::vector<UI_Item*>& vector) const
{
	for (std::vector<UI_Item*>::const_iterator it = children.begin(); it != children.end(); ++it)
	{
		vector.push_back(*it);
		(*it)->CollectAllChildren(vector);
	}
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
	if (active == false)
		return false;
	return (parent == nullptr ? true : parent->IsParentActive());
}