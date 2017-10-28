#include "Editor.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl_gl3.h"
#include "glew-2.1.0\include\GL\glew.h"

UI_Editor::UI_Editor()
{

}

UI_Editor::~UI_Editor()
{

}

bool UI_Editor::Init(SDL_Window* window)
{
	this->window = window;
	if (ImGui_ImplSdlGL3_Init(window) == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UI_Editor::Draw()
{
	ImGui_ImplSdlGL3_NewFrame(window);
	if (ImGui::Begin("New Window"))
	{
		ImGui::End();
	}
	ImGui::Render();
}

bool UI_Editor::CleanUp()
{
	return true;
}