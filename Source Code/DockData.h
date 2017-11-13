#ifndef __DOCK_DATA_H__
#define __DOCK_DATA_H__

#include <string>

class Dock;
typedef unsigned int uint;

class DockData
{
public:
	DockData();
	~DockData();

	virtual void Draw();

	void SetParent(Dock* dock);
	void SetActive(bool active);

	Dock* GetParent() const;
	bool IsActive() const;



private:

public:
	uint child_index = 0;
	std::string name;

private:
	Dock* parent = nullptr;
	bool active = false;
};

#endif //__DOCK_DATA_H__

