#ifndef __HIERARCHY_D_H__
#define __HIERARCHY_D_H__

#include "DockData.h"
#include "TreeDisplay.h"

class UI_Item;
class UI_Editor;

class Hierarchy : public DockData, public TreeDisplay<UI_Item>
{
public:
	Hierarchy(UI_Editor* editor) : DockData(editor), TreeDisplay<UI_Item>() { name = "Hierarchy"; };
	~Hierarchy() {};

	void Draw();
};

#endif
