#include "UI_Item.h"
#include "Config.h"
#include "TMath.h"

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
	RemoveFromParent();
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

void UI_Item::SetParent(UI_Item* parent, UI_Item* next_child, bool keep_global)
{
	if (parent != nullptr)
		transform.SetParent(parent->GetTransform(), next_child ? next_child->GetTransform() : nullptr, keep_global);
	else
		RemoveFromParent();
}

void UI_Item::RemoveFromParent()
{
	transform.SetParent(nullptr);
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

void UI_Item::Update(float dt)
{
	if (transitioning == true)
	{
		UpdateApSetTransition(dt);
	}
}

void UI_Item::InternalOnItemEvent(Item_Event event)
{
	last_event = event;
	switch (event)
	{
		case(Mouse_Enter):
		{
			s_hovered.Emit();
			break;
		}
		case(Mouse_Exit):
		{
			s_unhovered.Emit();
			break;
		}
	}
	OnItemEvent(event);
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

void UI_Item::InternalSave(Config& config)
{
	config.SetString("Name", name.c_str());
	config.SetNumber("Item_Type", (int)type);

	config.SetArray("Position").AddVec2(transform.GetPos());
	config.SetArray("Scale").AddVec2(transform.GetScale());
	config.SetNumber("Rotation", transform.GetRotation());
	config.SetArray("Size").AddVec2(size);
	config.SetArray("Pivot").AddVec2(pivot);

	config.SetNumber("ID", id);
	config.SetNumber("Parent ID", transform.GetParent() ? transform.GetParent()->Container<UI_Item>()->GetID() : -1);

	Save(config);
	Config_Array ev_data = config.SetArray("Signal_Events");
	for (uint i = 0; i < s_events.size(); ++i)
	{
		s_events[i].Save(ev_data.AddNode());
	}

	Config_Array ap_sets_data = config.SetArray("Ap_Sets");
	for (uint i = 0; i < appearance_sets.size(); ++i)
	{
		appearance_sets[i].Save(ap_sets_data.AddNode());
	}
}

void UI_Item::InternalLoad(Config& config)
{
	name = config.GetString("Name", "Undefined");

	transform.SetPos(config.GetArray("Position").GetVec2(0));
	transform.SetScale(config.GetArray("Scale").GetVec2(0));
	transform.SetRotationDeg(config.GetNumber("Rotation"));
	size = config.GetArray("Size").GetVec2(0);
	SetPivot(config.GetArray("Pivot").GetVec2(0));

	id = config.GetNumber("ID", -1);

	Config_Array ev_arr = config.GetArray("Signal_Events");
	for (uint i = 0; i < ev_arr.GetSize(); ++i)
	{
		s_events.push_back(Signal_Event());
		s_events[i].Load(ev_arr.GetNode(i));
	}

	Config_Array ap_sets_arr = config.GetArray("Ap_Sets");
	for (uint i = 0; i < ap_sets_arr.GetSize(); ++i)
	{
		appearance_sets.push_back(Appearance_Set());
		appearance_sets[i].Load(ap_sets_arr.GetNode(i));
	}

	Load(config); //TODO: consider if updating pos before or after
	transform.UpdateGlobalTransform();
}

bool UI_Item::ConnectItemWithSignal(UI_Item* item, std::string signal_name, Signal_Event& s_ev)
{
	if (signal_name == "hovered")
	{
		s_ev.slot_id = s_hovered.connect_manager<UI_Item>(item, &UI_Item::SignalManager, item->GetID());
		s_ev.signal_id = s_hovered.GetID();
		s_ev.signal_name = signal_name;
		s_ev.SetValueTypes(std::vector<std::string>());
		return true;
	}
	if (signal_name == "unhovered")
	{
		s_ev.slot_id = s_unhovered.connect_manager<UI_Item>(item, &UI_Item::SignalManager, item->GetID());
		s_ev.signal_id = s_unhovered.GetID();
		s_ev.signal_name = signal_name;
		s_ev.SetValueTypes(std::vector<std::string>());
		return true;
	}
	return false;
}

bool UI_Item::DisconnectItemWithSignal(UI_Item* item, Signal_Event& s_ev)
{
	if (s_ev.signal_name == "hovered")
	{
		s_hovered.disconnect_manager(s_ev.slot_id);
		s_ev.ClearSignal();
		return true;
	}
	if (s_ev.signal_name == "unhovered")
	{
		s_unhovered.disconnect_manager(s_ev.slot_id);
		s_ev.ClearSignal();
		return true;
	}
	return false;
}

std::vector<std::string> UI_Item::GetSignalsStr()
{
	std::vector<std::string> ret;
	ret.push_back("hovered");
	ret.push_back("unhovered");
	return ret;
}

void UI_Item::AddApSet()
{
	std::string name = "Apperance Set ";
	name += std::to_string(appearance_sets.size());
	Appearance_Set set;
	set.name = name;
	appearance_sets.push_back(set);
}

void UI_Item::SaveDefaultApSet()
{
	if (appearance_sets.size() == 0)
		AddApSet();
	appearance_sets[0] = SaveCurrentApState();
	appearance_sets[0].name = "Default";
}

void UI_Item::SetAppearanceSet(uint index, float trans_time)
{
	if (index < appearance_sets.size())
	{
		previous_set = SaveCurrentApState();
		current_set_index = index;
		transition_max_timer = trans_time;
		transition_timer = 0.0f;
		transitioning = true;
	}
}

Appearance_Set UI_Item::SaveCurrentApState()
{
	Appearance_Set set;
	set.item_ap = new Item_Ap();
	set.item_ap->SetAllAttributesTrue();

	set.item_ap->transform = transform;
	set.item_ap->pivot = pivot;
	set.item_ap->size = size;

	return set;
}

void UI_Item::UpdateApSetTransition(float dt)
{
	transition_timer += dt;
	float lerpRatio = transition_timer / transition_max_timer;

	Appearance_Set& set = appearance_sets[current_set_index];
	if (set.item_ap != nullptr)
	{
		Transform& p_tr = previous_set.item_ap->transform;
		Transform& n_tr = set.item_ap->transform;
		if (set.item_ap->attributes["position"] == true)
			transform.SetPos(Vec2::Lerp(p_tr.GetPos(), n_tr.GetPos(), lerpRatio));
		if (set.item_ap->attributes["scale"] == true)
			transform.SetScale(Vec2::Lerp(p_tr.GetScale(), n_tr.GetScale(), lerpRatio));
		if (set.item_ap->attributes["angle"] == true)
			transform.SetRotationDeg(Math::Lerp(p_tr.GetRotation(), n_tr.GetRotation(), lerpRatio));
		if (set.item_ap->attributes["size"] == true)
			SetSize(Vec2::Lerp(previous_set.item_ap->size, set.item_ap->size, lerpRatio));
		if (set.item_ap->attributes["pivot"] == true)
			SetPivot(set.item_ap->pivot);
	}

	if (transition_timer >= transition_max_timer) transitioning = false;
}