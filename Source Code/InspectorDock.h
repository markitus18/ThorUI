#ifndef __INSPECTOR_D_H__
#define __INSPECTOR_D_H__

#include "DockData.h"

class UI_Image;
class UI_Button;
class UI_Text;
class UI_Item;
class UI_Editor;

class Inspector : public DockData
{
public:
	Inspector(UI_Editor* editor) : DockData(editor) { name = "Inspector"; }
	~Inspector() {};

	void Draw();

private:
	void DrawImage(UI_Image* img);
	void DrawText(UI_Text* text);
	void DrawButton(UI_Button* button);
};

#endif