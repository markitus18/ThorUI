#include "InspectorDock.h"

#include "UI_Item.h"
#include "UI_Image.h"
#include "UI_Button.h"
#include "UI_Text.h"

#include "ImGui\imgui.h"
#include "ImGui\imgui_internal.h"

#include "ThorUI.h"
#include "Editor.h"
#include "HierarchyDock.h"

#include "Dock.h"
#include "FileSystem.h"

void Inspector::Draw()
{
	if (editor->hierarchy->selected.size() == 0)
		return;

	UI_Item* selected = editor->hierarchy->selected.front();

	bool active = selected->IsActive();
	if (ImGui::Checkbox("", &active))
	{
		selected->SetActive(active);
	}
	ImGui::SameLine();
	DisplayItemName(selected);

	if (editor->dev_tools == true)
	{
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 1.0, 0, 1.0));
		ImGui::Text("ID: %i", selected->GetID());
		ImGui::PopStyleColor();
	}

	ImGui::Separator();

	Vec2 pos = selected->GetTransform()->GetPos();
	if (ImGui::DragFloat2("Position", &pos))
	{
		selected->GetTransform()->SetPos(pos);
	}

	Vec2 size = selected->GetSize();
	if (ImGui::DragFloat2("Size", &size))
	{
		selected->SetSize(size);
	}
	Vec2 scale = selected->GetScale();
	if (ImGui::DragFloat2("Scale", &scale, 0.03f))
	{
		selected->SetScale(scale);
	}
	ImGui::Text("Angle");
	ImGui::SameLine(0, 100 - ImGui::CalcTextSize("Angle").x);
	float angle = selected->GetTransform()->GetRotation();
	if (ImGui::DragFloat("##angle_transform", &angle))
	{
		selected->GetTransform()->SetRotationDeg(angle);
	}
		
	Vec2 pivot = selected->GetPivot();;
	if (ImGui::DragFloat2("Pivot", &pivot, 0.03f))
	{
		selected->SetPivot(pivot);
	}
		
	if (editor->dev_tools == true)
	{
		if (ImGui::CollapsingHeader("Local Matrix"))
		{
			const float* ptr = selected->GetTransform()->Local().Ptr();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1));

			for (uint i = 0; i < 9; ++i)
			{
				ImGui::Text("%.2f  ", ptr[i]);
				if ((i + 1) % 3 != 0)
				{
					ImGui::SameLine(((i + 1) % 3) * 60);
				}

			}
			ImGui::PopStyleColor();
		}
		if (ImGui::CollapsingHeader("Center Matrix"))
		{
			const float* ptr = selected->GetTransform()->Center().Ptr();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1));

			for (uint i = 0; i < 9; ++i)
			{
				ImGui::Text("%.2f  ", ptr[i]);
				if ((i + 1) % 3 != 0)
				{
					ImGui::SameLine(((i + 1) % 3) * 60);
				}

			}
			ImGui::PopStyleColor();
		}
		if (ImGui::CollapsingHeader("Global Matrix"))
		{
			const float* ptr = selected->GetTransform()->Global().Ptr();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1));

			for (uint i = 0; i < 9; ++i)
			{
				ImGui::Text("%.2f  ", ptr[i]);
				if ((i + 1) % 3 != 0)
				{
					ImGui::SameLine(((i + 1) % 3) * 60);
				}

			}
			ImGui::PopStyleColor();
		}

	}

	ImGui::Separator();

	switch (selected->GetType())
	{
		case(Image):
		{
			DrawImageItem((UI_Image*)selected);
			break;
		}
		case(Text):
		{
			DrawTextItem((UI_Text*)selected);
			break;
		}
		case(Button):
		{
			DrawButtonItem((UI_Button*)selected);
			break;
		}
	}

	DisplayItemEvents(selected);
}

void Inspector::DisplayItemName(UI_Item* item)
{
	char name[50];
	strcpy_s(name, 50, item->GetName());
	ImGuiInputTextFlags name_input_flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;
	if (ImGui::InputTextEx("###", name, 50, ImVec2(200, 0), name_input_flags))
		item->SetName(name);
}

void Inspector::DrawImageItem(UI_Image* img)
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
			std::string fileName = FileSystem::OpenFileDialog();
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

void Inspector::DrawTextItem(UI_Text* text)
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

void Inspector::DrawButtonItem(UI_Button* button)
{
	Color color = button->GetColor();
	if (ImGui::ColorEdit3("Color", color.ptr()))
	{
		button->SetColor(color);
	}
}

void Inspector::DisplayItemEvents(UI_Item* item)
{
	if (ImGui::CollapsingHeader("Events"))
	{
		std::vector<Signal_Event>& events = item->GetSignalEvents();
		std::vector<Signal_Event>::iterator it;
		for (it = events.begin(); it != events.end(); ++it)
		{
			Signal_Event& ev = (*it);
			UI_Item* event_holder = nullptr;
			if (ev.item_signal_id != 0) event_holder = ThorUI::GetItem(ev.item_signal_id);
			
			ImGui::PushID(&ev);
			DisplayEventItemMenu(item, event_holder, ev);
			if (event_holder != nullptr)
				DisplaySignalMenu(item, event_holder, ev);

			ImGui::Separator();
			ImGui::PopID();
		}

		ImGui::NewLine();ImGui::SameLine(0, 50);
		if (ImGui::Button("Add Event", ImVec2(ImGui::GetWindowWidth() - 100, 0)))
		{
			events.push_back(Signal_Event());
		}
	}
}

void Inspector::DisplayEventItemMenu(UI_Item* item, UI_Item* ev_holder, Signal_Event& ev)
{
	ImGui::Text("Item Signal: "); ImGui::SameLine();
	ImVec4 text_color = ImGui::GetStyle().Colors[ImGuiCol_Text];
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.23f, 0.61f, 0.81f, 1.0f));
	if (ImGui::BeginMenu_ThorUI(ev.item_signal_id == 0 ? "-- Select Item --" : ev_holder->GetName()))
	{
		ImGui::PushStyleColor(ImGuiCol_Text, text_color);
		if (ImGui::MenuItem("Self"))
		{
			ev.item_signal_id = item->GetID();
		}
		ImGui::Separator();

		for (uint i = 1; i < ThorUI::items.size(); ++i)
		{
			if (ThorUI::items[i] != item)
			{
				ImGui::PushID(ThorUI::items[i]->GetID());
				if (ImGui::MenuItem(ThorUI::items[i]->GetName()))
				{
					ev.item_signal_id = ThorUI::items[i]->GetID();
				}
				ImGui::PopID();
			}
		}
		ImGui::PopStyleColor();
		ImGui::EndMenu();
	}
	else
	{
		if (ImGui::IsItemHovered() && ThorUI::IsMouseDown(SDL_BUTTON_RIGHT))
		{
			ev.item_signal_id = 0;
		}
	}
	ImGui::PopStyleColor();
}

void Inspector::DisplaySignalMenu(UI_Item* item, UI_Item* ev_holder, Signal_Event& ev)
{
	ImGui::Text("  Signal:    "); ImGui::SameLine();
	ImVec4 text_color = ImGui::GetStyle().Colors[ImGuiCol_Text];
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.23f, 0.61f, 0.81f, 1.0f));
	if (ImGui::BeginMenu_ThorUI(ev.signal_name == "" ? "-- Select Signal --" : ev.signal_name.c_str()))
	{
		ImGui::PushStyleColor(ImGuiCol_Text, text_color);

		std::vector<std::string> signals = ev_holder->GetSignalsStr();
		for (uint i = 0; i < signals.size(); ++i)
		{
			if (ImGui::MenuItem(signals[i].c_str()))
			{

			}
		}

		ImGui::PopStyleColor();
		ImGui::EndMenu();
	}
	else
	{
		if (ImGui::IsItemHovered() && ThorUI::IsMouseDown(SDL_BUTTON_RIGHT))
		{
			ev.signal_name = "";
			ev.signal_id = 0;
		}
	}
	ImGui::PopStyleColor();
}