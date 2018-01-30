#ifndef __TREENODE_H__
#define __TREENODE_H__

template<typename T>
class TreeDisplay;

template<typename T>
class TreeNode
{
public:
	TreeNode(T* container) : container(container)
	{}

	const char* GetName() const { return container->GetName(); }
	bool IsActive() const { return container->IsActiveHierarchy(); }
	T* GetContainer() const { return container };

	int ChildCount() const { return container->ChildCount(); }
	TreeNode<T>* GetChild(int index) const;
	int GetChildIndex(const TreeNode<T>* child) const;
	TreeNode<T>* GetNextOpenNode() const;
	TreeNode<T>* GetPrevOpenNode() const;
	TreeNode<T>* GetParent() const;

	void Select() { selected = true; }
	void Unselect() { selected = false; }
	bool IsParentSelected() const;

public:
	bool beenSelected = false;
	bool hierarchyOpen = false;
	bool selected = false;

private:
	T* container;
	TreeDisplay<T>* tree;
};

#endif
