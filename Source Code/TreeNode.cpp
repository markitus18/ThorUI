#include "TreeNode.h"
#include "TreeDisplay.h"
#include "UI_Item.h"

template class TreeNode<UI_Item>;

template <typename T>
TreeNode<T>* TreeNode<T>::GetChild(int index) const
{
	if (tree == nullptr) return nullptr;
	return tree->GetNode(container->GetChild(index));
}

template <typename T>
int TreeNode<T>::GetChildIndex(const TreeNode<T>* child) const
{
	for (int i = 0; i < container->ChildCount(); ++i)
	{
		if (container->GetChild(i) == child->container)
			return i;
	}
	return -1;
}

template <typename T>
TreeNode<T>* TreeNode<T>::GetNextOpenNode() const
{
	if (container->ChildCount() > 0 && hierarchyOpen == true)
		return GetChild(0);

	TreeNode<T>* parent = GetParent();
	const TreeNode* toEvaluate = this;
	while (parent != nullptr)
	{
		if (parent->hierarchyOpen == true)
		{
			int childIndex = parent->GetChildIndex(toEvaluate);
			if (TreeNode<T>* ret = parent->GetChild(++childIndex))
				return ret;
		}
	}
	return nullptr;
}

template <typename T>
TreeNode<T>* TreeNode<T>::GetPrevOpenNode() const
{
	int childIndex = GetParent()->GetChildIndex(this);
	if (childIndex > 0)
	{
		TreeNode<T>* prev = GetParent()->GetChild(--childIndex);
		while (prev->hierarchyOpen == true)
			prev = prev->GetChild(prev->ChildCount() - 1);
		return prev;
	}
	else
	{
		return GetParent(); //TODO: don not return root parent!
	}
}

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