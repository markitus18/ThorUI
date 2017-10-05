#include "UI_Button.h"
#include "ThorUI.h"
#include "Log.h"

void UI_Button::Draw()
{
	ThorUI::DrawQuad(pos, size, current_color, true, 5.0f);
}

void UI_Button::OnItemEvent(Item_Event event)
{
	UI_Item::OnItemEvent(event);
	switch (event)
	{
		case(Mouse_Enter):	current_color = color[1]; break;
		case(Mouse_Down):	current_color = color[2]; break;
		case(Mouse_Up):		current_color = color[1]; break;
		case(Mouse_Exit):	current_color = color[0]; break;
	}
}