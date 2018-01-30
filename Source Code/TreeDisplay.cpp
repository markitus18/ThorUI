#include "TreeDisplay.h"
#include "ImGui\imgui.h"

template <typename T>
void TreeDisplay<T>::DrawTree()
{

}

template <typename T>
void TreeDisplay<T>::DrawNode(TreeNode<T>* node)
{

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

	if (node->beenSelected == true)
	{
		ImGui::SetNextTreeNodeOpen(true);
		node->beenSelected = false;
	}

	bool nodeOpen = ImGui::TreeNodeEx(node, flags, node->GetName());
	node->hierarchyOpen = node->GetChilds().empty() ? false : nodeOpen;

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