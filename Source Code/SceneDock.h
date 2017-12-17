#ifndef __SCENE_D_H__
#define __SCENE_D_H__

#include "DockData.h"
#include "Vec2.h"

typedef unsigned int uint;
class UI_Editor;

class Scene : public DockData
{
public:
	Scene(UI_Editor* editor);
	~Scene() {}

	void Draw();

private:
	void GenScreenBuffers();

public:
	uint frameBuffer = 0;
	uint renderTexture = 0;

	bool dragging = false;
	float x_start = -1;
	float y_start = 0;
	float x_item_start = 0;
	float y_item_start = 0;
};
#endif
