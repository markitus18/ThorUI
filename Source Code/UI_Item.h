#ifndef __UI_ITEM_H__
#define __UI_ITEM_H__

#include "Vec2.h"
#include <vector>

enum Item_Type
{
	Button,
	Text,
	Image,
};

enum Item_Event
{
	Mouse_Enter,
	Mouse_Down,
	//TODO: Add Mouse_Hold?
	Mouse_Up,
	Mouse_Exit,
};

struct SDL_Texture;
typedef unsigned int uint;

class UI_Item
{
public:
	UI_Item() {};
	UI_Item(float x, float y) : pos(x, y) { UpdateGlobalPos(); };
	UI_Item(Vec2 pos, Vec2 size) : pos(pos), size(size) { UpdateGlobalPos(); };

	void SetPos(float x, float y);
	void SetPos(Vec2 pos);
	void SetSize(float w, float h);
	void SetSize(Vec2 size);
	void SetName(const char* name);

	void SetParent(UI_Item* parent);
	void RemoveChild(UI_Item* child);

	void UpdateGlobalPos();

	virtual void Draw() {}; //TODO: draw here, or in ThorUI?
	virtual void OnItemEvent(Item_Event event) { last_event = event; };

	Vec2 GetPos() const;
	Vec2 GetSize() const;
	Item_Event GetLastEvent() const;
	UI_Item* GetParent() const;
	const char* GetName() const;
	uint GetChildCount() const;
	const std::vector<UI_Item*> GetChildren() const;
	Item_Type GetType() const;

protected:
	std::string name;
	Item_Type type;

	Vec2 pos;
	Vec2 global_pos;
	Vec2 size;

	Item_Event last_event = Mouse_Exit;

	UI_Item* parent = nullptr;
	std::vector<UI_Item*> children;
};

#endif
