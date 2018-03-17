#ifndef __RESOURCES_D_H__
#define __RESOURCES_D_H__

#include "DockData.h"

class UI_Editor;
class Resources : public DockData
{
public:
	Resources(UI_Editor* editor) : DockData(editor) { name = "Resources"; }
	~Resources() {};

	void Draw();
};

#endif // !__RESOURCES_D_H__
