#include "Signal_Event.h"

void Signal_Event::SetValueTypes(std::vector<std::string> types)
{
	v_types = types;
	s_values.clear(); i_values.clear(); f_values.clear();
	for (uint i = 0; i < v_types.size(); ++i)
	{
		SetValue(0, i);
		v_c_type.push_back(None);
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