#include "Dock.h"

#include "ImGui\imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui\imgui_internal.h"

#include "Log.h"
//using namespace ImGui;

Dock::Dock(const char* name, Vec2 size): size(size), name(name)
{

}

Dock::~Dock()
{

}

void Dock::Draw()
{
	if (!open) return;

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoBringToFrontOnFocus;

	ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
	if (ImGui::Begin(name.c_str(), &open, flags))
	{

	}
	//ImGui::Separator();

	bool child_drawn = false;
	if (ImGui::BeginChild("Child"))
	{
		ImVec2 win_size = ImGui::GetWindowSize();

		ImGui::BeginChild("Second", ImVec2(separator_position, 0), true);
		{
			DrawTabPanels();
			ImGui::Separator();

			for (uint i = 0; i < childs.size() && !child_drawn; ++i)
			{
				if (childs[i]->active == true)
				{
					child_drawn = true;
					childs[i]->DrawData();
				}
			}

			if (!child_drawn) DrawData();
		}
		ImGui::EndChild();
		
		ImGui::SameLine();
		ImGui::SameLine(0, 2);

		ImGui::Button("b", ImVec2(5, win_size.y));

		if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
		{
			if (button_pressed == false)
			{
				button_pressed = true;
				init_separator_position = separator_position;
			}
		}
		else if (button_pressed == true && !ImGui::IsMouseDown(0))
		{
			button_pressed = false;
		}



		ImGui::SameLine(0, 2);
		//ImGui::SetCursorPos(c_pos);
		ImVec2 cursor_pos = ImGui::GetCursorPos();
		float padding = ImGui::GetStyle().WindowPadding.x;
		float size = win_size.x - cursor_pos.x;
		ImGui::BeginChild("Third", ImVec2(size, 0), true);
		ImGui::Text("Third Child");
		ImGui::EndChild();

		if (button_pressed == true)
		{
			ImVec2 delta = ImGui::GetMouseDragDelta(0);
			separator_position = init_separator_position + delta.x;
		}

		ImGui::EndChild();
	}
	ImGui::End();
}

void Dock::DrawData()
{
	ImGui::Text("Drawing Child: %s", name.c_str());

	if (ImGui::Button("Close panel"))
	{
		Close();
	}
}

void Dock::DrawTabPanels()
{
	DrawSingleTab(this, 0);
	for (int i = 0; i < childs.size(); i++)
	{
		DrawSingleTab(childs[i], i+1);
	}
}

void Dock::DrawSingleTab(Dock* dock, uint index)
{
	ImGui::SameLine(0, 15);

	if (!DoesTabFit(dock))
	{
		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::SameLine(0, 15);
		int k = 1;
	}

	float line_height = ImGui::GetTextLineHeightWithSpacing();
	ImVec2 size(ImGui::CalcTextSize(dock->name.c_str(), dock->name.c_str() + dock->name.length()).x, line_height);
	if (ImGui::InvisibleButton(dock->name.c_str(), size))
	{
		this->SetActive(false);
		for (uint i = 0; i < this->childs.size(); ++i)
		{
			childs[i]->SetActive(false);
		}
		dock->SetActive(true);
	}

	bool hovered = ImGui::IsItemHovered();

	if (hovered && ImGui::IsMouseClicked(2))
	{
		dock->Close();
	}

	ImU32 color = ImGui::GetColorU32(ImGuiCol_FrameBg);
	ImU32 color_active = ImGui::GetColorU32(ImGuiCol_FrameBgActive);
	ImU32 color_hovered = ImGui::GetColorU32(ImGuiCol_FrameBgHovered);

	ImVec2 pos = ImGui::GetItemRectMin();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	draw_list->PathClear();
	draw_list->PathLineTo(pos + ImVec2(-15, size.y));
	draw_list->PathBezierCurveTo(
		pos + ImVec2(-10, size.y), pos + ImVec2(-5, 0), pos + ImVec2(0, 0), 10);
	draw_list->PathLineTo(pos + ImVec2(size.x, 0));
	draw_list->PathBezierCurveTo(pos + ImVec2(size.x + 5, 0),
		pos + ImVec2(size.x + 10, size.y),
		pos + ImVec2(size.x + 15, size.y),
		10);
	draw_list->PathFillConvex(hovered ? color_hovered : (dock->active ? color_active : color));
	draw_list->AddText(pos, ImGui::GetColorU32(ImGuiCol_Text), dock->name.c_str(), nullptr);
	draw_list->PathClear();
}

bool Dock::DoesTabFit(Dock* dock)
{
	ImVec2 cursor_pos = ImGui::GetCursorPos();
	ImVec2 size(ImGui::CalcTextSize(dock->name.c_str(), dock->name.c_str() + dock->name.length()).x, 0);
	return (cursor_pos.x + size.x + 15 < ImGui::GetWindowSize().x);
}

void Dock::AddChild(Dock* dock)
{
	childs.push_back(dock);
	dock->child_index = childs.size();
	dock->parent = this;
	dock->root = false;
}

void Dock::RemoveChild(Dock* dock)
{
	for (std::vector<Dock*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		if (*it == dock)
		{
			childs.erase(it);
			break;
		}
	}
}

void Dock::SetActive(bool active)
{
	this->active = active;
}

void Dock::Close()
{
	if (childs.size() > 0)
	{
		childs[0]->root = true;
		for (uint i = 1; i < childs.size(); ++i)
		{
			childs[0]->AddChild(childs[i]);
		}
		if (active) childs[0]->SetActive(true);
	}
	if (parent != nullptr)
	{
		parent->RemoveChild(this);
	}
	root = false;
}