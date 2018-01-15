#include "UI_Item.h"
#include "Config.h"

UI_Item::UI_Item() : transform(this)
{
	SetPivot(Vec2(0.5, 0.5));
}

UI_Item::UI_Item(float x, float y) : transform(this)
{
	transform.SetPos(Vec2(x, y));
	SetPivot(Vec2(0.5, 0.5));
}

UI_Item::UI_Item(Vec2 pos, Vec2 size) : transform(this), size(size)
{
	transform.SetPos(pos);
	SetPivot(Vec2(0.5, 0.5));
}

UI_Item::~UI_Item()
{
	//TODO: remove from transform parent. SetParent calls non-necessary functions
	transform.SetParent(nullptr);
	std::vector<Transform*> children = transform.GetChildren();
	while (!children.empty())
	{
		delete children[0]->Container<UI_Item>();
		children.erase(children.begin());
	}
}

void UI_Item::SetPos(float x, float y)
{
	transform.SetPos(Vec2(x, y));
}

void UI_Item::SetPos(Vec2 pos)
{
	transform.SetPos(pos);
}

void UI_Item::SetSize(float w, float h)
{
	size.Set(w, h);
	transform.SetPivot((Vec2(0.5f, 0.5f) - pivot) * size, true);
}

void UI_Item::SetSize(Vec2 size)
{
	this->size = size;
	transform.SetPivot((Vec2(0.5f, 0.5f) - pivot) * size, true);
}

void UI_Item::SetID(int id)
{
	this->id = id;
}

void UI_Item::SetScale(float x, float y)
{
	transform.SetScale(Vec2(x, y));
}

void UI_Item::SetScale(Vec2 scale)
{
	transform.SetScale(scale);
}

void UI_Item::SetRotation(float rotation)
{
	transform.SetRotationDeg(rotation);
}

void UI_Item::SetPivot(Vec2 pivot)
{
	this->pivot = pivot;
	transform.SetPivot((Vec2(0.5f, 0.5f) - pivot) * size);
}

void UI_Item::SetName(const char* name)
{
	this->name = name;
}

void UI_Item::SetActive(bool active)
{
	this->active = active;
	if (hierarchyActive == true)
	{
		SetHierarchyActive(true);
	}
}

void UI_Item::SetParent(UI_Item* parent, bool keep_global)
{
	transform.SetParent(parent->GetTransform(), keep_global);
}

void UI_Item::RemoveChild(UI_Item* child)
{
	transform.RemoveChild(child->GetTransform());
}

void UI_Item::DeleteChildren()
{
	std::vector<Transform*> children = transform.GetChildren();
	while (children.size() != 0)
	{
		delete children[0]->Container<UI_Item>();
		children.erase(children.begin());
	}
}

UI_Item* UI_Item::GetParent() const
{
	return transform.GetParent() ? transform.GetParent()->Container<UI_Item>() : nullptr;
}

UI_Item* UI_Item::GetChild(uint index) const
{
	if (index < 0 || index >= transform.GetChildren().size())
		return nullptr;
	return transform.GetChildren()[index]->Container<UI_Item>();
}

uint UI_Item::ChildCount() const
{
	return transform.GetChildren().size();
}

void UI_Item::CollectChildren(std::vector<UI_Item*>& vector)
{
	std::vector<Transform*> children = transform.GetChildren();
	for (uint i = 0; i < children.size(); ++i)
	{
		vector.push_back(children[i]->Container<UI_Item>());
		children[i]->Container<UI_Item>()->CollectChildren(vector);
	}
}

void UI_Item::SetHierarchyActive(bool active)
{
	hierarchyActive = active;
	std::vector<Transform*> children = transform.GetChildren();
	for (std::vector<Transform*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->Container<UI_Item>()->SetHierarchyActive(this->active && hierarchyActive);
	}
}

void UI_Item::Save(Config& config)
{
	config.SetString("Name", name.c_str());
	config.SetNumber("Type", (int)type);
	config.SetArray("Position").AddVec2(transform.GetPos());
	config.SetArray("Size").AddVec2(size);
	config.SetNumber("ID", id);
	config.SetNumber("Parent ID", transform.GetParent() ? transform.GetParent()->Container<UI_Item>()->GetID() : -1);
	InternalSave(config);
}

void UI_Item::Load(Config& config)
{
	name = config.GetString("Name", "Undefined");
	transform.SetPos(config.GetArray("Position").GetVec2(0));
	size = config.GetArray("Size").GetVec2(0);
	id = config.GetNumber("ID", -1);

	InternalLoad(config); //TODO: consider if updating pos before or after
	transform.UpdateGlobalTransform();
}

Vec2 UI_Item::GetPos() const
{
	return transform.Local().GetTranslation();
}

Vec2 UI_Item::GetGlobalPos() const
{
	return transform.Global().GetTranslation();
}

Vec2 UI_Item::GetSize() const
{
	return size;
}

Vec2 UI_Item::GetScale() const
{
	return transform.GetScale();
}

Vec2 UI_Item::GetPivot() const
{
	return pivot;
}

Transform* UI_Item::GetTransform()
{
	return &transform;
}

int UI_Item::GetID() const
{
	return id;
}

Item_Event UI_Item::GetLastEvent() const
{
	return last_event;
}

const char* UI_Item::GetName() const
{
	return name.c_str();
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
	return active && hierarchyActive; //TODO: change function name
}