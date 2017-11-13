#include "DockData.h"

#include "Dock.h"

DockData::DockData()
{

}

DockData::~DockData()
{

}

void DockData::Draw()
{

}

void DockData::SetParent(Dock* dock)
{
	if (parent != nullptr)
	{
		parent->RemoveChildData(this);
	}
	parent = dock;
}

void DockData::SetActive(bool active)
{
	this->active = active;
}

Dock* DockData::GetParent() const
{
	return parent;
}

bool DockData::IsActive() const
{
	return active;
}