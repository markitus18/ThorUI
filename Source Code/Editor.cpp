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

	ImGuiIO io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	//TODO: load own font

	ThorUI::Init(window);
	Dock* dock = new Dock("0 Dock");
	dock->separation = HORIZONTAL;
	docks.push_back(dock);

	/*
	for (uint i = 0; i < 5; ++i)
	{
		char name[50];
		sprintf_s(name, "%i Dock", i+1);
		Dock* next = new Dock(name);
		dock->AddChild(next);
		docks.push_back(next);
		int k = 1;
	}
	*/

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
	ThorUI::Draw();
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
				text->SetParent(button);
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

		DrawHierarchy();
		DrawInspector();
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

void UI_Editor::DrawHierarchy()
{
	if (!ImGui::Begin("Hierarchy"))
	{
		ImGui::End();
		return;
	}

	DrawHierarchyChilds(ThorUI::window_item);

	ImGui::End();
}

void UI_Editor::DrawHierarchyNode(UI_Item* item)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
	if (item->GetChildCount() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	if (item == selected)
		flags |= ImGuiTreeNodeFlags_Selected;
	bool node_open = ImGui::TreeNodeEx(item, flags, item->GetName());
	if (ImGui::IsItemHoveredRect() && ThorUI::GetMouseState(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		selected = item;
	}

	if (node_open == true)
	{
		if (item->GetChildCount() > 0)
		{
			DrawHierarchyChilds(item);
			ImGui::TreePop();
		}
	}
}

void UI_Editor::DrawHierarchyChilds(UI_Item* item)
{
	const std::vector<UI_Item*> children = item->GetChildren();
	for (std::vector<UI_Item*>::const_iterator it = children.begin(); it != children.end(); ++it)
	{
		DrawHierarchyNode(*it);
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
		char name[50];
		strcpy_s(name, 50, item->GetName());
		ImGuiInputTextFlags name_input_flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;
		if (ImGui::InputText("###", name, 50, name_input_flags))
			item->SetName(name);
		ImGui::Separator();

		Vec2 pos = item->GetPos();
		if (ImGui::DragFloat2("Position", &pos))
		{
			item->SetPos(pos);
		}

		Vec2 size = item->GetSize();
		if (ImGui::DragFloat2("Size", &size))
		{
			item->SetSize(size);
		}

		Vec2 pivot = item->GetPivot();
		if (ImGui::DragFloat2("Pivot", &pivot, 0.03f))
		{
			item->SetPivot(pivot);
		}

		ImGui::Separator();

		switch (item->GetType())
		{
			case(Image):
			{
				UI_Image* img = (UI_Image*)item;
				DrawInspectorImage((UI_Image*)item);
				break;
			}
			case(Text):
			{
				UI_Text* txt = (UI_Text*)item;
				DrawInspectorText((UI_Text*)item);
				break;
			}

		}
	}
}

void UI_Editor::DrawInspectorImage(UI_Image* img)
{
	ImGui::Text("Source Image");
	if (img->GetTexID() != 0)
	{
		DisplayTexture(ThorUI::GetTexture(img->GetTexID()));
	}
	if (ImGui::BeginMenu_ThorUI("Set Texture: "))
	{
		std::map<uint, ThorUI::Texture>::iterator it;
		for (it = ThorUI::textures.begin(); it != ThorUI::textures.end(); ++it)
		{
			if (ImGui::MenuItem((*it).second.path.c_str()))
			{
				img->SetTexture((*it).second.id);
			}
		}
		if (ImGui::MenuItem("Load New Texture..."))
		{
			std::string fileName = OpenFileDialog();
			if (fileName != "")
			{
				img->SetTexture(ThorUI::LoadTexture(fileName.c_str()));
			}
		}
		ImGui::EndMenu();
	}
	Color color = img->GetColor();
	if (ImGui::ColorEdit3("Color", color.ptr()))
	{
		img->SetColor(color);
	}
}

void UI_Editor::DrawInspectorText(UI_Text* text)
{
	char orig_text[50];
	strcpy_s(orig_text, 50, text->GetText());

	if (ImGui::InputText("Text", orig_text, 50))
	{
		text->SetText(orig_text);
	}

	Color color = text->GetColor();
	if (ImGui::ColorEdit3("Color", color.ptr()))
	{
		text->SetColor(color);
	}
}

void UI_Editor::DrawInspectorButton(UI_Button* button)
{

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