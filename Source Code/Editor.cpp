#include "Editor.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl_gl3.h"
#include "glew-2.1.0\include\GL\glew.h"
#include "ThorUI.h"
#include "UI_Image.h"
#include "SDL2-2.0.6\include\SDL.h"
#include <Windows.h>
#include <ShObjIdl.h>
#include <comdef.h>

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
				Vec2 image_size(100, 100);
				Vec2 image_pos = window_size / 2 - image_size / 2;
				
				UI_Image* image = new UI_Image(image_pos, image_size, 0);
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
				if (ImGui::BeginMenu("Change Texture: "))
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
						std::string fileName = OpenFileDialog2();
						if (fileName != "")
						{
							img->SetTexture(ThorUI::LoadTexture(fileName.c_str()));
						}
					}
					ImGui::EndMenu();
				}
			}
		}
	}
}

std::string UI_Editor::OpenFileDialog() const
{
	char fileName[1024];
	ZeroMemory(&fileName, sizeof(fileName));

	OPENFILENAME oFileName;
	ZeroMemory(&oFileName, sizeof(oFileName));
	oFileName.lStructSize = sizeof(oFileName);
	oFileName.lpstrFile = fileName;
	oFileName.nMaxFile = 1024;
	oFileName.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	oFileName.lpstrTitle = "Select file to import";

	return GetOpenFileName(&oFileName) != 0 ? std::string(fileName) : std::string("");
}

std::string UI_Editor::OpenFileDialog2() const
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