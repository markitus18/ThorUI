#ifndef __TREE_DISPLAY_H__
#define __TRE__DISPLAY_H__

#include <list>
#include "TreeNode.h"

template <typename T>
class TreeDisplay
{
public:

	TreeNode<T>* GetNode(T* c)
	{
		if (!c) return nullptr;
		std::list<TreeNode<T>>::iterator it;
		for (it = nodes.begin(); it != nodes.end(); ++it)
		{
			if ((*it).GetContainer() == c)
				return &(*it);
		}
	}

	void AddNode(T* c)
	{
		//Checking if already added
		std::list<TreeNode<T>>::iterator it;
		for (it = nodes.begin(); it != nodes.end(); ++it)
		{
			if ((*it).GetContainer() == c)
				return;
		}
		nodes.push_back(TreeNode<T>(c, this));
	}

	void RemoveNode(T* node);

	void UnselectAll();
	void SelectSingle(T* node, bool openTree = true);

	void DrawTree();
	void DrawNode(TreeNode<T>* node);
	void DisplayNode(TreeNode<T>* node);
	void DrawNodeChilds(TreeNode<T>* node);

	void HandleUserInput(TreeNode<T>* node);
	void HandleArrows();

public:
	std::list<TreeNode<T>> nodes;
	std::list<TreeNode<T>*> selected;
	TreeNode<T>* last_selected = nullptr;
};

#endif
