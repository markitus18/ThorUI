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
};
#endif
