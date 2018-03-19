#ifndef __SIGNAL_H__
#define __SIGNAL_H__

#include <map>
#include <functional>

typedef unsigned int uint;

template<typename... Args>
class Signal
{
public:
	Signal() {};
	~Signal() {};

	uint connect(std::function<void(Args...)>func)
	{
		slots[++current_id] = std::function<void(Args...) >> (func);
		return current_id;
	}

	template <typename C>
	uint connect(C* instance, void(C::*func)(Args...))
	{
		return connect([instance = instance, func = func](Args... arg) { (instance->*(func))(arg...); });
	}

	void disconnect(uint id)
	{
		slots.erase(id);
	}

private:
	std::map<uint, std::function<void(Args...)>> slots;
	uint current_id = 0;
};

#endif