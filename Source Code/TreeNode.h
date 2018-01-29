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

	T* GetContainer() const { return container };
	TreeNode* GetParent() const;

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
