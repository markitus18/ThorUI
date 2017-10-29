#include "Editor.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl_gl3.h"
#include "glew-2.1.0\include\GL\glew.h"
#include "ThorUI.h"
#include "UI_Image.h"
#include "SDL2-2.0.6\include\SDL.h"

UI_Editor::UI_Editor()
{

}

UI_Editor::~UI_Editor()
{

}

bool UI_Editor::Init(SDL_Window* window)
{
	this->window = window;
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	window_size.Set(w, h);

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
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::MenuItem("Button", "", false, false))
			{

			}
			if (ImGui::MenuItem("Image"))
			{
				Vec2 image_size = default_img_size_ratio * window_size;
				Vec2 image_pos = window_size / 2 - image_size / 2;
				
				UI_Image* image = new UI_Image(image_pos, image_size, ThorUI::LoadTexture(created ? "maul.bmp" : "photo.bmp"));
				if (created) image->SetName("image2");
				ThorUI::AddItem(image);
				created = true;
			}
			if (ImGui::MenuItem("Text", "", false, false))
			{

			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();

		DrawHierarchy();
		DrawInspector();
	}
	ImGui::Render();
}

bool UI_Editor::CleanUp()
{
	return true;
}

void UI_Editor::DrawHierarchy()
{
	if (ImGui::Begin("Hierarchy"))
	{
		DrawHierarchyNode(ThorUI::window_item);
		ImGui::End();
	}
}

void UI_Editor::DrawHierarchyNode(UI_Item* item)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
	if (item->GetChildCount() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

	bool node_open = ImGui::TreeNodeEx(item, flags, item->GetName());
	if (ImGui::IsItemHoveredRect() && ThorUI::GetMouseState(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		selected = item;
	}

	if (node_open == true)
	{
		if (item->GetChildCount() > 0)
		{
			const std::vector<UI_Item*> children = item->GetChildren();
			for (std::vector<UI_Item*>::const_iterator it = children.begin(); it != children.end(); ++it)
			{
				DrawHierarchyNode(*it);
			}
			ImGui::TreePop();
		}
	}
}

void UI_Editor::DrawInspector()
{
	if (ImGui::Begin("Inspector"))
	{
		if (selected != nullptr)
		{
			DrawItemData(selected);
		}
		ImGui::End();
	}
}

void UI_Editor::DrawItemData(UI_Item* item)
{
	if (item != nullptr)
	{
		ImGui::Text(item->GetName());
		ImGui::Separator();

		if (item->GetType() == Image)
		{
			UI_Image* img = (UI_Image*)item;

			if (img->GetTexID() != 0)
			{
				ImVec2 window_size = ImGui::GetWindowSize();
				float padding = ImGui::GetStyle().WindowPadding.x;
				Vec2 size = ThorUI::GetTexture(img->GetTexID())->original_size / 5;

				if (size.x > (window_size.x - padding * 2))
				{
					size.y /= (size.x / (window_size.x - padding * 2));
					size.x /= (size.x / (window_size.x - padding * 2));
				}

				ImGui::Image((ImTextureID)img->GetTexID(), ImVec2(size.x, size.y));
			}
		}
	}
}