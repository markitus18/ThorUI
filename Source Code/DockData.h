#ifndef __DOCK_DATA_H__
#define __DOCK_DATA_H__

#include <string>

class Dock;
typedef unsigned int uint;
class UI_Editor;

class DockData
{
public:
	DockData(UI_Editor* editor);
	~DockData();

	virtual void Draw() = 0;

	void SetParent(Dock* dock);
	void SetActive(bool active);

	Dock* GetParent() const;
	bool IsActive() const;

private:

public:
	uint child_index = 0;
	std::string name;
	UI_Editor* editor;

protected:
	Dock* parent = nullptr;
	bool active = false;
};

#endif //__DOCK_DATA_H__

