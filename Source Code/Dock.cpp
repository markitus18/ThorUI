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
	if (root == true)
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus;

		ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
		ImGui::Begin(name.c_str(), &open, flags);
	}
	else
	{
		ImGui::PushID(this);
		ImGui::BeginChild("Child", ImVec2(size.x, size.y), true);

	}
	switch (separation)
	{
		case(NONE):
		{
			if (!data_children.empty())
			{
				DrawTabPanels();
			}
			else
			{
				ImGui::Text("Child");
				if (ImGui::Button("Split"))
				{
					Split(parent->separation == HORIZONTAL ? VERTICAL : HORIZONTAL);
				}
			}
			break;
		}
		case(VERTICAL): {}
		case(HORIZONTAL):
		{
			if (dock_children.size() == 2)
			{
				dock_children[0]->Draw();
			}

			if (separation == VERTICAL) ImGui::SameLine(0, 2);

			DrawSeparator();

			if (separation == VERTICAL) ImGui::SameLine(0, 2);

			ImVec2 cursor_pos = ImGui::GetCursorPos();
			float padding = (separation == VERTICAL ? ImGui::GetStyle().WindowPadding.x : ImGui::GetStyle().WindowPadding.y);
			float child_size = (separation == VERTICAL ? size.x - cursor_pos.x - padding : size.y - cursor_pos.y - padding);

			if (dock_children.size() == 2)
			{
				dock_children[1]->Draw();
			}
			break;
		}
	}
	if (root == true)
	{
		ImGui::End();
	}
	else
	{
		ImGui::EndChild();
		ImGui::PopID();
	}
}

void Dock::DrawSeparator()
{
	ImVec2 button_size = (separation == VERTICAL ?	ImVec2(5, ImGui::GetContentRegionAvail().y ) :
														ImVec2(ImGui::GetContentRegionAvail().x, 5));
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
		separation == VERTICAL ? dock_children[0]->size.x = separator.position : dock_children[0]->size.y = separator.position;
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

void Dock::Split(Separation_Type type)
{
	separation = type;

	ClearDockChildren();
	dock_children.push_back(new Dock("Child 1"));
	dock_children.push_back(new Dock("Child 2"));

	while(data_children.size() > 0)
	{
		dock_children[0]->AddChildData(data_children[0]);
		data_children.erase(data_children.begin());
	}
	dock_children[0]->root = false;
	separation == VERTICAL ? dock_children[0]->size.x = separator.position : dock_children[0]->size.y = separator.position;
	dock_children[1]->root = false;

	dock_children[0]->parent = this;
	dock_children[1]->parent = this;
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

void Dock::ClearDockChildren()
{
	for (uint i = 0; i < dock_children.size(); ++i)
	{
		delete dock_children[i];
	}
	dock_children.clear();
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