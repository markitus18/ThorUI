#ifndef __SIGNAL_H__
#define __SIGNAL_H__

#include <map>
#include <functional>
#include "TMath.h"

typedef unsigned int uint;

template<typename... Args>
class Signal
{
public:
	Signal() { signal_id = Math::RandUInt(); };
	~Signal() {};

	uint connect(std::function<void(Args...)> func)
	{
		slots.insert(std::pair<uint, std::function<void(Args...) >>(++current_id, func));
		return current_id;
	}

	template <typename C>
	uint connect(C* instance, void(C::*func)(Args...))
	{
		return connect([instance = instance, func = func](Args... arg) { (instance->*(func))(arg...); });
	}

	void Emit(Args... args)
	{
		for (auto it : slots)
			it.second(args...);	
		for (auto it_m : managers)
			it_m.second(signal_id, args...);
	}

	void disconnect(uint id)
	{
		slots.erase(id);
	}

	void disconnect_manager(uint slot_id)
	{
		managers.erase(slot_id);
	}

	uint connect_manager(std::function<void(int, Args...)> func, int slot_id)
	{
		managers.insert(std::pair<uint, std::function<void(int, Args...)>>(slot_id, func));
		return slot_id; //TODO: return T/F

	}

	template <typename C>
	uint connect_manager(C* instance, void(C::*func)(int, Args...), int slot_id)
	{
		return connect_manager([instance = instance, func = func](int v, Args... arg) { (instance->*(func))(v, arg...); }, slot_id);
	}

	uint GetID() { return signal_id; }

private:
	std::map<uint, std::function<void(Args...)>> slots;
	std::map<uint, std::function<void(int, Args...)>> managers;
	uint current_id = 0;
	uint signal_id = 0;
};

#endif