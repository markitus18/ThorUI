#ifndef __INSPECTOR_D_H__
#define __INSPECTOR_D_H__

#include "DockData.h"

class UI_Image;
class UI_Button;
class UI_Text;
class UI_Item;
class UI_Editor;
struct Signal_Event;

class Inspector : public DockData
{
public:
	Inspector(UI_Editor* editor) : DockData(editor) { name = "Inspector"; }
	~Inspector() {};

	void Draw();

private:
	void DisplayItemName(UI_Item* item);
	void DrawImageItem(UI_Image* img);
	void DrawTextItem(UI_Text* text);
	void DrawButtonItem(UI_Button* button);

	//Events and signals
	void DisplayItemEvents(UI_Item* item);
	void DisplayEventItemMenu(UI_Item* item, UI_Item* ev_holder, Signal_Event& ev);
	void DisplaySignalMenu(UI_Item* item, UI_Item* ev_holder, Signal_Event& ev);
	void DisplaySignalParameters(Signal_Event& ev);
	
	//Apperance sets
	void DisplayItemApperance(UI_Item* item);
};

#endif
