#include "Dock.h"

#include "ImGui\imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui\imgui_internal.h"

#include "Log.h"
#include "DockData.h"

Dock::Dock(const char* name, Vec2 size): size(size), name(name)
{

}

Dock::~Dock()
{

}

void Dock::Draw()
{
	ImGuiWindowFlags flags =	ImGuiWindowFlags_NoTitleBar			| ImGuiWindowFlags_NoCollapse |
								ImGuiWindowFlags_NoSavedSettings	| ImGuiWindowFlags_NoScrollbar |
								ImGuiWindowFlags_NoScrollWithMouse	| ImGuiWindowFlags_NoBringToFrontOnFocus;

	ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
	if (ImGui::Begin(name.c_str(), &open, flags))
	{
		switch (separation)
		{
			case(NONE):
			{
				if (!data_children.empty())
				{
					DrawTabPanels();
				}
				break;
			}
			case(VERTICAL): {}
			case(HORIZONTAL):
			{
				DrawAsChild(separation, separator.position);

				if (separation == VERTICAL) ImGui::SameLine(0, 2);

				DrawSeparator();

				if (separation == VERTICAL) ImGui::SameLine(0, 2);

				ImVec2 cursor_pos = ImGui::GetCursorPos();
				float padding = (separation == VERTICAL ? ImGui::GetStyle().WindowPadding.x : ImGui::GetStyle().WindowPadding.y);
				float child_size = (separation == VERTICAL ? size.x - cursor_pos.x - padding : size.y - cursor_pos.y - padding);

				DrawAsChild(separation, child_size);
				break;
			}
		}
	}
	ImGui::End();
}

void Dock::DrawAsChild(Separation_Type sep_type, float size)
{
	ImGui::PushID(std::rand());
	ImGui::BeginChild("###", ( separation == VERTICAL ? ImVec2(size, 0) : ImVec2(0, size)), true);
		ImGui::Text("Child");
	ImGui::EndChild();
	ImGui::PopID();

}

void Dock::DrawSeparator()
{
	ImVec2 button_size = (separation == VERTICAL ?	ImVec2(5, size.y - ImGui::GetStyle().WindowPadding.y * 2) :
														ImVec2(size.x - ImGui::GetStyle().WindowPadding.x * 2, 5));
	ImGui::Button("b", button_size);

	if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
	{
		if (separator.pressed == false)
		{
			separator.pressed = true;
			separator.init_position = separator.position;
		}
	}
	else if (separator.pressed == true && !ImGui::IsMouseDown(0))
	{
		separator.pressed = false;
	}
	if (separator.pressed == true)
	{
		ImVec2 delta = ImGui::GetMouseDragDelta(0);
		separator.position = separator.init_position + (separation == VERTICAL ? delta.x : delta.y);
	}
}

void Dock::DrawTabPanels()
{
	for (int i = 0; i < data_children.size(); i++)
	{
		DrawSingleTab(data_children[i]);
	}
}

void Dock::DrawSingleTab(DockData* data)
{
	ImGui::SameLine(0, 15);

	if (!DoesTabFit(data))
	{
		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::SameLine(0, 15);
		int k = 1;
	}

	float line_height = ImGui::GetTextLineHeightWithSpacing();
	ImVec2 size(ImGui::CalcTextSize(data->name.c_str(), data->name.c_str() + data->name.length()).x, line_height);
	if (ImGui::InvisibleButton(data->name.c_str(), size))
	{
		SetDataActive(data);
	}

	bool hovered = ImGui::IsItemHovered();

	if (hovered && ImGui::IsMouseClicked(2))
	{
		RemoveChildData(data);
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
	draw_list->PathFillConvex(hovered ? color_hovered : (data->IsActive() ? color_active : color));
	draw_list->AddText(pos, ImGui::GetColorU32(ImGuiCol_Text), data->name.c_str(), nullptr);
	draw_list->PathClear();
}

void Dock::AddChildData(DockData* data, int position)
{
	data->SetParent(this);
	if (position == -1 || position > data_children.size())
		data_children.push_back(data);
	else
		data_children.insert(data_children.begin() + position, data);
}

bool Dock::DoesTabFit(DockData* data)
{
	ImVec2 cursor_pos = ImGui::GetCursorPos();
	ImVec2 size(ImGui::CalcTextSize(data->name.c_str(), data->name.c_str() + data->name.length()).x, 0);
	return (cursor_pos.x + size.x + 15 < ImGui::GetWindowSize().x);
}

void Dock::RemoveChildData(DockData* dock)
{
	for (std::vector<DockData*>::iterator it = data_children.begin(); it != data_children.end(); ++it)
	{
		if (*it == dock)
		{
			data_children.erase(it);
			break;
		}
	}
}

void Dock::Close()
{
	/*
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
	*/
}

void Dock::SetDataActive(DockData* data)
{
	for (uint i = 0; i < data_children.size(); ++i)
	{
		data_children[i]->SetActive(data == data_children[i]);
	}
}

void Dock::CloseDockData(DockData* data)
{
	std::vector<DockData*>::iterator it;
	for (it = data_children.begin(); it != data_children.end(); ++it)
	{
		if ((*it) == data)
		{
			data_children.erase(it);
			data->SetActive(false);
		}
	}
}