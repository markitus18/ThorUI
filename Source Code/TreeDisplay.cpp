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
	typename std::map<int, TreeNode<T>>::iterator it = nodes.find(node->GetID());
	if (it != nodes.end())
	{
		if (it->second.selected == true)
		{
			UnselectSingle(node);
		}
		nodes.erase(it);
	}
}

template <typename T>
void TreeDisplay<T>::UnselectAll()
{
	std::list<T*>::iterator it;
	for (it = selected.begin(); it != selected.end(); ++it)
	{
		if (TreeNode<T>* node = GetNode((*it)->GetID()))
		{
			node->Unselect();
		}
	}
	selected.clear();
}

template <typename T>
void TreeDisplay<T>::SelectSingle(T* c, bool openTree)
{
	UnselectAll();
	if (TreeNode<T>* node = GetNode(c->GetID()))
	{
		selected.push_back(node->GetContainer());
		last_selected = c;
		node->Select();

		if (openTree)
		{
			TreeNode<T>* it = GetNode(node->GetParentID());
			while (it != nullptr)
			{
				it->beenSelected = true;
				it = GetNode(it->GetParentID());
			}
		}
	}
}

template <typename T>
void TreeDisplay<T>::UnselectSingle(T* node)
{
	TreeNode<T>* n = GetNode(node->GetID());
	if (n != nullptr)
	{
		n->Unselect();
		typename std::list<T*>::iterator it;	//TODO: change if unordered_map
		for (it = selected.begin(); it != selected.end(); ++it)
		{
			if (*it == node)
			{
				selected.erase(it);
			}
		}
	}
}

template <typename T>
void TreeDisplay<T>::DrawTree()
{
	DrawNodeChilds(root);
	HandleArrows();
}

template <typename T>
void TreeDisplay<T>::DrawNode(TreeNode<T>& node)
{
	DisplayNode(node);
	HandleUserInput(node);

	if (ImGui::BeginPopup(node.GetName()))
	{
		if (ImGui::Button("delete"))
		{
			//delete node
		}
		ImGui::EndPopup();
	}

	//TODO: dragging

	if (node.hierarchyOpen == true)
	{
		DrawNodeChilds(node);
		ImGui::TreePop();
	}
}

template <typename T>
void TreeDisplay<T>::DisplayNode(TreeNode<T>& node)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
	if (node.ChildCount() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	if (node.selected == true) //TODO: check if it is "ToSelect"
		flags |= ImGuiTreeNodeFlags_Selected;

	//TODO: control dragging (line 106)

	if (node.IsActive() == false)
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 0.4));

	if (node.beenSelected == true && node.ChildCount() > 0)
	{
		ImGui::SetNextTreeNodeOpen(true);
		node.beenSelected = false;
	}

	bool nodeOpen = ImGui::TreeNodeEx(&node, flags, node.GetName());
	node.hierarchyOpen = node.ChildCount() == 0 ? false : nodeOpen;

	if (node.IsActive() == false)
		ImGui::PopStyleColor();
}

template <typename T>
void TreeDisplay<T>::DrawNodeChilds(TreeNode<T>& node)
{
	int childs = node.ChildCount();
	for (int i = 0; i < childs; ++i)
		DrawNode(*GetNode(node.GetChildID(i)));
}

template <typename T>
void TreeDisplay<T>::HandleUserInput(TreeNode<T>& node)
{
	//TODO: dragging
	//TODO: multiple selection
	if (ImGui::IsItemHovered())
	{
		if (ThorUI::GetMouseState(SDL_BUTTON_RIGHT) == KEY_DOWN)
		{
			ImGui::OpenPopup(node.GetName());
		}
		if (ThorUI::GetMouseState(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			SelectSingle(node.GetContainer());
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
			if (TreeNode<T>* next = GetNextOpenNode(*GetNode(last_selected->GetID())))
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
			if (TreeNode<T>* prev = GetPrevOpenNode(*GetNode(last_selected->GetID())))
			{
				//TODO: handle multiple selection
				SelectSingle(prev->GetContainer());
			}
		}
	}
}

template <typename T>
TreeNode<T>* TreeDisplay<T>::GetNextOpenNode(const TreeNode<T>& node)
{
	if (node.ChildCount() > 0 && node.hierarchyOpen == true)
		return GetNode(node.GetChildID(0));

	TreeNode<T>* parent = GetNode(node.GetParentID());
	const TreeNode<T>* toEvaluate = &node;
	while (parent != nullptr)
	{
		if (parent->hierarchyOpen == true)
		{
			int childIndex = parent->GetChildIndex(*toEvaluate);
			if (TreeNode<T>* ret = GetNode(parent->GetChildID(++childIndex)))
				return ret;
		}
		toEvaluate = parent;
		parent = GetNode(parent->GetParentID());
	}
	return nullptr;
}

template <typename T>
TreeNode<T>* TreeDisplay<T>::GetPrevOpenNode(const TreeNode<T>& node)
{
	TreeNode<T>* parent = GetNode(node.GetParentID());
	int childIndex = parent->GetChildIndex(node);
	if (childIndex > 0)
	{
		TreeNode<T>* prev = GetNode(parent->GetChildID(--childIndex));
		while (prev && prev->hierarchyOpen == true)
			prev = GetNode(prev->GetChildID(prev->ChildCount() - 1));
		return prev;
	}
	else
	{
		return parent != &root ? parent : nullptr;
	}
}

template <typename T>
bool TreeDisplay<T>::IsParentSelected(const TreeNode<T>& node)
{
	TreeNode<T>* parent = GetNode(node.GetParentID());
	return (parent ? (parent->selected ? true : IsParentSelected(*parent)) : false);
}