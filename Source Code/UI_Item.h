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

	inline void SetPos(float x, float y) { transform.SetPos(Vec2(x, y));}
	inline void SetPos(Vec2 pos) { transform.SetPos(pos); }
	void SetSize(float w, float h);
	virtual void SetSize(Vec2 size);
	inline void SetScale(float x, float y){ transform.SetScale(Vec2(x, y)); }
	inline void SetScale(Vec2 scale){ transform.SetScale(scale); }
	inline void SetRotation(float rotation){ transform.SetRotationDeg(rotation); }
	inline void SetPivot(Vec2 pivot)
	{
		this->pivot = pivot;
		transform.SetPivot((Vec2(0.5f, 0.5f) - pivot) * size);
	}

	inline void SetID(int id) { this->id = id; }
	inline void SetName(const char* name){ this->name = name; }
	void SetActive(bool active);

	void SetParent(UI_Item* parent, UI_Item* next_child = nullptr, bool keep_global = true);
	void RemoveChild(UI_Item* child);
	void DeleteChildren();

	inline UI_Item* GetParent() const
	{
		return transform.GetParent() ? transform.GetParent()->Container<UI_Item>() : nullptr;
	}

	UI_Item* GetChild(uint index) const;
	inline uint ChildCount() const { return transform.GetChildren().size(); }
	void CollectChildren(std::vector<UI_Item*>& vector);

	virtual void Draw() {}; //TODO: draw here, or in ThorUI?
	virtual void OnItemEvent(Item_Event event) { last_event = event; };

	void Save(Config& config);
	void Load(Config& config);

	inline Vec2 GetPos() const{ return transform.GetPos();	}
	inline Vec2 GetGlobalPos() const {return transform.Global().GetTranslation();}
	inline Vec2 GetSize() const { return size; }
	inline Vec2 GetScale() const { return transform.GetScale(); }
	inline float GetRotation() const { return transform.GetRotation(); }
	inline Vec2 GetPivot() const { return pivot; }
	inline Transform* GetTransform() { return &transform; }

	inline int GetID() const { return id; }

	inline Item_Event GetLastEvent() const { return last_event; }
	inline const char* GetName() const { return name.c_str(); }
	inline Item_Type GetType() const { return type; }

	inline bool IsActive() const { return active; }
	inline bool IsActiveHierarchy() const { return active && hierarchyActive; }

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
	//Item pivot is saved as a % pivot for its rect size
	//Range [0, 1], 0 being bottom-left, 1 top-right
	Vec2 pivot;
	Transform transform;

};

#endif
