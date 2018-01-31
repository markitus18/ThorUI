#include "TreeDisplay.h"
#include "ThorUI.h"
#include "UI_Item.h"

#include "ImGui\imgui.h"
#include "SDL2-2.0.6\include\SDL_mouse.h"
#include "SDL2-2.0.6\include\SDL_scancode.h"

template class TreeDisplay<UI_Item>;

template <typename T>
void TreeDisplay<T>::RemoveNode(T* node)
{
	std::list<TreeNode<T>>::iterator it;
	for (it = nodes.begin(); it != nodes.end(); ++it)
	{
		if ((*it).GetContainer() == node)
		{
			nodes.erase(it);
			break;
		}
	}
}

template <typename T>
void TreeDisplay<T>::UnselectAll()
{
	std::list<TreeNode<T>*>::iterator it;
	for (it = selected.begin(); it != selected.end(); ++it)
	{
		(*it)->Unselect();
	}
	selected.clear();
}


template <typename T>
void TreeDisplay<T>::SelectSingle(T* c, bool openTree)
{
	UnselectAll();
	TreeNode<T>* node = GetNode(c);
	if (node)
	{
		selected.push_back(node);
		node->Select();

		if (openTree)
		{
			TreeNode<T>* it = node->GetParent();
			while (it != nullptr)
			{
				it->beenSelected = true;
				it = it->GetParent();
			}
		}
	}
}

template <typename T>
void TreeDisplay<T>::DrawTree()
{
	if (nodes.size() > 0)
		DrawNodeChilds(&nodes.front());
	HandleArrows();
}

template <typename T>
void TreeDisplay<T>::DrawNode(TreeNode<T>* node)
{
	DisplayNode(node);
	HandleUserInput(node);

	if (ImGui::BeginPopup(node->GetName()))
	{
		if (ImGui::Button("delete"))
		{
			//delete node
		}
		ImGui::EndPopup();
	}

	//TODO: dragging

	if (node->hierarchyOpen == true)
	{
		DrawNodeChilds(node);
		ImGui::TreePop();
	}
}

template <typename T>
void TreeDisplay<T>::DisplayNode(TreeNode<T>* node)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
	if (node->ChildCount() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	if (node->selected == true) //TODO: check if it is "ToSelect"
		flags |= ImGuiTreeNodeFlags_Selected;

	//TODO: control dragging (line 106)

	if (node->IsActive() == false)
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 0.4));

	if (node->beenSelected == true && node->ChildCount() > 0)
	{
		ImGui::SetNextTreeNodeOpen(true);
		node->beenSelected = false;
	}

	bool nodeOpen = ImGui::TreeNodeEx(node, flags, node->GetName());
	node->hierarchyOpen = node->ChildCount() == 0 ? false : nodeOpen;

	if (node->IsActive() == false)
		ImGui::PopStyleColor();
}

template <typename T>
void TreeDisplay<T>::DrawNodeChilds(TreeNode<T>* node)
{
	int childs = node->ChildCount();
	for (int i = 0; i < childs; ++i)
		DrawNode(node->GetChild(i));
}

template <typename T>
void TreeDisplay<T>::HandleUserInput(TreeNode<T>* node)
{
	//TODO: dragging
	//TODO: multiple selection
	if (ImGui::IsItemHovered())
	{
		if (ThorUI::GetMouseState(SDL_BUTTON_RIGHT) == KEY_DOWN)
		{
			ImGui::OpenPopup(node->GetName());
		}
		if (ThorUI::GetMouseState(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			SelectSingle(node->GetContainer());
		}
	}
}

template <typename T>
void TreeDisplay<T>::HandleArrows()
{
	if (ThorUI::GetKeyState(SDL_SCANCODE_DOWN) == KEY_DOWN)
	{
		if (last_selected != nullptr)
		{
			if (TreeNode<T>* next = last_selected->GetNextOpenNode())
			{
				//TODO: handle multiple selection
				SelectSingle(next->GetContainer());
			}
		}
	}
	if (ThorUI::GetKeyState(SDL_SCANCODE_UP) == KEY_DOWN)
	{
		if (last_selected != nullptr)
		{
			if (TreeNode<T>* prev = last_selected->GetNextOpenNode())
			{
				//TODO: handle multiple selection
				SelectSingle(prev->GetContainer());
			}
		}
	}
}
