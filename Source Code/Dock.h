#ifndef __DOCK_H__
#define __DOCK_H__

#include <vector>
#include <string>
#include "Vec2.h"

typedef unsigned int uint;

class Dock
{
public:
	Dock(const char* name, Vec2 size = Vec2(800, 300));
	~Dock();

	virtual void Draw();
	virtual void DrawData();

	void AddChild(Dock* dock);
	void RemoveChild(Dock* dock);

	void SetActive(bool active);

	void Close();

private:
	void DrawTabPanels();
	void DrawSingleTab(Dock* dock);

public:
	std::string name = "dock";
	bool root = true;

private:
	Dock* parent = nullptr;
	std::vector<Dock*> childs;

	Vec2 size;

	float separator_position = 200.0f;
	bool button_pressed = false;
	float init_separator_position = 0.0f;

	bool open = true;
	bool active = false;
	uint child_index = 0;
};

#endif