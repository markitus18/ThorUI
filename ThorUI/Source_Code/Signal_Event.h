#ifndef __S_EVENT_H__
#define __S_EVENT_H__

#include <vector>
#include <string>

typedef unsigned int uint;

enum S_Compare_Type
{
	None,
	Equal,
	Smaller,
	Greater,
};

union S_Value
{
	int i;
	float f;
	char str;
};

struct Signal_Event
{
	//* The item holding the signal. Just for editor purposes
	uint item_signal_id = 0;

	//* The signal id that triggers the event
	uint signal_id = 0;

	//* Values to be compared with the arguments sent by the signal
	std::vector<std::string> s_values;
	std::vector<int> i_values;
	std::vector<float> f_values;

	//* Values data type
	std::vector<std::string> v_types;

	//* How to compare the values given by the values set
	std::vector<S_Compare_Type> v_c_type;

	//* Event to trigger when all conditions are satisfied
	uint event_id = 0;

	template <typename T>
	bool CompareValue(T v, int v_index)
	{
		if (v_index <= values_size)
		{
			switch (v_c_type)
			{
				case(None):
				{
					return true;
				}
				case(Equal):
				{
					return v == GetValue<T>(v_index);
				}
				case(Smaller):
				{
					return v < GetValue<T>(v_index);
				}
				case(Greater):
				{
					return v > GetValue<T>(v_index);
				}
			}
		}
		return false;
	}

	template <typename T>
	T GetValue(int v_index)
	{
		std::string v_type = v_types[v_index];
		int v_count = 0;
		for (uint i = 0; i < v_types.size() && i <= v_index; ++i)
		{
			if (v_type == v_types[i]) v_count++;
		}
		
		if (v_type == "string") return s_values[v_count];
		if (v_type == "int") return i_values[v_count];
		if (v_type == "float") return f_values[v_count];
	
	}
};

#endif