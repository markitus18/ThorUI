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
}

void UI_Item::SetParent(UI_Item* parent, bool keep_global)
{
	if (this->parent != nullptr) this->parent->RemoveChild(this);

	this->parent = parent;
	transform.SetParent(&parent->transform, keep_global);

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

void UI_Item::Save(Config& config)
{
	config.SetString("Name", name.c_str());
	config.SetNumber("Type", (int)type);
	config.SetArray("Position").AddVec2(transform.GetPos());
	config.SetArray("Size").AddVec2(size);
	config.SetNumber("ID", id);
	config.SetNumber("Parent ID", parent ? parent->GetID() : -1);
	InternalSave(config);
}

void UI_Item::Load(Config& config)
{
	name = config.GetString("Name", "Undefined");
	transform.GetPos() = config.GetArray("Position").GetVec2(0);
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

Transform& UI_Item::GetTransform()
{
	return transform;
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