#include "InspectorDock.h"

#include "UI_Item.h"
#include "UI_Image.h"
#include "UI_Button.h"
#include "UI_Text.h"

#include "ImGui\imgui.h"
#include "Editor.h"

void Inspector::Draw()
{
	if (editor->selected != nullptr)
	{
		UI_Item* selected = editor->selected;
		char name[50];
		strcpy_s(name, 50, selected->GetName());
		ImGuiInputTextFlags name_input_flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;
		if (ImGui::InputText("###", name, 50, name_input_flags))
			selected->SetName(name);
		ImGui::Separator();

		Vec2 pos = selected->GetPos();
		if (ImGui::DragFloat2("Position", &pos))
		{
			selected->SetPos(pos);
		}

		Vec2 size = selected->GetSize();
		if (ImGui::DragFloat2("Size", &size))
		{
			selected->SetSize(size);
		}

		Vec2 pivot = selected->GetPivot();
		if (ImGui::DragFloat2("Pivot", &pivot, 0.03f))
		{
			selected->SetPivot(pivot);
		}

		ImGui::Separator();

		switch (selected->GetType())
		{
			case(Image):
			{
				UI_Image* img = (UI_Image*)selected;
				DrawImage((UI_Image*)selected);
				break;
			}
			case(Text):
			{
				UI_Text* txt = (UI_Text*)selected;
				DrawText((UI_Text*)selected);
				break;
			}
		}
	}
}

void Inspector::DrawImage(UI_Image* img)
{

}

void Inspector::DrawText(UI_Text* text)
{

}

void Inspector::DrawButton(UI_Button* button)
{

}