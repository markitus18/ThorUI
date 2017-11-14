#ifndef __DOCK_H__
#define __DOCK_H__

#include <vector>
#include <string>
#include "Vec2.h"

typedef unsigned int uint;
class DockData;

struct Separator
{
	float position = 0.5f;
	bool pressed = false;
	float init_position = 0.5f;
};

enum Separation_Type
{
	NONE,
	VERTICAL,
	HORIZONTAL,
};

class Dock
{
public:
	Dock(const char* name, Vec2 size = Vec2(0, 0));
	~Dock();

	void Draw();
	void DrawSeparator();

	void AddChildData(DockData* dock, int position = -1);
	void RemoveChildData(DockData* dock);

	void Split(Separation_Type type, float pos = 0.5f);

	void Close();
	void ClearDockChildren();

	void SetDataActive(DockData* data);
	void CloseDockData(DockData* data);

	void SetSize(Vec2 size);

	std::vector<Dock*>& GetDockChildren();

private:
	void DrawTabPanels();
	void DrawSingleTab(DockData* data);
	bool DoesTabFit(DockData* dock);

	void UpdateChildrenPosition();

public:
	std::string name = "dock";
	bool root = true;

	Vec2 position;
	Vec2 size;

	Separation_Type separation = NONE;
private:
	Dock* parent = nullptr;
	std::vector<DockData*> data_children;
	std::vector<Dock*> dock_children;

	Separator separator;

	bool open = true;
};

#endif