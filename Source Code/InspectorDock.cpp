#include "InspectorDock.h"

#include "UI_Item.h"
#include "UI_Image.h"
#include "UI_Button.h"
#include "UI_Text.h"

#include "ImGui\imgui.h"
#include "ImGui\imgui_internal.h"

#include "ThorUI.h"
#include "Editor.h"
#include "Dock.h"

void Inspector::Draw()
{
	if (editor->selected != nullptr)
	{
		UI_Item* selected = editor->selected;

		bool active = selected->IsActive();
		if (ImGui::Checkbox("", &active))
		{
			selected->SetActive(active);
		}
		ImGui::SameLine();
		DisplayItemName(selected);
		ImGui::Separator();

		ImGui::BeginChild("ChildID", ImVec2(parent->size.x < 330 ? 0 : 330, ImGui::GetItemsLineHeightWithSpacing() * 3)); //To limit DragFloat size
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
		ImGui::EndChild();
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

void Inspector::DisplayItemName(UI_Item* item)
{
	char name[50];
	strcpy_s(name, 50, item->GetName());
	ImGuiInputTextFlags name_input_flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;
	if (ImGui::InputTextEx("###", name, 50, ImVec2(200, 0), name_input_flags))
		item->SetName(name);
}

void Inspector::DrawImage(UI_Image* img)
{
	ImGui::Text("Source Image");
	if (img->GetTexID() != 0)
	{
		editor->DisplayTexture(ThorUI::GetTexture(img->GetTexID()));
	}
	if (ImGui::BeginMenu_ThorUI("Set Texture: "))
	{
		std::map<uint, ThorUI::Texture>::iterator it;
		for (it = ThorUI::textures.begin(); it != ThorUI::textures.end(); ++it)
		{
			if (ImGui::MenuItem((*it).second.path.c_str()))
			{
				img->SetTexture((*it).second.id);
			}
		}
		if (ImGui::MenuItem("Load New Texture..."))
		{
			std::string fileName = editor->OpenFileDialog();
			if (fileName != "")
			{
				img->SetTexture(ThorUI::LoadTexture(fileName.c_str()));
			}
		}
		ImGui::EndMenu();
	}
	Color color = img->GetColor();
	if (ImGui::ColorEdit3("Color", color.ptr()))
	{
		img->SetColor(color);
	}
}

void Inspector::DrawText(UI_Text* text)
{
	char orig_text[50];
	strcpy_s(orig_text, 50, text->GetText());

	if (ImGui::InputText("Text", orig_text, 50))
	{
		text->SetText(orig_text);
	}

	Color color = text->GetColor();
	if (ImGui::ColorEdit3("Color", color.ptr()))
	{
		text->SetColor(color);
	}
}

void Inspector::DrawButton(UI_Button* button)
{

}