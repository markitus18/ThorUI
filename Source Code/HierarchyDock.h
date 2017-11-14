#ifndef __HIERARCHY_D_H__
#define __HIERARCHY_D_H__

#include "DockData.h"

class UI_Item;
class UI_Editor;

class Hierarchy : public DockData
{
public:
	Hierarchy(UI_Editor* editor) : DockData(editor) { name = "Hierarchy"; };
	~Hierarchy() {};

	void Draw();
	void DrawChild(UI_Item* item);
	void DrawItemChilds(UI_Item* item);
};

#endif
