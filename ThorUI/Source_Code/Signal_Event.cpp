#include "Signal_Event.h"
#include "Config.h"

void Signal_Event::SetValueTypes(std::vector<std::string> types)
{
	//If the event was just loaded, we ignore the type change
	//This function will be called always after loading an event
	if (s_event_loaded == true)
	{
		s_event_loaded = false;
		return;
	}

	ClearTypes();
	v_types = types;
	for (uint i = 0; i < v_types.size(); ++i)
	{
		SetValue(0, i);
		v_c_types.push_back(None);
	}
}

int Signal_Event::GetVectorIndex(int v_index)
{
	std::string v_type = v_types[v_index];
	int v_count = -1;
	for (uint i = 0; i < v_types.size() && i <= v_index; ++i)
	{
		if (v_type == v_types[i]) v_count++;
	}
	return v_count;
}

std::string Signal_Event::CompareTypeToString(S_Compare_Type type)
{
	switch (type)
	{
		case(Equal):
		{
			return "==";
		}
		case(Different):
		{
			return "!=";
		}
		case(Smaller):
		{
			return "<";
		}
		case(Greater):
		{
			return ">";
		}
		case(None):
		{
			return "ignore";
		}
	}
}

void Signal_Event::Reset()
{
	ClearSignal();
	apperance_set = -1;
	item_id = 0;
}

void Signal_Event::ClearSignal()
{
	ClearTypes();
	slot_id = -1;
	signal_name = "";
	signal_id = 0;
}

void Signal_Event::ClearTypes()
{
	v_types.clear();
	v_c_types.clear();
	s_values.clear(); i_values.clear(); f_values.clear();
}

void Signal_Event::Save(Config& data)
{
	data.SetNumber("Item_ID", item_id);
	data.SetString("Signal_Name", signal_name.c_str());

	Config_Array v_types_arr = data.SetArray("Value_Types");
	for (uint i = 0; i < v_types.size(); ++i)
		v_types_arr.AddString(v_types[i].c_str());

	Config_Array v_c_types_arr = data.SetArray("Value_Comp_Types");
	for (uint i = 0; i < v_c_types.size(); ++i)
		v_c_types_arr.AddNumber(v_c_types[i]);

	Config_Array s_values_arr = data.SetArray("String_Values");
	for (uint i = 0; i < s_values.size(); ++i)
		s_values_arr.AddString(s_values[i].c_str());

	Config_Array i_values_arr = data.SetArray("Int_Values");
	for (uint i = 0; i < i_values.size(); ++i)
		i_values_arr.AddNumber(i_values[i]);

	Config_Array f_values_arr = data.SetArray("Float_Values");
	for (uint i = 0; i < f_values.size(); ++i)
		f_values_arr.AddNumber(f_values[i]);

	data.SetNumber("Apperance_Set", apperance_set);
	data.SetNumber("Transition_Time", transition_time);
}

void Signal_Event::Load(Config& data)
{
	item_id = data.GetNumber("Item_ID");
	signal_name = data.GetString("Signal_Name");

	Config_Array v_types_arr = data.GetArray("Value_Types");
	for (uint i = 0; i < v_types_arr.GetSize(); ++i)
		v_types.push_back(v_types_arr.GetString(i));

	Config_Array v_c_types_arr = data.GetArray("Value_Comp_Types");
	for (uint i = 0; i < v_c_types_arr.GetSize(); ++i)
		v_c_types.push_back((S_Compare_Type)(int)v_c_types_arr.GetNumber(i));

	Config_Array s_values_arr = data.GetArray("String_Values");
	for (uint i = 0; i < s_values_arr.GetSize(); ++i)
		s_values.push_back(s_values_arr.GetString(i));

	Config_Array i_values_arr = data.GetArray("Int_Values");
	for (uint i = 0; i < i_values_arr.GetSize(); ++i)
		i_values.push_back(i_values_arr.GetNumber(i));

	Config_Array f_values_arr = data.GetArray("Float_Values");
	for (uint i = 0; i < f_values_arr.GetSize(); ++i)
		f_values.push_back(f_values_arr.GetNumber(i));

	apperance_set = data.GetNumber("Apperance_Set");
	transition_time = data.GetNumber("Transition_Time");

	s_event_loaded = true;
}

void Signal_Event::SetLinkError()
{
	link_error = true;
}