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
		if (!c) return nullptr;
		std::vector<TreeNode<T>*>::iterator it;
		for (it = nodes.begin(); it != nodes.end(); ++it)
		{
			if ((*it)->GetContainer() == c)
				return *it;
		}
	}

	void AddNode(T* c) const
	{
		//Checking if already added
		for (int i = 0; i < nodes.size(); ++i)
		{
			if (nodes[i].GetContainer() == c)
				return;
		}
		nodes.push_back(TreeNode<T>(c));
	}

	void DrawTree();
	void DrawNode(TreeNode<T>* node);
	void DisplayNode(TreeNode<T>* node);
	void DrawNodeChilds(TreeNode<T>* node);

private:
	std::vector<TreeNode<T>> nodes;
};

#endif
