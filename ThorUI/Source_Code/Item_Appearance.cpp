#include "Item_Appearance.h"

#include "ThorUI.h"
#include "Config.h"

void Generic_Ap::SetAllAttributesTrue()
{
	std::unordered_map<std::string, bool>::iterator it;
	for (it = attributes.begin(); it != attributes.end(); ++it)
	{
		(*it).second = true;
	}
}

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

void Image_Ap::Save(Config& config)
{
	if (attributes["color"] == true)
	{
		config.SetArray("color").AddColor(color);
	}
	if (attributes["texture"] == true)
	{
		ThorUI::Texture* tex = ThorUI::GetTexture(texture_id);
		config.SetString("texture", tex != nullptr ? tex->path.c_str() : "");
	}
}

void Image_Ap::Load(Config& config)
{
	if (config.HasValue("color") == true)
	{
		attributes["color"] = true;
		color = config.GetArray("color").GetColor(0);
	}
	if (config.HasValue("texture") == true)
	{
		attributes["texture"] = true;
		texture_id = ThorUI::LoadTexture(config.GetString("texture").c_str());
	}
}

void Button_Ap::Save(Config& config)
{
	if (attributes["color"] == true)
	{
		config.SetArray("color").AddColor(color);
	}
	if (attributes["texture"] == true)
	{
		ThorUI::Texture* tex = ThorUI::GetTexture(texture_id);
		config.SetString("texture", tex != nullptr ? tex->path.c_str() : "");
	}
}

void Button_Ap::Load(Config& config)
{
	if (config.HasValue("color") == true)
	{
		attributes["color"] = true;
		color = config.GetArray("color").GetColor(0);
	}
	if (config.HasValue("texture") == true)
	{
		attributes["texture"] = true;
		texture_id = ThorUI::LoadTexture(config.GetString("texture").c_str());
	}
}

void Text_Ap::Save(Config& config)
{
	if (attributes["color"] == true)
	{
		config.SetArray("color").AddColor(color);
	}
	if (attributes["text"] == true)
	{
		config.SetString("text", text.c_str());
	}
}

void Text_Ap::Load(Config& config)
{
	if (config.HasValue("color") == true)
	{
		attributes["color"] = true;
		color = config.GetArray("color").GetColor(0);
	}
	if (config.HasValue("text") == true)
	{
		attributes["text"] = true;
		text = config.GetString("text");
	}
}

void Panel_Ap::Save(Config& config)
{
	if (attributes["color"] == true)
	{
		config.SetArray("color").AddColor(color);
	}
	if (attributes["border_color"] == true)
	{
		config.SetArray("border_color").AddColor(border_color);
	}
	if (attributes["border_width"] == true)
	{
		config.SetNumber("border_width", border_width);
	}
}

void Panel_Ap::Load(Config& config)
{
	if (config.HasValue("color") == true)
	{
		attributes["color"] = true;
		color = config.GetArray("color").GetColor(0);
	}
	if (config.HasValue("border_color") == true)
	{
		attributes["border_color"] = true;
		border_color = config.GetArray("border_color").GetColor(0);
	}
	if (config.HasValue("border_width") == true)
	{
		attributes["border_width"] = true;
		border_width = config.GetNumber("border_width");
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
	config.SetBool("Button_Ap", button_ap != nullptr);
	if (button_ap != nullptr)
	{
		button_ap->Save(config);
	}
	config.SetBool("Image_Ap", image_ap != nullptr);
	if (image_ap != nullptr)
	{
		image_ap->Save(config);
	}
	config.SetBool("Text_Ap", text_ap != nullptr);
	if (text_ap != nullptr)
	{
		text_ap->Save(config);
	}
	config.SetBool("Panel_Ap", panel_ap != nullptr);
	if (panel_ap != nullptr)
	{
		panel_ap->Save(config);
	}
}

void Appearance_Set::Load(Config& config)
{
	name = config.GetString("Name");
	if (config.GetBool("Item_Ap"))
	{
		item_ap = new Item_Ap();
		item_ap->Load(config);
	}
	if (config.GetBool("Image_Ap"))
	{
		image_ap = new Image_Ap();
		image_ap->Load(config);
	}
	if (config.GetBool("Button_Ap"))
	{
		button_ap = new Button_Ap();
		button_ap->Load(config);
	}
	if (config.GetBool("Text_Ap"))
	{
		text_ap = new Text_Ap();
		text_ap->Load(config);
	}
	if (config.GetBool("Panel_Ap"))
	{
		panel_ap = new Panel_Ap();
		panel_ap->Load(config);
	}
}