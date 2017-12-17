#include "SceneDock.h"

#include "Dock.h"
#include "ThorUI.h"
#include "Log.h"
#include "Editor.h"
#include "UI_Item.h"

//External Libraries include
#include "ImGui\imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui\imgui_internal.h"
#include "glew-2.1.0\include\GL\glew.h"

#include "ImGui\ImGuizmo\ImGuizmo.h"

Scene::Scene(UI_Editor* editor) : DockData(editor)
{
	name = "Scene";
	GenScreenBuffers();
}

void Scene::Draw()
{
	ImGuizmo::SetDrawlist();
	Vec2 space_size = Vec2(parent->size.x, parent->size.y) - Vec2(10, 10);
	Vec2 final_size = ThorUI::screen_size.FitInRect(space_size);

	ImVec2 offset = ImVec2(space_size.x - final_size.x, space_size.y - final_size.y) / 2;
	ImGui::SetCursorPos(ImGui::GetCursorPos() + offset);
	ImVec2 global_cursor = ImGui::GetCursorScreenPos();

	ImGui::Image((ImTextureID)renderTexture, ImVec2(final_size.x, final_size.y), ImVec2(0, 1), ImVec2(1, 0));

	if (editor->selected != nullptr)
	{
		Vec2 item_pos = editor->selected->GetGlobalPos();
		Vec2 win_size = editor->window_size;
		Vec2 relative_pos = item_pos / win_size;
		relative_pos.y = 1 - relative_pos.y;
		Vec2 final_pos = relative_pos * final_size;

		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		draw_list->AddRectFilled(global_cursor + ImVec2(final_pos.x, final_pos.y), global_cursor + ImVec2(final_pos.x, final_pos.y) + ImVec2(50, -2), 0xFF0000AA);
		draw_list->AddRectFilled(global_cursor + ImVec2(final_pos.x, final_pos.y), global_cursor + ImVec2(final_pos.x, final_pos.y) + ImVec2(2, -50), 0xFFAA0000);

	}
}

void Scene::GenScreenBuffers()
{
	//Generating buffers for scene render
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	//Generating texture to render to
	glGenTextures(1, &renderTexture);
	glBindTexture(GL_TEXTURE_2D, renderTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ThorUI::screen_size.x, ThorUI::screen_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Configuring frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG("Error creating screen buffer");
	}


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}