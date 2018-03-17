#ifndef __SCENE_D_H__
#define __SCENE_D_H__

#include "DockData.h"
#include "Vec2.h"

#include "Shape.h"
#include "Rect.h"
#include "Circle.h"

#include "ImGui\imgui.h" //TMP

typedef unsigned int uint;
class UI_Editor;

enum class Drag_Type
{
	NONE,
	X,
	Y,
	XY,
};

enum class Gizmo_Op
{
	TRANSLATION,
	ROTATION,
	SCALE,
};

class Scene : public DockData
{
public:
	Scene(UI_Editor* editor);
	~Scene() {}

	void Draw();
	void OnResize();

	Vec2 ScreenToWorld(Vec2 p) const;
	Vec2 WorldToScreen(Vec2 p) const;

private:
	void GenScreenBuffers();

	void HandleInput();

	//Gizmos ---------------------
	void HandleGizmoActivation(Vec2 mouse_pos);
	void HandleDrag(Vec2 mouse_pos, Vec2 image_size);
	
	void SetGizmoOp(Gizmo_Op op);

	void DrawTranslationGizmo();
	void DrawScaleGizmo();
	void DrawRotationGizmo();
	//----------------------------
private:
	Vec2 win_size;
	Vec2 img_size;
	Vec2 img_corner;
	ImVec2 img_offset;

	Rect trans_buttons[3];
	Circle rot_button;

public:
	uint frameBuffer = 0;
	uint renderTexture = 0;

	Gizmo_Op gizmo_op = Gizmo_Op::TRANSLATION;
	Drag_Type drag = Drag_Type::NONE;

	Vec2 start_drag;
	Vec2 init_drag_val;
};
#endif
