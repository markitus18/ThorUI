#ifndef __S_EVENT_H__
#define __S_EVENT_H__

#include <vector>
#include <string>

#ifdef THORUI_EXPORTS
#define THORUI_API __declspec(dllexport) 
#else
#define THORUI_API __declspec(dllimport) 
#endif

typedef unsigned int uint;

enum S_Compare_Type
{
	Equal,
	Different,
	Smaller,
	Greater,
	None,
};

struct Signal_Event
{
	template <typename T>
	bool CompareValue(T v, int v_index)
	{
		if (v_index <= v_c_type.size())
		{
			switch (v_c_type[v_index])
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
				case (Different):
				{
					return v != GetValue<T>(v_index);
				}
			}
		}
		return false;
	}

	THORUI_API void SetValueTypes(std::vector<std::string> types);
	THORUI_API int GetVectorIndex(int v_index);
	THORUI_API static std::string CompareTypeToString(S_Compare_Type type);

	template <typename T>
	T GetValue(int v_index)
	{
		std::string v_type = v_types[v_index];
		int vec_index = GetVectorIndex(v_index);

		if (vec_index >= 0)
		{
			if (v_type == "string") return *(reinterpret_cast<T*>(&s_values[vec_index]));
			if (v_type == "int") return *(reinterpret_cast<T*>(&i_values[vec_index]));
			if (v_type == "float") return *(reinterpret_cast<T*>(&f_values[vec_index]));
		}
	}

	template <typename T>
	void SetValue(T v, int v_index)
	{
		std::string v_type = v_types[v_index];
		int vec_index = GetVectorIndex(v_index);

		if (v_type == "string")
		{
			while (vec_index >= s_values.size())
				s_values.push_back("");
			s_values[vec_index] = v;
		}
		if (v_type == "int")
		{
			while (vec_index >= i_values.size())
				i_values.push_back(0);
			i_values[vec_index] = v;
		}
		if (v_type == "float")
		{
			while (vec_index >= f_values.size())
				f_values.push_back(0);
			f_values[vec_index] = v;
		}
	}

	template <typename... Args>
	bool ProcessArgs(Args... args)
	{
		return CompareArg(0, args...);
	}

	bool CompareArg(uint index)
	{
		return true;
	}

	template <typename T, typename... Args>
	bool CompareArg(uint index, T t, Args... args)
	{
		bool ret = CompareValue(t, index);
		if (ret == false)
			return false;
		return CompareArg(++index, args...);
	}

	//* Just for editor purposes
	uint item_signal_id = 0;
	std::string signal_name;

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
};

#endif