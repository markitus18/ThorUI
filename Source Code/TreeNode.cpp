#include "TreeNode.h"
#include "TreeDisplay.h"
#include "UI_Item.h"

template class TreeNode<UI_Item>;

template <typename T>
int TreeNode<T>::GetChildID(int index) const
{
	T* child = container->GetChild(index);
	return child ? child->GetID() : 0;
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
int TreeNode<T>::GetParentID() const
{
	T* parent = container->GetParent();
	return parent ? parent->GetID() : -1;
}