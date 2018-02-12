#include "TreeDisplay.h"
#include "ThorUI.h"
#include "UI_Item.h"

#include "ImGui\imgui.h"
#include "ImGui\imgui_internal.h"
#include "ImGui\imconfig.h"

#include "SDL2-2.0.6\include\SDL_mouse.h"
#include "SDL2-2.0.6\include\SDL_scancode.h"

#define IMGUI_DEFINE_MATH_OPERATORS

template class TreeDisplay<UI_Item>;

template <typename T>
void TreeDisplay<T>::RemoveNode(T* node)
{
	typename std::map<int, TreeNode<T>>::iterator it = nodes.find(node->GetID());
	if (it != nodes.end())
	{
		if (it->second.selected == true)
		{
			SetSelect(node, false);
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
void TreeDisplay<T>::SetSelect(T* c, bool select, bool single, bool openTree)
{
	if (TreeNode<T>* node = GetNode(c->GetID()))
	{
		if (select == true)
		{
			if (single == true)
			{
				UnselectAll();
				last_selected = c;
			}

			selected.push_back(node->Get());
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
		else
		{
			node->Unselect();
			typename std::list<T*>::iterator it;	//TODO: change if unordered_map
			for (it = selected.begin(); it != selected.end(); ++it)
			{
				if (*it == c)
				{
					selected.erase(it);
				}
			}
		}
	}
}

template <typename T>
void TreeDisplay<T>::DrawTree()
{
	if (selection_started == true && ThorUI::mouse_dragging == true)
	{
		dragging = true;
	}
	if (selection_started == true && dragging == false && (ThorUI::GetMouseState(SDL_BUTTON_LEFT) == KEY_IDLE || ThorUI::GetMouseState(SDL_BUTTON_LEFT) == KEY_UP))
	{
		FinishSelection(false, false);
	}
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

	if (selection_started == true && dragging == true)
	{
		//Drawing inter-item buttons
		ImVec2 cursor_pos = ImGui::GetCursorPos();
		cursor_pos.y -= 5.0f;
		ImGui::SetCursorPos(cursor_pos);
		ImGui::PushID(node.GetID());
		ImVec2 button_size = ImVec2(ImGui::GetWindowSize().x, 6);
		ImGui::InvisibleButton("Button", button_size);
		if (ImGui::IsItemHoveredRect())
		{
			ImVec2 g_cursor_pos = ImGui::GetCursorScreenPos();
			g_cursor_pos.y -= 10.0f;
			ImGui::RenderFrame(ImVec2(g_cursor_pos), ImVec2(g_cursor_pos.x + ImGui::GetWindowSize().x, g_cursor_pos.y + 6), ImGui::GetColorU32(ImGuiCol_TitleBgActive)); //
			ImGui::GetCurrentWindow()->DrawList->AddRect(ImVec2(g_cursor_pos), ImVec2(g_cursor_pos.x + ImGui::GetWindowSize().x, g_cursor_pos.y + 6), ImGui::GetColorU32(ImGuiCol_TextSelectedBg));
			if (ThorUI::GetMouseState(SDL_BUTTON_LEFT) == KEY_IDLE || ThorUI::GetMouseState(SDL_BUTTON_LEFT) == KEY_UP)
			{
				TreeNode<T>* parent = node.hierarchyOpen ? &node : GetNode(node.GetParentID());
				SetParentByPlace(*parent, to_drag, GetNextVisibleNode(node));
				FinishSelection(true, true);
			}
		}
		cursor_pos = ImGui::GetCursorPos();
		cursor_pos.y -= 5.0f;
		ImGui::SetCursorPos(cursor_pos);
		ImGui::PopID();
	}

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
	if (IsNodeHighlighted(node) == true)
		flags |= ImGuiTreeNodeFlags_Selected;
	if (selection_started == true && dragging == true)
		flags |= ImGuiTreeNodeFlags_Fill;
	if (node.IsActive() == false)
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 0.4));

	if (node.beenSelected == true && node.ChildCount() > 0)
	{
		ImGui::SetNextTreeNodeOpen(true);
		node.beenSelected = false;
	}

	bool nodeOpen = ImGui::TreeNodeEx(&node, flags, node.GetName());
	node.hierarchyOpen = (node.ChildCount() == 0 ? false : nodeOpen);

	if (node.IsActive() == false)
		ImGui::PopStyleColor();
}

template <typename T>
void TreeDisplay<T>::DrawNodeChilds(TreeNode<T>& node)
{
	for (int i = 0; i < node.ChildCount(); ++i)
		DrawNode(*GetNode(node.GetChildID(i)));
}

template <typename T>
void TreeDisplay<T>::HandleUserInput(TreeNode<T>& node)
{
	if (ImGui::IsItemHovered())
	{
		if ((ThorUI::GetMouseState(SDL_BUTTON_LEFT) == KEY_UP || ThorUI::GetMouseState(SDL_BUTTON_LEFT) == KEY_IDLE) && selection_started == true && dragging == true)
		{
			if (to_drag.size() > 0)
			{
				bool parent_set = SetParentByPlace(node, to_drag);
				if (parent_set)
					node.beenSelected = true;

				FinishSelection(true, true);
			}
		}
		if (ThorUI::GetMouseState(SDL_BUTTON_RIGHT) == KEY_DOWN)
		{
			ImGui::OpenPopup(node.GetName());
		}
		if (ThorUI::GetMouseState(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			selection_started = true;
			last_selected = node.Get();

			//Pressing Ctrl: add single selection
			if (ThorUI::GetKeyState(SDL_SCANCODE_LCTRL) == KEY_REPEAT || ThorUI::GetKeyState(SDL_SCANCODE_RCTRL) == KEY_REPEAT)
			{
				to_drag.push_back(&node);
				last_selected = node.Get();
				node.selected == true ? SetSelect(node.Get(), false) : to_select.push_back(&node); //TODO: add to unselect?

				for (std::list<T*>::iterator it = selected.begin(); it != selected.end(); ++it)
				{
					to_drag.push_back(GetNode((*it)->GetID()));
				}
			}
			//Pressing shift: multiple seletion
			else if (ThorUI::GetKeyState(SDL_SCANCODE_LSHIFT) == KEY_REPEAT || ThorUI::GetKeyState(SDL_SCANCODE_RSHIFT) == KEY_REPEAT)
			{
				if (selected.size() == 0)
				{
					to_select.push_back(&node);
					to_drag.push_back(&node);
				}
				else
				{
					ShiftSelection(node, !node.selected);
				}
			}
			//Normal single selection
			else
			{
				for (std::list<T*>::iterator it = selected.begin(); it != selected.end(); ++it)
				{
					if ((*it)->GetID() != node.GetID())
						to_unselect.push_back(GetNode((*it)->GetID()));
					if (node.selected == true)
						to_drag.push_back(GetNode((*it)->GetID()));
				}
				if (node.selected == false)
				{
					to_select.push_back(&node);
					to_drag.push_back(&node); //If selected = true it will be added to drag in the first loop
				}
			}
		}
	}
}

template <typename T>
void TreeDisplay<T>::HandleArrows()
{
	if (ThorUI::GetKeyState(SDL_SCANCODE_DOWN) == KEY_DOWN && last_selected != nullptr)
	{
		if (TreeNode<T>* next = GetNextVisibleNode(*GetNode(last_selected->GetID())))
		{
			if (ThorUI::GetKeyState(SDL_SCANCODE_LSHIFT) == KEY_REPEAT || ThorUI::GetKeyState(SDL_SCANCODE_RSHIFT) == KEY_REPEAT ||
				ThorUI::GetKeyState(SDL_SCANCODE_LCTRL) == KEY_REPEAT || ThorUI::GetKeyState(SDL_SCANCODE_RCTRL) == KEY_REPEAT)
			{
				TreeNode<T>* prev = GetPrevVisibleNode(*GetNode(last_selected->GetID()));
				if ((prev != nullptr && prev->selected == true) || next->selected == false)
				{
					SetSelect(next->Get(), true, false);
				}
				else
				{
					SetSelect(last_selected, false);
				}
			}
			else
			{
				SetSelect(next->Get(), true, true, true);
			}
			last_selected = next->Get();
		}
	}
	if (ThorUI::GetKeyState(SDL_SCANCODE_UP) == KEY_DOWN && last_selected != nullptr)
	{
		if (TreeNode<T>* prev = GetPrevVisibleNode(*GetNode(last_selected->GetID())))
		{
			if (ThorUI::GetKeyState(SDL_SCANCODE_LSHIFT) == KEY_REPEAT || ThorUI::GetKeyState(SDL_SCANCODE_RSHIFT) == KEY_REPEAT ||
				ThorUI::GetKeyState(SDL_SCANCODE_LCTRL) == KEY_REPEAT || ThorUI::GetKeyState(SDL_SCANCODE_RCTRL) == KEY_REPEAT)
			{
				TreeNode<T>* next = GetNextVisibleNode(*GetNode(last_selected->GetID()));
				if ((next != nullptr && next->selected == true) || prev->selected == false)
				{
					SetSelect(prev->Get(), true, false);
				}
				else
				{
					SetSelect(last_selected, false);
				}
			}
			else
			{
				SetSelect(prev->Get(), true, true, true);
			}
			last_selected = prev->Get();
		}
	}
}

template <typename T>
void TreeDisplay<T>::ShiftSelection(TreeNode<T>& node, bool select)
{
	if (last_selected->GetID() != node.GetID())
	{
		TreeNode<T>* next = GetFirstVisibleNode(*GetNode(last_selected->GetID()), node); //The starting node
		TreeNode<T>* last = (&node == next ? GetNode(last_selected->GetID()) : &node); //The finishing node

		while (next != nullptr)
		{
			if (select == true)
			{
				if (next->selected == false)
				{
					to_select.push_back(next);
					to_drag.push_back(next);
				}
			}
			else if (next != &node)
			{
				SetSelect(next->Get(), false);
			}
			if (next == last)
			{
				last_selected = node.Get();
				break;
			}
			next = GetNextVisibleNode(*next);
		}
	}
	for (std::list<T*>::iterator it = selected.begin(); it != selected.end(); ++it)
		to_drag.push_back(GetNode((*it)->GetID()));
}

template <typename T>
void TreeDisplay<T>::FinishSelection(bool dragging, bool select_dragged)
{
	std::vector<TreeNode<T>*>::iterator it;
	if (dragging == true)
	{
		if (select_dragged)
		{
			for (it = to_drag.begin(); it != to_drag.end(); ++it)
			{
				SetSelect((*it)->Get(), true);
			}
		}
	}
	else
	{
		for (it = to_select.begin(); it != to_select.end(); ++it)
		{
			SetSelect((*it)->Get(), true, false);
		}
	}
	for (it = to_unselect.begin(); it != to_unselect.end(); ++it)
	{
		SetSelect((*it)->Get(), false);
	}

	to_select.clear();
	to_unselect.clear();
	to_drag.clear();
	selection_started = false;
	this->dragging = false;
}

template <typename T>
TreeNode<T>* TreeDisplay<T>::GetFirstVisibleNode(TreeNode<T>& n1, TreeNode<T>& n2)
{
	if (n1.Get() == n2.Get()) return &n1;

	TreeNode<T>* to_evaluate = GetNextVisibleNode(root);
	while (to_evaluate != nullptr)
	{
		if (n1.GetID() == to_evaluate->GetID()) return &n1;
		if (n2.GetID() == to_evaluate->GetID()) return &n2;
		to_evaluate = GetNextVisibleNode(*to_evaluate);
	}
	return nullptr;
}

template <typename T>
typename std::vector<TreeNode<T>*>::iterator TreeDisplay<T>::GetFirstVisibleNode(std::vector<TreeNode<T>*>& nodes)
{
	TreeNode<T>* to_evaluate = GetNextVisibleNode(root);
	while (to_evaluate != nullptr)
	{
		std::vector<TreeNode<T>*>::iterator it;
		for (it = nodes.begin(); it != nodes.end(); ++it)
		{
			if ((*it)->GetID() == to_evaluate->GetID())
				return it;
		}
		to_evaluate = GetNextVisibleNode(*to_evaluate);
	}
	return nodes.end();
}

template <typename T>
TreeNode<T>* TreeDisplay<T>::GetNextVisibleNode(const TreeNode<T>& node)
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
TreeNode<T>* TreeDisplay<T>::GetPrevVisibleNode(const TreeNode<T>& node)
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

template <typename T>
bool TreeDisplay<T>::IsNodeHighlighted(const TreeNode<T>& node)
{
	if (node.selected == true) return true;

	for (std::vector<TreeNode<T>*>::iterator it = to_select.begin(); it != to_select.end(); ++it)
	{
		if ((*it)->GetID() == node.GetID())
			return true;
	}
	return false;
}

template <typename T>
bool TreeDisplay<T>::ExistsInChildTree(const TreeNode<T>& parent, const TreeNode<T>& node)
{
	int max_childs = parent.ChildCount();
	for (int i = 0; i < max_childs; ++i)
	{
		if (GetNode(parent.GetChildID(i))->GetID() == node.GetID())
			return true;
	}
	for (int i = 0; i < max_childs; ++i)
	{
		if (ExistsInChildTree(*GetNode(parent.GetChildID(i)), node))
			return true;
	}
	return false;
}

template <typename T>
bool TreeDisplay<T>::SetParentByPlace(TreeNode<T>& parent, std::vector<TreeNode<T>*>& children, TreeNode<T>* next)
{
	bool ret = false;
	bool prev_parent_hierarchy_open = parent.hierarchyOpen;
	parent.hierarchyOpen = true; //Little trick to add multiple childs
	std::vector<TreeNode<T>*>::iterator it = GetFirstVisibleNode(children);
	while (it != children.end())
	{
		if ((!next || next->GetID() != (*it)->GetID()) && parent.GetID() != (*it)->GetID() && ExistsInChildTree(*(*it), parent) == false)
		{
			(*it)->SetParent(parent, next);
			ret = true;
		}
		children.erase(it);
		it = GetFirstVisibleNode(children);
	}
	parent.hierarchyOpen = prev_parent_hierarchy_open;
	//TODO: recalculate open nodes
	return ret;
}