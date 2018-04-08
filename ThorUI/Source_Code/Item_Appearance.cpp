#include "Item_Appearance.h"

#include "Config.h"

void Item_Ap::Save(Config& config)
{
	if (attributes["position"] == true)
	{
		config.SetArray("position").AddVec2(transform.GetPos());
	}
	if (attributes["scale"] == true)
	{
		config.SetArray("scale").AddVec2(transform.GetScale());
	}
	if (attributes["angle"] == true)
	{
		config.SetNumber("angle", transform.GetRotation());
	}
	if (attributes["size"] == true)
	{
		config.SetArray("size").AddVec2(size);
	}
	if (attributes["pivot"] == true)
	{
		config.SetArray("pivot").AddVec2(pivot);
	}
}

void Item_Ap::Load(Config& config)
{
	if (config.HasValue("position") == true)
	{
		attributes["position"] = true;
		transform.SetPos(config.GetArray("position").GetVec2(0));
	}

	if (config.HasValue("scale") == true)
	{
		attributes["scale"] = true;
		transform.SetScale(config.GetArray("scale").GetVec2(0));
	}

	if (config.HasValue("angle") == true)
	{
		attributes["angle"] = true;
		transform.SetRotationDeg(config.GetNumber("angle"));
	}

	if (config.HasValue("size") == true)
	{
		attributes["size"] = true;
		size = config.GetArray("size").GetVec2(0);
	}

	if (config.HasValue("pivot") == true)
	{
		attributes["pivot"] = true;
		size = config.GetArray("pivot").GetVec2(0);
	}
}

void Appearance_Set::Save(Config& config)
{
	config.SetString("Name", name.c_str());
	config.SetBool("Item_Ap", item_ap != nullptr);
	if (item_ap != nullptr)
	{
		item_ap->Save(config);
	}
}

void Appearance_Set::Load(Config& config)
{
	name = config.GetString("Name");
	bool item_b = config.GetBool("Item_Ap");
	if (item_b)
	{
		item_ap = new Item_Ap();
		item_ap->Load(config);
	}
}