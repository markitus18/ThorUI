#include "Editor.h"

#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl_gl3.h"
#include "glew-2.1.0\include\GL\glew.h"
#include "SDL2-2.0.6\include\SDL.h"

#include "ThorUI.h"
#include "UI_Image.h"
#include "UI_Button.h"
#include "UI_Text.h"

#include "Rect.h"
#include "Circle.h"

#include "FileSystem.h"
#include "ImGui\Dock\imgui_dock.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui\imgui_internal.h"
#include "Log.h"
#include "Dock.h"
#include "HierarchyDock.h"
#include "SceneDock.h"
#include "InspectorDock.h"
#include "ResourcesDock.h"

#include "ImGui\ImGuizmo\ImGuizmo.h"


//TMP TESTING
#include "Mat3x3.h"

UI_Editor::UI_Editor()
{

}

UI_Editor::~UI_Editor()
{

}

bool UI_Editor::Init(SDL_Window* window)
{
	bool ret = true;

	this->window = window;
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	window_size.Set(w, h);

	grid_div = window_size / grid_separation;
	if (ImGui_ImplSdlGL3_Init(window) == true)
	{
		ret = true;
	}
	else
	{
		ret = false;
	}

	ImGuiIO io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	//TODO: load own font

	ImGuiContext& g = *GImGui;
	float menubar_size = 13 + g.Style.FramePadding.y * 2.0f;
	float iconbar_size = 30.0f;

	ThorUI::Init(window);

	Dock* dock = new Dock("0 Dock", Vec2(window_size.x, window_size.y - menubar_size - iconbar_size));
	docks.push_back(dock);
	dock->position = Vec2(0, menubar_size + iconbar_size);
	dock->Split(HORIZONTAL, 0.8f);
	dock->GetDockChildren()[0]->Split(VERTICAL, 0.8f);
	dock->GetDockChildren()[0]->GetDockChildren()[0]->Split(VERTICAL, 0.2f);
	
	hierarchy = new Hierarchy(this);
	dock->GetDockChildren()[0]->GetDockChildren()[0]->GetDockChildren()[0]->AddChildData(hierarchy);
	hierarchy->AddNode(ThorUI::window_item);

	scene = new Scene(this);
	dock->GetDockChildren()[0]->GetDockChildren()[0]->GetDockChildren()[1]->AddChildData(scene);

	inspector = new Inspector(this);
	dock->GetDockChildren()[0]->GetDockChildren()[1]->AddChildData(inspector);

	resources = new Resources(this);
	dock->GetDockChildren()[1]->AddChildData(resources);

	return ret;
}

void UI_Editor::Draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, scene->frameBuffer);
	glClearColor(0.7f, 0.7f, 0.7f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	ThorUI::StartFrame(); //Just to use ThorUI keyboard and mouse input

	ThorUI::Draw();

	if (grid)
	{
		DrawGrid();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.7f, 0.7f, 0.7f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplSdlGL3_NewFrame(window);
	
	HandleInput();
	for (uint i = 0; i < docks.size(); ++i)
	{
		if (docks[i]->root) docks[i]->Draw();
	}
	DrawMainMenuBar();
	DrawIconBar();

	ImGui::Render();

	if (ThorUI::GetKeyState(SDL_SCANCODE_DELETE) == KEY_DOWN)
	{
		std::list<UI_Item*>::iterator it;
		for (it = hierarchy->selected.begin(); it != hierarchy->selected.end(); ++it)
		{
			ThorUI::DeleteItem((*it));

			std::vector<UI_Item*> to_remove;
			(*it)->CollectChildren(to_remove);
			to_remove.push_back((*it));

			for (uint i = 0; i < to_remove.size(); ++i)
				hierarchy->RemoveNode(to_remove[i]);
		}
		hierarchy->selected.clear();
	}
}

void UI_Editor::HandleInput()
{
	if (ThorUI::GetKeyState(SDL_SCANCODE_G) == KEY_DOWN)
	{
		grid = !grid;
	}
}

void UI_Editor::DrawMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				ThorUI::SaveScene("scene_save.thor");
			}
			if (ImGui::MenuItem("Load"))
			{
				ThorUI::LoadScene("scene_save.thor");
			}
			if (ImGui::MenuItem("New Scene"))
			{
				ThorUI::ClearScene();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Canvas"))
			{
				canvas_win = true;
			}
			if (ImGui::MenuItem("Grid"))
			{
				grid_win = true;
			}
			if (ImGui::MenuItem("Snap"))
			{
				snap_win = true;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::MenuItem("Button"))
			{
				UI_Button* button = new UI_Button(window_size / 2, Vec2(150, 60));
				button->SetName(ThorUI::GenUniqueName(ThorUI::window_item, "Button").c_str());
				ThorUI::AddItem(button);
				UI_Text* text = new UI_Text(Vec2(0, 0), Vec2(0, 0), "Button");
				text->SetParent(button, false);
				ThorUI::AddItem(text);

				hierarchy->AddNode(text);
				hierarchy->AddNode(button);
			}
			if (ImGui::MenuItem("Image"))
			{
				Vec2 image_size(100, 100);
				Vec2 image_pos = window_size / 2;

				UI_Image* image = new UI_Image(image_pos, image_size);
				image->SetName(ThorUI::GenUniqueName(ThorUI::window_item, "Image").c_str());
				ThorUI::AddItem(image);
				hierarchy->AddNode(image);
			}
			if (ImGui::MenuItem("Text"))
			{
				UI_Text* text = new UI_Text(window_size / 2, Vec2(0, 0), "New Text");
				text->SetName(ThorUI::GenUniqueName(ThorUI::window_item, "Text").c_str());
				ThorUI::AddItem(text);
				hierarchy->AddNode(text);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Dev Tools"))
		{
			if (ImGui::Checkbox("Enable", &dev_tools))
			{

			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();

		if (canvas_win)
			DrawCanvasWindow();
		if (grid_win)
			DrawGridWindow();
		if (snap_win)
			DrawSnapWindow();
	}
}

void UI_Editor::DrawIconBar()
{

}

void UI_Editor::DrawGrid()
{
	glEnable(GL_BLEND);
	glColor4f(0.5f, 0.5f, 0.5f, 0.2f);

	glBegin(GL_LINES);

	for (float i = 0; i < window_size.x; i += grid_separation.x)
	{
		glVertex2f(i, 0);
		glVertex2f(i, window_size.y);
	}
	for (float i = 0; i < window_size.y; i += grid_separation.y)
	{
		glVertex2f(0, i);
		glVertex2f(window_size.x, i);
	}

	glEnd();
	glDisable(GL_BLEND);
	glColor4f(1, 1, 1, 1);
}

void UI_Editor::DrawCanvasWindow()
{
	if (ImGui::Begin("Canvas_Popup", &canvas_win))
	{
		Vec2 size = ThorUI::window_item->GetSize();
		if (ImGui::InputFloat("Size X: ", &size.x))
		{
			ThorUI::SetCanvasSize(size);
		}
		if (ImGui::InputFloat("Size Y: ", &size.y))
		{
			ThorUI::SetCanvasSize(size);
		}
	}
	ImGui::End();
}

void UI_Editor::DrawGridWindow()
{
	if (ImGui::Begin("Grid Edit", &grid_win))
	{
		ImGui::Checkbox("Link axis", &block_grid);
		int divX = grid_div.x;
		if (ImGui::InputInt("Divisions X", &divX))
		{
			grid_div.x = divX;
			grid_separation.x = window_size.x / divX;
			if (block_grid)
			{
				grid_separation.y = grid_separation.x;
				grid_div.y = window_size.y / grid_separation.y;

			}

		}
		int divY = grid_div.y;
		if (ImGui::InputInt("Divisions Y", &divY))
		{
			grid_div.y = divY;
			grid_separation.y = window_size.y / divY;
			if (block_grid)
			{
				grid_separation.x = grid_separation.y;
				grid_div.x = window_size.x / grid_separation.x;
			}

		}
		ImGui::Separator();
		int sep_x = grid_separation.x;
		if (ImGui::InputInt("Pixel size X", &sep_x))
		{
			grid_separation.x = sep_x;
			grid_div.x = window_size.x / grid_separation.x;
			if (block_grid)
			{
				grid_separation.y = grid_separation.x;
				grid_div.y = window_size.y / grid_separation.y;
			}
		}

		int sep_y = grid_separation.y;
		if (ImGui::InputInt("Pixel size Y", &sep_y))
		{
			grid_separation.y = sep_y;
			grid_div.y = window_size.y / grid_separation.y;
			if (block_grid)
			{
				grid_div.x = window_size.x / grid_separation.x;
				grid_separation.x = grid_separation.y;
			}
		}
	}
	ImGui::End();
}

void UI_Editor::DrawSnapWindow()
{
	if (ImGui::Begin("Snap Edit", &snap_win))
	{
		ImGui::Text("Angle Interval");
		ImGui::InputInt("##angle_snap", &angle_interval);
		ImGui::Separator();

		ImGui::Text("Scale Interval");
		ImGui::InputFloat("##scale_snap", &scale_interval);
	}
	ImGui::End();
}

void UI_Editor::ProcessEvent(SDL_Event* event)
{
	switch (event->type)
	{
		case(SDL_WINDOWEVENT):
		{
			switch (event->window.event)
			{
				case(SDL_WINDOWEVENT_SIZE_CHANGED):
				{
					int w, h;
					SDL_GetWindowSize(window, &w, &h);
					window_size.Set(w, h);
					docks[0]->SetSize(window_size);
				}
			}
		}
	}
	ImGui_ImplSdlGL3_ProcessEvent(event);
}

bool UI_Editor::CleanUp()
{
	for (uint i = 0; i < docks.size(); ++i)
	{
		delete docks[i];
	}
	docks.clear();

	delete hierarchy;
	delete inspector;
	delete resources;
	delete scene;

	return true;
}

void UI_Editor::DisplayTexture(ThorUI::Texture* tex)
{
	ImVec2 window_size = ImGui::GetWindowSize();
	float padding = ImGui::GetStyle().WindowPadding.x;
	Vec2 size = tex->original_size.FitInRect(Vec2(window_size.x - padding * 2, 200));

	ImGui::Image((ImTextureID)tex->id, ImVec2(size.x, size.y));
	ImGui::Text("Original size: %i, %i", (int)tex->original_size.x, (int)tex->original_size.y);
}

void UI_Editor::DrawRect(Rect rect, ImU32 color, bool filled, float thickness)
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	if (filled)
		draw_list->AddRectFilled(ImVec2(rect.pos.x, window_size.y - rect.pos.y), ImVec2(rect.pos.x + rect.size.x, window_size.y - rect.pos.y - rect.size.y), color);
	else
		draw_list->AddRect(ImVec2(rect.pos.x, window_size.y - rect.pos.y), ImVec2(rect.pos.x + rect.size.x, window_size.y - rect.pos.y - rect.size.y), color, 0.0f, -1, thickness);
}

void UI_Editor::DrawTriangle(Vec2 a, Vec2 b, Vec2 c, ImU32 color)
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddTriangleFilled(ToImVec2(a), ToImVec2(b), ToImVec2(c), color);
}

void UI_Editor::DrawCircle(Circle circle, ImU32 color)
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddCircleFilled(ToImVec2(circle.pos), circle.radius, color, 64);
}

void UI_Editor::SetDockFocus(Dock* dock)
{
	if (dock_focus)
		dock_focus->focused = false;
	dock_focus = dock;
	if (dock_focus)
		dock_focus->focused = true;
}

ImVec2 UI_Editor::ToImVec2(Vec2 point)
{
	return ImVec2(point.x, window_size.y - point.y);
}

Vec2 UI_Editor::ToVec2(ImVec2 point)
{
	return Vec2(point.x, window_size.y - point.y);
}

Vec2 UI_Editor::GetClosestGridPoint(int x, int y)
{
	Vec2 ret(round(x / grid_separation.x), round(y / grid_separation.y));
	return ret * grid_separation;
}
