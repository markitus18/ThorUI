#ifndef __TREENODE_H__
#define __TREENODE_H__

template<typename T>
class TreeDisplay;

template<typename T>
class TreeNode
{
public:
	TreeNode(){}
	TreeNode(T* container) : container(container)
	{}

	inline const char* GetName() const { return container->GetName(); }
	inline int GetID() const { return container->GetID(); }
	inline bool IsActive() const { return container->IsActiveHierarchy(); }
	inline T* Get() const { return container; }

	inline int ChildCount() const { return container->ChildCount(); }
	int GetChildID(int index) const;
	int GetChildIndex(const TreeNode<T>& child) const;
	int GetParentID() const;

	inline void Select() { selected = true; };
	inline void Unselect() { selected = false;}

	void SetParent(TreeNode<T>& parent, TreeNode<T>* next_child = nullptr);

public:
	bool beenSelected = false;
	bool hierarchyOpen = false;
	bool selected = false;

private:
	T* container;
};

#endif
