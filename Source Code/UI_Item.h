#ifndef __UI_ITEM_H__
#define __UI_ITEM_H__

#include "Vec2.h"
#include "Rect.h"
#include "Transform.h"

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
class Config;

class UI_Item
{
public:
	UI_Item();
	UI_Item(float x, float y);
	UI_Item(Vec2 pos, Vec2 size);
	~UI_Item();

	void SetPos(float x, float y);
	void SetPos(Vec2 pos);
	void SetSize(float w, float h);
	virtual void SetSize(Vec2 size);
	void SetScale(float x, float y);
	void SetScale(Vec2 scale);
	void SetRotation(float rotation);
	void SetPivot(Vec2 pivot);

	void SetID(int id);
	void SetName(const char* name);
	void SetActive(bool active);

	void SetParent(UI_Item* parent, bool keep_global = true);
	void RemoveChild(UI_Item* child);
	void DeleteChildren();

	UI_Item* GetParent() const;
	UI_Item* GetChild(uint index) const;
	uint ChildCount() const;
	void CollectChildren(std::vector<UI_Item*>& vector);

	virtual void Draw() {}; //TODO: draw here, or in ThorUI?
	virtual void OnItemEvent(Item_Event event) { last_event = event; };

	void Save(Config& config);
	void Load(Config& config);

	Vec2 GetPos() const;
	Vec2 GetGlobalPos() const;
	Vec2 GetSize() const;
	Vec2 GetScale() const;
	Vec2 GetPivot() const;
	Transform* GetTransform();

	int GetID() const;

	Item_Event GetLastEvent() const;
	const char* GetName() const;
	Item_Type GetType() const;

	bool IsActive() const;
	bool IsActiveHierarchy() const;

protected:
	virtual void InternalSave(Config& config) = 0;
	virtual void InternalLoad(Config& config) = 0;

private:
	void SetHierarchyActive(bool active);

protected:
	std::string name;
	Item_Type type;

	Item_Event last_event = Mouse_Exit;

	bool active = true;
	bool hierarchyActive = true;

	int id = -1;

	Vec2 size;
	Vec2 pivot;
	Transform transform;

};

#endif
