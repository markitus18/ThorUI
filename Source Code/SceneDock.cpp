#include "SceneDock.h"

#include "Editor.h"
#include "Dock.h"

#include "ImGui\imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui\imgui_internal.h"

void Scene::Draw()
{
	ImVec2 original_size(parent->size.x, parent->size.y); ImGui::GetContentRegionAvail();
	original_size.y -= 28;
	original_size.x -= 11;

	ImVec2 size_v = original_size;
	float screen_ratio = scene_size.x / scene_size.y;
	float x_ratio = size_v.x / scene_size.x;
	float y_ratio = size_v.y / scene_size.y;

	if (x_ratio < y_ratio)
	{
		size_v.y = size_v.x / screen_ratio;
	}
	else
	{
		size_v.x = size_v.y * screen_ratio;
	}
	
	ImVec2 cursor_pos = ImGui::GetCursorPos();
	cursor_pos += (original_size - size_v) / 2;
	ImGui::SetCursorPos(cursor_pos);
	ImGui::Image((ImTextureID)renderTexture, size_v, ImVec2(0, 1), ImVec2(1, 0));
}