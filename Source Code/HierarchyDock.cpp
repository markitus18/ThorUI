#include "HierarchyDock.h"

#include "ImGui\imgui.h"
#include "UI_Item.h"
#include "ThorUI.h"
#include "Editor.h"

#include "SDL2-2.0.6\include\SDL.h"

void Hierarchy::Draw()
{
	DrawTree();
}