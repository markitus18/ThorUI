#ifndef __TREE_DISPLAY_H__
#define __TRE__DISPLAY_H__

#include <map>
#include <list>
#include "TreeNode.h"

template <typename T>
class TreeDisplay
{
public:

	TreeNode<T>* GetNode(int id)
	{
		if (id == root.GetID()) return &root;
		return nodes.find(id) != nodes.end() ? &nodes[id] : nullptr;
	}

	void SetRoot(T* c)
	{
		root = TreeNode<T>(c);
		root.hierarchyOpen = true;
	}

	void AddNode(T* c)
	{
		//Checking if already added
		if (nodes.find(c->GetID()) == nodes.end())
		{
			nodes[c->GetID()] = TreeNode<T>(c);
		}
	}

	void RemoveNode(T* node);

	void UnselectAll();
	void SelectSingle(T* node, bool openTree = true);
	void UnselectSingle(T* node);

	void DrawTree();
	void DrawNode(TreeNode<T>& node);
	void DisplayNode(TreeNode<T>& node);
	void DrawNodeChilds(TreeNode<T>& node);

	void HandleUserInput(TreeNode<T>& node); //TODO: change into more encapsulated system
	void HandleArrows();					//TODO

	TreeNode<T>* GetNextOpenNode(const TreeNode<T>& node);
	TreeNode<T>* GetPrevOpenNode(const TreeNode<T>& node);

	bool IsParentSelected(const TreeNode<T>& node);

public:
	TreeNode<T> root;
	std::map<int, TreeNode<T>> nodes;
	std::list<T*> selected; //TODO: add to unordered map?
	T* last_selected = nullptr;

private:
	bool selection_started = false;
};

#endif
