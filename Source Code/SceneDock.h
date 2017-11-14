#ifndef __SCENE_D_H__
#define __SCENE_D_H__

#include "DockData.h"
#include "Vec2.h"

typedef unsigned int uint;
class UI_Editor;

class Scene : public DockData
{
public:
	Scene(UI_Editor* editor) : DockData(editor) { name = "Scene"; }
	~Scene() {}

	void Draw();

public:
	uint tex_id = 0;
	Vec2 scene_size;
};
#endif
