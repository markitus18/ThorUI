#include "SceneDock.h"

#include "Editor.h"
#include "Dock.h"

#include "ImGui\imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui\imgui_internal.h"

//tmp
#include "glew-2.1.0\include\GL\glew.h"

void Scene::Draw()
{
	Vec2 space_size = Vec2(parent->size.x, parent->size.y) - Vec2(10, 10);
	Vec2 final_size = scene_size.FitInRect(space_size);

	ImVec2 cursor_pos = ImGui::GetCursorPos();
	ImVec2 offset = ImVec2(space_size.x - final_size.x, space_size.y - final_size.y) / 2;
	cursor_pos += offset;
	ImGui::SetCursorPos(cursor_pos);

	ImGui::Image((ImTextureID)renderTexture, ImVec2(final_size.x, final_size.y), ImVec2(0, 1), ImVec2(1, 0));
}