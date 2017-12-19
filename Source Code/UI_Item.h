#ifndef __UI_ITEM_H__
#define __UI_ITEM_H__

#include "Vec2.h"
#include "Rect.h"

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
	UI_Item() { global_rect.pivot.Set(0.5, 0.5); rect.pivot.Set(0.5, 0.5); };
	UI_Item(float x, float y) : rect(Vec2(x, y), Vec2::zero()) { global_rect.pivot.Set(0.5, 0.5); rect.pivot.Set(0.5, 0.5); UpdateGlobalTransform(); };
	UI_Item(Vec2 pos, Vec2 size) : rect(pos, size) { global_rect.pivot.Set(0.5, 0.5); rect.pivot.Set(0.5, 0.5); UpdateGlobalTransform(); };
	~UI_Item();

	void SetPos(float x, float y);
	void SetPos(Vec2 pos);
	void SetGlobalPos(float x, float y);
	void SetGlobalPos(Vec2 pos);
	void SetSize(float w, float h);
	virtual void SetSize(Vec2 size);
	void SetScale(float x, float y);
	void SetScale(Vec2 scale);
	void SetGlobalScale(Vec2 scale);
	void SetRotation(float rotation);

	void SetID(int id);
	void SetName(const char* name);
	void SetPivot(Vec2 pivot);
	void SetActive(bool active);
	void SetParent(UI_Item* parent, bool keep_global = true);
	void RemoveChild(UI_Item* child);
	void DeleteChildren();

	void UpdateGlobalTransform();

	virtual void Draw() {}; //TODO: draw here, or in ThorUI?
	virtual void OnItemEvent(Item_Event event) { last_event = event; };

	void Save(Config& config);
	void Load(Config& config);

	Vec2 GetPos() const;
	Vec2 GetGlobalPos() const;
	Vec2 GetSize() const;
	Vec2 GetScale() const;
	Vec2 GetGlobalScale() const;
	Vec2 GetPivot() const;
	Rect& GetRect();
	Rect& GetGlobalRect();

	int GetID() const;

	Item_Event GetLastEvent() const;
	UI_Item* GetParent() const;
	const char* GetName() const;
	uint GetChildCount() const;
	UI_Item* GetChild(uint index) const;
	const std::vector<UI_Item*> GetChildren() const;
	void CollectAllChildren(std::vector<UI_Item*>& vector) const;
	Item_Type GetType() const;

	bool IsActive() const;
	bool IsParentActive() const;

protected:
	virtual void InternalSave(Config& config) = 0;
	virtual void InternalLoad(Config& config) = 0;

protected:
	std::string name;
	Item_Type type;

	Rect rect;
	Rect global_rect;

	Vec2 scale = Vec2(1.0, 1.0);
	Vec2 global_scale = Vec2(1.0, 1.0);

	Item_Event last_event = Mouse_Exit;

	UI_Item* parent = nullptr;
	std::vector<UI_Item*> children;

	bool active = true;
	int id = -1;
};

#endif
