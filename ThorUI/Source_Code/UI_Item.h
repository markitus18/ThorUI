#ifndef __UI_ITEM_H__
#define __UI_ITEM_H__

#include "Vec2.h"
#include "Rect.h"
#include "Transform.h"
#include "Signal.h"

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
	THORUI_API UI_Item();
	THORUI_API UI_Item(float x, float y);
	THORUI_API UI_Item(Vec2 pos, Vec2 size);
	THORUI_API ~UI_Item();

	THORUI_API inline void SetPos(float x, float y) { transform.SetPos(Vec2(x, y));}
	THORUI_API inline void SetPos(Vec2 pos) { transform.SetPos(pos); }
	THORUI_API void SetSize(float w, float h);
	THORUI_API virtual void SetSize(Vec2 size);
	THORUI_API inline void SetScale(float x, float y){ transform.SetScale(Vec2(x, y)); }
	THORUI_API inline void SetScale(Vec2 scale){ transform.SetScale(scale); }
	THORUI_API inline void SetRotation(float rotation){ transform.SetRotationDeg(rotation); }
	THORUI_API inline void SetPivot(Vec2 pivot)
	{
		this->pivot = pivot;
		transform.SetPivot((Vec2(0.5f, 0.5f) - pivot) * size);
	}

	THORUI_API inline void SetID(int id) { this->id = id; }
	THORUI_API inline void SetName(const char* name){ this->name = name; }
	THORUI_API void SetActive(bool active);

	THORUI_API 	void SetParent(UI_Item* parent, UI_Item* next_child = nullptr, bool keep_global = true);
	THORUI_API 	void RemoveChild(UI_Item* child);
	THORUI_API void DeleteChildren();

	THORUI_API inline UI_Item* GetParent() const
	{
		return transform.GetParent() ? transform.GetParent()->Container<UI_Item>() : nullptr;
	}

	THORUI_API UI_Item* GetChild(uint index) const;
	THORUI_API inline uint ChildCount() const { return transform.GetChildren().size(); }
	THORUI_API void CollectChildren(std::vector<UI_Item*>& vector);

	THORUI_API virtual void Draw() {}; //TODO: draw here, or in ThorUI?
	THORUI_API virtual void OnItemEvent(Item_Event event) { last_event = event; };

	THORUI_API void Save(Config& config);
	THORUI_API void Load(Config& config);

	THORUI_API inline Vec2 GetPos() const{ return transform.GetPos();	}
	THORUI_API inline Vec2 GetGlobalPos() const {return transform.Global().GetTranslation();}
	THORUI_API inline Vec2 GetSize() const { return size; }
	THORUI_API inline Vec2 GetScale() const { return transform.GetScale(); }
	THORUI_API inline float GetRotation() const { return transform.GetRotation(); }
	THORUI_API inline Vec2 GetPivot() const { return pivot; }
	THORUI_API inline Transform* GetTransform() { return &transform; }

	THORUI_API inline int GetID() const { return id; }

	THORUI_API inline Item_Event GetLastEvent() const { return last_event; }
	THORUI_API inline const char* GetName() const { return name.c_str(); }
	THORUI_API inline Item_Type GetType() const { return type; }

	THORUI_API inline bool IsActive() const { return active; }
	THORUI_API inline bool IsActiveHierarchy() const { return active && hierarchyActive; }

protected:
	THORUI_API virtual void InternalSave(Config& config) = 0;
	THORUI_API virtual void InternalLoad(Config& config) = 0;

private:
	THORUI_API void SetHierarchyActive(bool active);

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
