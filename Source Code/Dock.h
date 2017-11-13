#ifndef __DOCK_H__
#define __DOCK_H__

#include <vector>
#include <string>
#include "Vec2.h"

typedef unsigned int uint;
class DockData;

struct Separator
{
	float position = 200.0f;
	bool pressed = false;
	float init_position = 200.0f;
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

	void Split(Separation_Type type);

	void Close();
	void ClearDockChildren();

	void SetDataActive(DockData* data);
	void CloseDockData(DockData* data);

private:
	void DrawTabPanels();
	void DrawSingleTab(DockData* data);
	bool DoesTabFit(DockData* dock);

public:
	std::string name = "dock";
	bool root = true;

	Separation_Type separation = NONE;
private:
	Dock* parent = nullptr;
	std::vector<DockData*> data_children;
	std::vector<Dock*> dock_children;
	Vec2 size;


	Separator separator;

	bool open = true;
};

#endif