#include "SceneDock.h"

#include "Dock.h"
#include "ThorUI.h"
#include "Log.h"

//External Libraries include
#include "ImGui\imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui\imgui_internal.h"
#include "glew-2.1.0\include\GL\glew.h"

void Scene::Draw()
{
	Vec2 space_size = Vec2(parent->size.x, parent->size.y) - Vec2(10, 10);
	Vec2 final_size = ThorUI::screen_size.FitInRect(space_size);

	ImVec2 cursor_pos = ImGui::GetCursorPos();
	ImVec2 offset = ImVec2(space_size.x - final_size.x, space_size.y - final_size.y) / 2;
	cursor_pos += offset;
	ImGui::SetCursorPos(cursor_pos);

	ImGui::Image((ImTextureID)renderTexture, ImVec2(final_size.x, final_size.y), ImVec2(0, 1), ImVec2(1, 0));
}

void Scene::GenerateScreenBuffers()
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