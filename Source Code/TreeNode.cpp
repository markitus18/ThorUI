#include "TreeNode.h"
#include "TreeDisplay.h"

template <typename T>
TreeNode<T>* TreeNode<T>::GetParent() const
{
	if (tree == nullptr) return nullptr;
	return tree->GetNode(container->GetParent());
}

template <typename T>
bool TreeNode<T>::IsParentSelected() const
{
	TreeNode<T>* parent = GetParent();
	return (parent ? (parent->selected ? true : parent->IsParentSelected()) : false);
}