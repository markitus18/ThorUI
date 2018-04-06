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

class Config;

struct Signal_Event
{
	THORUI_API void SetValueTypes(std::vector<std::string> types);
	THORUI_API int GetVectorIndex(int v_index);
	THORUI_API static std::string CompareTypeToString(S_Compare_Type type);

	THORUI_API void Reset();
	THORUI_API void ClearSignal();
	THORUI_API void ClearTypes();
	THORUI_API void Save(Config& data);
	THORUI_API void Load(Config& data);

	//Function to be called when the signal links of this event cannot be constructed
	THORUI_API void SetLinkError();

	template <typename T>
	bool CompareValue(T v, int v_index)
	{
		if (v_index <= v_c_types.size())
		{
			switch (v_c_types[v_index])
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

	//The function is called when all the arguments have been expanded or when there are no arguments at all.
	//In both cases, the parameters are successflully matched
	THORUI_API inline bool CompareArg(uint index) { return true; }

	template <typename T, typename... Args>
	bool CompareArg(uint index, T t, Args... args)
	{
		return CompareValue(t, index) ? CompareArg(++index, args...) : false;
	}

	//* Just for editor purposes
	uint item_id = 0;
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
	std::vector<S_Compare_Type> v_c_types;

	//* Event to trigger when all conditions are satisfied
	uint apperance_set = 0;

	bool s_event_loaded = false;
	bool link_error = false;
};

#endif