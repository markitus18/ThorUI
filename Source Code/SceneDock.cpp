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
	SetGizmoOp(Gizmo_Op::TRANSLATION);
}

void Scene::Draw()
{
	HandleInput();

	ImGui::SetCursorPos(img_offset);
	img_corner = editor->ToVec2(ImGui::GetCursorScreenPos()) - Vec2(0, img_size.y);

	ImGui::Image((ImTextureID)renderTexture, ImVec2(img_size.x, img_size.y), ImVec2(0, 1), ImVec2(1, 0));
	
	if (editor->selected != nullptr)
	{
		switch (gizmo_op)
		{
			case Gizmo_Op::TRANSLATION:
				DrawTranslationGizmo();
				break;
			case Gizmo_Op::ROTATION:
				DrawRotationGizmo();
				break;
			case Gizmo_Op::SCALE:
				DrawScaleGizmo();
				break;
		}

		Vec2 mouse_pos = editor->ToVec2(ImGui::GetMousePos());

		if (drag == Drag_Type::NONE)
		{
			HandleGizmoActivation(mouse_pos);
		}
		else if (drag != Drag_Type::NONE)
		{
			HandleDrag(mouse_pos, img_size);
		}
	}
	
}

void Scene::OnResize()
{
	win_size = Vec2(parent->size.x, parent->size.y) - Vec2(10, 10);
	img_size = ThorUI::screen_size.FitInRect(win_size);
	img_offset = ImVec2(win_size.x - img_size.x, win_size.y - img_size.y) / 2 + ImVec2(0, 25);
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

void Scene::HandleInput()
{
	if (ThorUI::GetKeyState(SDL_SCANCODE_W) == KEY_DOWN)
		SetGizmoOp(Gizmo_Op::TRANSLATION);
	if (ThorUI::GetKeyState(SDL_SCANCODE_E) == KEY_DOWN)
		SetGizmoOp(Gizmo_Op::ROTATION);
	if (ThorUI::GetKeyState(SDL_SCANCODE_R) == KEY_DOWN)
		SetGizmoOp(Gizmo_Op::SCALE);
}

void Scene::HandleGizmoActivation(Vec2 mouse_pos)
{
	if (gizmo_op == Gizmo_Op::ROTATION)
	{
		if (rot_button.Contains(mouse_pos) && ThorUI::GetMouseState(1) == KEY_DOWN)
		{
			drag = Drag_Type::XY;
		}
	}
	else
	{
		if (trans_buttons[2].Contains(mouse_pos) && ThorUI::GetMouseState(1) == KEY_DOWN)
		{
			drag = Drag_Type::XY;
		}
		else if (trans_buttons[0].Contains(mouse_pos) && ThorUI::GetMouseState(1) == KEY_DOWN)
		{
			drag = Drag_Type::X;
		}
		else if (trans_buttons[1].Contains(mouse_pos) && ThorUI::GetMouseState(1) == KEY_DOWN)
		{
			drag = Drag_Type::Y;
		}
	}
	if (drag != Drag_Type::NONE)
	{
		start_drag = mouse_pos;
		init_drag_val = editor->selected->GetGlobalPos();
	}
}

void Scene::HandleDrag(Vec2 mouse_pos, Vec2 image_size)
{
	if (ThorUI::IsMouseDown(1) == false)
	{
		drag = Drag_Type::NONE;
	}
	else
	{
		Vec2 delta = mouse_pos - start_drag;
		Vec2 scale = image_size / editor->window_size;
		delta /= scale;
		if (delta.x != 0 && delta.y != 0)
		{
			int k = 1;
		}
		Vec2 previous_pos = editor->selected->GetGlobalPos();
		Vec2 final_pos = Vec2(drag != Drag_Type::Y ? init_drag_val.x + delta.x : init_drag_val.x,
			drag != Drag_Type::X ? init_drag_val.y + delta.y : init_drag_val.y);
		editor->selected->SetGlobalPos(final_pos.x, final_pos.y);
	}
}

void Scene::SetGizmoOp(Gizmo_Op op)
{
	gizmo_op = op;
}

void Scene::DrawTranslationGizmo()
{
	Vec2 relative_pos = editor->selected->GetGlobalPos() / editor->window_size;
	Vec2 pos_on_image = relative_pos * img_size;

	Vec2 initial_pos = img_corner + pos_on_image;

	Rect x_axis = Rect(initial_pos + Vec2(0, -1), Vec2(80, 3));
	Rect y_axis = Rect(initial_pos + Vec2(-1, 0), Vec2(3, 80));

	editor->DrawRect(x_axis, drag == Drag_Type::X ? 0xFF42F1F4 : 0xFFFF0000);
	editor->DrawRect(y_axis, drag == Drag_Type::Y ? 0xFF42F1F4 : 0xFF0000FF);

	Vec2 final_x = initial_pos + Vec2(70, 0);
	Vec2 final_y = initial_pos + Vec2(0, 70);
	editor->DrawTriangle(final_x + Vec2(0, 10), final_x + Vec2(15, 0), final_x + Vec2(0, -10), drag == Drag_Type::X ? 0xFF42F1F4 : 0xFFFF0000);
	editor->DrawTriangle(final_y + Vec2(10, 0), final_y + Vec2(0, 15), final_y + Vec2(-10, 0), drag == Drag_Type::Y ? 0xFF42F1F4 : 0xFF0000FF);

	trans_buttons[0] = Rect(initial_pos + Vec2(8, -8), Vec2(70, 16));
	trans_buttons[1] = Rect(initial_pos + Vec2(-8, 8), Vec2(16, 70));
	trans_buttons[2] = Rect(initial_pos + Vec2(-1, -1), Vec2(16, 16));

	editor->DrawRect(trans_buttons[2], drag == Drag_Type::XY ? 0xFF42F1F4 : 0xFF00AA00);
}

void Scene::DrawScaleGizmo()
{
	Vec2 relative_pos = editor->selected->GetGlobalPos() / editor->window_size;
	Vec2 pos_on_image = relative_pos * img_size;

	Vec2 initial_pos = img_corner + pos_on_image;

	Rect x_axis = Rect(initial_pos + Vec2(0, -1), Vec2(80, 3));
	Rect y_axis = Rect(initial_pos + Vec2(-1, 0), Vec2(3, 80));

	editor->DrawRect(x_axis, drag == Drag_Type::X ? 0xFF42F1F4 : 0xFFFF0000);
	editor->DrawRect(y_axis, drag == Drag_Type::Y ? 0xFF42F1F4 : 0xFF0000FF);

	Vec2 final_x = initial_pos + Vec2(70, 0);
	Vec2 final_y = initial_pos + Vec2(0, 70);

	Rect rect_x(initial_pos + Vec2(70, -7), Vec2(14, 14));
	Rect rect_y(initial_pos + Vec2(-7, 70), Vec2(14, 14));

	editor->DrawRect(rect_x, drag == Drag_Type::X ? 0xFF42F1F4 : 0xFFFF0000);
	editor->DrawRect(rect_y, drag == Drag_Type::Y ? 0xFF42F1F4 : 0xFF0000FF);

	trans_buttons[0] = Rect(initial_pos + Vec2(8, -8), Vec2(70, 16));
	trans_buttons[1] = Rect(initial_pos + Vec2(-8, 8), Vec2(16, 70));
	trans_buttons[2] = Rect(initial_pos + Vec2(-1, -1), Vec2(16, 16));

	editor->DrawRect(trans_buttons[2], drag == Drag_Type::XY ? 0xFF42F1F4 : 0xFF00AA00);
}

void Scene::DrawRotationGizmo()
{

}