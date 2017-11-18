#include "Editor.h"

#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl_gl3.h"
#include "glew-2.1.0\include\GL\glew.h"
#include "SDL2-2.0.6\include\SDL.h"

#include <Windows.h>
#include <ShObjIdl.h>
#include <comdef.h>

#include "ThorUI.h"
#include "UI_Image.h"
#include "UI_Button.h"
#include "UI_Text.h"

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

	ThorUI::Init(window);

	Dock* dock = new Dock("0 Dock", Vec2(window_size.x, window_size.y - menubar_size));
	docks.push_back(dock);
	dock->position = Vec2(0, menubar_size);
	dock->Split(HORIZONTAL, 0.8f);
	dock->GetDockChildren()[0]->Split(VERTICAL, 0.8f);
	dock->GetDockChildren()[0]->GetDockChildren()[0]->Split(VERTICAL, 0.2f);
	
	hierarchy = new Hierarchy(this);
	dock->GetDockChildren()[0]->GetDockChildren()[0]->GetDockChildren()[0]->AddChildData(hierarchy);

	scene = new Scene(this);
	dock->GetDockChildren()[0]->GetDockChildren()[0]->GetDockChildren()[1]->AddChildData(scene);
	scene->scene_size = Vec2(ThorUI::screen_size.x, ThorUI::screen_size.y);

	inspector = new Inspector(this);
	dock->GetDockChildren()[0]->GetDockChildren()[1]->AddChildData(inspector);

	resources = new Resources(this);
	dock->GetDockChildren()[1]->AddChildData(resources);

	//Generating buffers for scene render
	glGenFramebuffers(1, &scene->frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, scene->frameBuffer);

	//Generating texture to render to
	glGenTextures(1, &scene->renderTexture);
	glBindTexture(GL_TEXTURE_2D, scene->renderTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ThorUI::screen_size.x, ThorUI::screen_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	//Configuring frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, scene->renderTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		int k = 1;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return ret;
}

void UI_Editor::Draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, scene->frameBuffer);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	ThorUI::Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplSdlGL3_NewFrame(window);
	
	for (uint i = 0; i < docks.size(); ++i)
	{
		if (docks[i]->root) docks[i]->Draw();
	}
	
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
				selected = nullptr;
			}
			if (ImGui::MenuItem("New Scene"))
			{
				ThorUI::ClearScene();
				selected = nullptr;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Canvas"))
			{
				canvas_win = true;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::MenuItem("Button"))
			{
				UI_Button* button = new UI_Button(window_size / 2, Vec2(150, 60));
				ThorUI::AddItem(button);

				UI_Text* text = new UI_Text(Vec2(0, 0), Vec2(0, 0), "Button");
				text->SetParent(button, false);
				ThorUI::AddItem(text);
			}
			if (ImGui::MenuItem("Image"))
			{
				Vec2 image_size(100, 100);
				Vec2 image_pos = window_size / 2;
				
				UI_Image* image = new UI_Image(image_pos, image_size);
				ThorUI::AddItem(image);
			}
			if (ImGui::MenuItem("Text"))
			{
				UI_Text* text = new UI_Text(Vec2(0, 0), Vec2(0, 0), "New Text");
				Vec2 text_pos = window_size / 2 - Vec2(text->GetSize().x / 2, text->GetSize().y / 2);
				text->SetPos(text_pos);
				ThorUI::AddItem(text);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();

		if (canvas_win && ImGui::Begin("Canvas_Popup", &canvas_win))
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
			ImGui::End();
		}
	}
	
	ImGui::Render();
}

void UI_Editor::ProcessEvent(SDL_Event* event)
{
	ImGui_ImplSdlGL3_ProcessEvent(event);
}

bool UI_Editor::CleanUp()
{
	return true;
}

void UI_Editor::DisplayTexture(ThorUI::Texture* tex)
{
	ImVec2 window_size = ImGui::GetWindowSize();
	float padding = ImGui::GetStyle().WindowPadding.x;
	float max_h = 200;
	Vec2 size = tex->original_size;

	if (size.x > (window_size.x - padding * 2))
	{
		size.y /= (size.x / (window_size.x - padding * 2));
		size.x /= (size.x / (window_size.x - padding * 2));
	}
	if (size.y > max_h)
	{
		size.x /= (size.y / max_h);
		size.y /= (size.y / max_h);
	}
	ImGui::Image((ImTextureID)tex->id, ImVec2(size.x, size.y));
	ImGui::Text("Original size: %i, %i", (int)tex->original_size.x, (int)tex->original_size.y);
}

std::string UI_Editor::OpenFileDialog() const
{
	std::string result = "";
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileOpenDialog *pFileOpen;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		if (SUCCEEDED(hr))
		{
			hr = pFileOpen->Show(NULL);

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				IShellItem *pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					wchar_t* pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
						result = _bstr_t(pszFilePath);
						char* base_path = SDL_GetBasePath();
						result = FileSystem::GetRelativePath(base_path, result.c_str());
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}
	return result;
}