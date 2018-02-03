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
	if (parent != nullptr)
		transform.SetParent(parent->GetTransform(), keep_global);
}

void UI_Item::RemoveChild(UI_Item* child)
{
	if (child != nullptr)
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

UI_Item* UI_Item::GetChild(uint index) const
{
	if (index < 0 || index >= transform.GetChildren().size())
		return nullptr;
	return transform.GetChildren()[index]->Container<UI_Item>();
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