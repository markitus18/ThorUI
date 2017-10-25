#include "UI_Button.h"
#include "ThorUI.h"
#include "Log.h"

void UI_Button::Draw()
{
	ThorUI::DrawQuad(pos, size, color, true, 5.0f);
}

void UI_Button::OnItemEvent(Item_Event event)
{
	UI_Item::OnItemEvent(event);
	switch (event)
	{
		case(Mouse_Enter):	color = color_data[1]; break;
		case(Mouse_Down):	color = color_data[2]; break;
		case(Mouse_Up):		color = color_data[1]; break;
		case(Mouse_Exit):	color = color_data[0]; break;
	}
}