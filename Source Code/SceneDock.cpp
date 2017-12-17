#include "SceneDock.h"

#include "Dock.h"
#include "ThorUI.h"
#include "Log.h"
#include "Editor.h"
#include "UI_Item.h"
#include "Rect.h"
#include "SDL2-2.0.6\include\SDL_scancode.h"

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
	Vec2 win_size = Vec2(parent->size.x, parent->size.y) - Vec2(10, 10);
	Vec2 image_size = ThorUI::screen_size.FitInRect(win_size);

	ImVec2 offset = ImVec2(win_size.x - image_size.x, win_size.y - image_size.y) / 2;
	ImGui::SetCursorPos(ImGui::GetCursorPos() + offset);
	Vec2 img_corner = editor->ToVec2(ImGui::GetCursorScreenPos()) - Vec2(0, image_size.y);
	ImGui::Image((ImTextureID)renderTexture, ImVec2(image_size.x, image_size.y), ImVec2(0, 1), ImVec2(1, 0));

	if (editor->selected != nullptr)
	{
		Vec2 relative_pos = editor->selected->GetGlobalPos() / editor->window_size;
		Vec2 pos_on_image = relative_pos * image_size;

		Vec2 initial_pos = img_corner + pos_on_image;

		Rect x_axis = Rect(initial_pos + Vec2(0, -1), Vec2(80, 3));
		Rect y_axis = Rect(initial_pos + Vec2(-1, 0), Vec2(3, 80));

		editor->DrawRect(x_axis, 0xFFAA0000);
		editor->DrawRect(y_axis, 0xFF0000AA);

		Vec2 final_x = initial_pos + Vec2(70, 0);
		Vec2 final_y = initial_pos + Vec2(0, 70);
		editor->DrawTriangle(final_x + Vec2(0, 10), final_x + Vec2(15, 0), final_x + Vec2(0, -10), 0xFFAA0000);
		editor->DrawTriangle(final_y + Vec2(10, 0), final_y + Vec2(0, 15), final_y + Vec2(-10, 0), 0xFF0000AA);

		Rect x_button = Rect(initial_pos + Vec2(8, -8), Vec2(70, 16));
		Rect y_button = Rect(initial_pos + Vec2(-8, 8), Vec2(16, 70));
		Rect xy_button = Rect(initial_pos + Vec2(-8, -8), Vec2(16, 16));

		editor->DrawRect(x_button, 0x51AA0000);
		editor->DrawRect(y_button, 0x510000AA);
		editor->DrawRect(xy_button, 0x5100AA00);

		Vec2 mouse_pos = editor->ToVec2(ImGui::GetMousePos());

		if (xy_button.Contains(mouse_pos) && ThorUI::GetMouseState(1) == KEY_DOWN)
		{
			dragging = true;
			x_start = mouse_pos.x;
			y_start = mouse_pos.y;
			x_item_start = editor->selected->GetGlobalPos().x;
			y_item_start = editor->selected->GetGlobalPos().y;
		}
		else if (x_button.Contains(mouse_pos) && ThorUI::GetMouseState(1) == KEY_DOWN)
		{
			dragging = true;
			x_start = mouse_pos.x;
			x_item_start = editor->selected->GetGlobalPos().x;
		}
		else if (y_button.Contains(mouse_pos) && ThorUI::GetMouseState(1) == KEY_DOWN)
		{
			dragging = true;
			y_start = mouse_pos.y;
			x_item_start = editor->selected->GetGlobalPos().y;
		}

		if (dragging == true)
		{
			if (ThorUI::IsMouseDown(1) == false)
			{
				dragging = false;
				x_start = -1;
				y_start = -1;
			}
			else if (y_start == -1)
			{
				float delta_x = mouse_pos.x - x_start;
				float x_scale = image_size.x / editor->window_size.x;
				delta_x /= x_scale;
				editor->selected->SetGlobalPos(x_item_start + delta_x, editor->selected->GetGlobalPos().y);
			}
			else if (x_start == -1)
			{
				float delta_y = mouse_pos.y - y_start;
				float y_scale = image_size.y / editor->window_size.y;
				delta_y /= y_scale;
				editor->selected->SetGlobalPos(editor->selected->GetGlobalPos().x, x_item_start + delta_y);
			}
			else
			{
				Vec2 delta = mouse_pos - Vec2(x_start, y_start);
				Vec2  scale = image_size / editor->window_size;
				delta /= scale;
				editor->selected->SetGlobalPos(x_item_start + delta.x, y_item_start + delta.y);
			}

		}

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