#include "HierarchyDock.h"

#include "ImGui\imgui.h"
#include "UI_Item.h"
#include "ThorUI.h"
#include "Editor.h"

#include "SDL2-2.0.6\include\SDL.h"

void Hierarchy::Draw()
{
	DrawItemChilds(ThorUI::window_item);
}

void Hierarchy::DrawChild(UI_Item* item)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
	if (item->GetChildCount() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	if (item == editor->selected)
		flags |= ImGuiTreeNodeFlags_Selected;
	if (item->IsParentActive() == false)
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 0.4));
	bool node_open = ImGui::TreeNodeEx(item, flags, item->GetName());
	if (item->IsParentActive() == false)
		ImGui::PopStyleColor();
	if (ImGui::IsItemHoveredRect() && ThorUI::GetMouseState(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		editor->selected = item;
	}

	if (node_open == true)
	{
		if (item->GetChildCount() > 0)
		{
			DrawItemChilds(item);
			ImGui::TreePop();
		}
	}
}

void Hierarchy::DrawItemChilds(UI_Item* item)
{
	const std::vector<UI_Item*> children = item->GetChildren();
	for (std::vector<UI_Item*>::const_iterator it = children.begin(); it != children.end(); ++it)
	{
		DrawChild(*it);
	}
}