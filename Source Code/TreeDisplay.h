#ifndef __TREE_DISPLAY_H__
#define __TRE__DISPLAY_H__

#include <vector>
#include "TreeNode.h"

template <typename T>
class TreeDisplay
{
public:
	TreeNode<T>* GetNode(T* c) const
	{
		std::vector<TreeNode<T>*>::iterator it;
		for (it = nodes.begin(); it != nodes.end(); ++it)
		{
			if ((*it)->GetContainer() == c)
				return *it;
		}
	}

public:
	std::vector<TreeNode<T>*> nodes;
};

#endif
