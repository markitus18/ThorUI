#include "ThorUI.h"

#include "SDL2-2.0.6\include\SDL.h"
#include "glew-2.1.0\include\GL\glew.h"
#include "SDL2_ttf-2.0.14\include\SDL_ttf.h"

#include <string>
#include <map>

#include "Log.h"
#include "Vec2.h"
#include "Color.h"
#include "UI_Item.h"
#include "UI_Image.h"
#include "UI_Button.h"
#include "UI_Text.h"
#include "UI_Panel.h"
#include "Signal_Event.h"
#include "Circle.h"
#include "TMath.h"
#include <time.h> // TODO:
#include "Config.h"

#pragma comment (lib, "SDL2_ttf-2.0.14/libx86/SDL2_ttf.lib")
#pragma comment( lib, "SDL2-2.0.6/libx86/SDL2.lib" )
#pragma comment( lib, "SDL2-2.0.6/libx86/SDL2main.lib" )
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")

namespace ThorUI
{
	Key_State* keyboard = nullptr;
	Key_State* mouse_buttons = nullptr;
	bool* mouse_button_event = nullptr;
	Vec2 mouse_pos = Vec2(-1000, -1000);
	Vec2 last_mouse_click = Vec2(-1000, -1000);
	bool mouse_dragging = false;

	UI_Item* window_item = nullptr;
	std::vector<UI_Item*> items;
	std::vector<UI_Item*> to_remove_items;
	std::map<uint, Texture> textures;
	std::vector<Font> fonts;

	Vec2 screen_size;
	bool mouse_out = false;

	void Init(SDL_Window* window)
	{
		srand(time(nullptr)); //TODO: should be done in math namespace
		keyboard = new Key_State[SDL_NUM_SCANCODES];
		for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
			keyboard[i] = KEY_IDLE;
		mouse_buttons = new Key_State[3];
		for (int i = 0; i < 3; ++i)
			mouse_buttons[i] = KEY_IDLE;
		mouse_button_event = new bool[3];
		memset(mouse_button_event, 0, 3);

		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		screen_size.Set(w, h);
		gluOrtho2D(0.0f, screen_size.x, 0.0f, screen_size.y);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Sets the mouse to trigger "OnMouseDown" event when gaining focus
		SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");

		if (TTF_Init() != 0)
		{
			LOG("Could not initialize SDL_TTF: %s", TTF_GetError());
		}
		else
		{
			LoadFont("Times_New_Roman_Normal.ttf", 36);
		}

		window_item = new UI_Text(Vec2(0, 0), screen_size, ""); //TODO Hehe
		window_item->SetName("Window");
		window_item->SetID(0);
		items.push_back(window_item);
	}

	void StartFrame()
	{
		PreStart();
		Update();
	}

	void PreStart()
	{
		RemoveToDeleteItems();
		UpdateKeyboardState();
		UpdateMouseState();
	}

	void Draw()
	{
		std::vector<UI_Item*>::iterator it;
		for (it = items.begin(); it != items.end(); ++it)
		{
			(*it)->Draw();
		}
	}

	void CleanUp()
	{
		delete[] mouse_buttons;
		mouse_buttons = nullptr;
		delete[] keyboard;
		keyboard = nullptr;
	}

	void UpdateKeyboardState()
	{
		const Uint8* keys = SDL_GetKeyboardState(nullptr);
		for (unsigned int i = 0; i < SDL_NUM_SCANCODES; ++i)
		{
			if (keys[i] == 1)
			{
				keyboard[i] = (keyboard[i] == KEY_DOWN || keyboard[i] == KEY_REPEAT) ? KEY_REPEAT : KEY_DOWN;
			}
			else if (keyboard[i] != KEY_IDLE)
			{
				keyboard[i] = (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
			}
		}
	}

	//Just for debugging purposes: shows key state change output
	void DebugKeyboardState(int key, Key_State state)
	{
		std::string str = SDL_GetKeyName(SDL_GetKeyFromScancode((SDL_Scancode)key));

		char* key_str[4] = { "Idle", "Down", "Repeat", "Up" };
		str.append(" - Key ").append(key_str[state]);

		LOG(str.c_str());
	}

	Key_State GetKeyState(int key)
	{
		return keyboard[key];
	}

	void UpdateMouseState()
	{
		if (mouse_out == false)
		{
			int x, y;
			SDL_GetMouseState(&x, &y);
			mouse_pos.Set(x, screen_size.y - y); //Y must be inverted (SDL 0y is top)
		}

		for (int i = 0; i < 3; ++i)
		{
			if (mouse_button_event[i] == false)
			{
				if (mouse_buttons[i] == KEY_DOWN)
				{
					mouse_buttons[i] = KEY_REPEAT;
					last_mouse_click = mouse_pos;
				}
				if (mouse_buttons[i] == KEY_UP)
					mouse_buttons[i] = KEY_IDLE;
			}
		}
		memset(mouse_button_event, 0, 3);

		mouse_dragging = false;
		if (mouse_buttons[0] == KEY_REPEAT && mouse_pos != last_mouse_click)
		{
			mouse_dragging = true;
		}
	}

	bool IsMouseHovering(Vec2 pos, Vec2 size)
	{
		return (mouse_pos.x > pos.x && mouse_pos.x < pos.x + size.x &&
			mouse_pos.y > pos.y && mouse_pos.y < pos.y + size.y);
	}

	Key_State GetMouseState(int id)
	{
		return mouse_buttons[id - 1];
	}

	bool IsMouseDown(int id)
	{
		return (mouse_buttons[id - 1] == KEY_DOWN || mouse_buttons[id - 1] == KEY_REPEAT);
	}

	bool IsMouseIdle(int id)
	{
		return (mouse_buttons[id - 1] == KEY_UP || mouse_buttons[id - 1] == KEY_IDLE);
	}


	bool IsMouseHoveringItem(UI_Item* item)
	{
		return IsPointOnItem(item, mouse_pos);
	}

	bool IsPointOnItem(UI_Item* item, Vec2 p)
	{
		//Transforming the point into item axis
		p.Transform(item->GetTransform()->Center().Inverted());

		Vec2 pos = item->GetSize() / -2;
		Vec2 size = item->GetSize();

		return (p.x > pos.x && p.x < pos.x + size.x &&
			p.y > pos.y && p.y < pos.y + size.y);
	}

	uint LoadTexture(const char* path)
	{
		//Looking if the texture is already loaded
		if (uint tex_id = FindTexture(path))
			return tex_id;

		SDL_Surface* surf = SDL_LoadBMP(path);
		if (surf == nullptr)
		{
			LOG("Error loading texture: %s", SDL_GetError());
			return 0;
		}

		//Generating texture buffer
		Texture texture;
		texture.id = GenTextureFromSurf(surf);
		texture.original_size.Set(surf->w, surf->h);
		texture.path = path;

		textures[texture.id] = texture;
		SDL_FreeSurface(surf);

		return texture.id;
	}

	uint FindTexture(const char* path)
	{
		for (std::map<uint, Texture>::iterator it = textures.begin(); it != textures.end(); ++it)
		{
			if ((*it).second.path.compare(path) == 0)
				return (*it).first;
		}
		return 0;
	}

	Texture* GetTexture(uint texture_id)
	{
		if (textures.find(texture_id) != textures.end())
			return &textures[texture_id];
		return nullptr;
	}

	void FreeTexture(uint texture_id)
	{
		glDeleteTextures(1, &texture_id);
		if (textures.find(texture_id) != textures.end())
		{
			textures.erase(texture_id);
		}
	}

	void OnSetTexture(uint texture_id)
	{
		if (textures.find(texture_id) != textures.end())
			textures[texture_id].instances++;
	}

	void OnLeaveTexture(uint texture_id)
	{
		if (textures.find(texture_id) != textures.end())
			textures[texture_id].instances--;
	}

	uint LoadFont(const char* path, uint size)
	{
		if (uint font_id = FindFont(path, size))
			return font_id;

		TTF_Font* font = TTF_OpenFont(path, size);
		if (font == nullptr) return 0;

		fonts.push_back(Font(font, path, size));
		return fonts.size();
	}

	uint FindFont(const char* path, uint size)
	{
		int i = 0;
		for (std::vector<Font>::iterator it = fonts.begin(); it != fonts.end(); ++it, ++i)
			if ((*it).path == path && (*it).size == size)
				return i + 1;
		return 0;
	}

	void OnSetFont(uint font_id)
	{
		if (font_id <= fonts.size())
			fonts[font_id - 1].instances++;
	}

	void OnLeaveFont(uint font_id)
	{
		if (font_id <= fonts.size())
			fonts[font_id - 1].instances--;
	}

	uint GenTextTexture(const char* text, uint font, Vec2& texture_size, Vec2 boundaries_size)
	{
		if (font > fonts.size()) return 0; //Immediate return on invalid font


		SDL_Surface* surf = TTF_RenderText_Blended_Wrapped(fonts[font - 1].font, text, Color::White().ToSDL(), boundaries_size.x);
		texture_size.Set(surf->w, surf->h);
		if (surf == nullptr)
		{
			LOG("Error loading text texture: %s", TTF_GetError());
			return 0;
		}

		uint texture = GenTextureFromSurf(surf);
		SDL_FreeSurface(surf);

		return texture;
	}

	uint GenTexture()
	{
		GLuint texture = 0;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		return texture;
	}

	uint GenTextureFromSurf(SDL_Surface* surf)
	{
		SDL_Rect image_area;
		SDL_Surface* gl_surf = nullptr;

		//TODO: check RGBA masks, depending on SDL data saving
		gl_surf = SDL_CreateRGBSurface(SDL_SWSURFACE, surf->w, surf->h, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
		gl_surf->format->Ashift = 24;
		//TOOD: save surface format to restore it later

		SDL_SetSurfaceAlphaMod(surf, SDL_ALPHA_OPAQUE);
		image_area = { 0, 0, surf->w, surf->h };
		SDL_BlitSurface(surf, &image_area, gl_surf, &image_area);

		//TODO: restore surface

		uint texture = GenTexture();

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surf->w, surf->h, 0,
			GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, gl_surf->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		return texture;
	}

	void GetEvent(SDL_Event* event)
	{
		switch (event->type)
		{
		case(SDL_MOUSEBUTTONUP):
		{
			mouse_buttons[event->button.button - 1] = KEY_UP;
			mouse_button_event[event->button.button - 1] = true;
			break;
		}
		case(SDL_MOUSEBUTTONDOWN):
		{
			mouse_buttons[event->button.button - 1] = KEY_DOWN;
			mouse_button_event[event->button.button - 1] = true;
			break;
		}
		case(SDL_WINDOWEVENT):
		{
			switch (event->window.event)
			{
			case(SDL_WINDOWEVENT_LEAVE):
			{
				mouse_pos.Set(-1000, -1000);
				mouse_out = true;
				break;
			}
			case(SDL_WINDOWEVENT_ENTER):
			{
				mouse_out = false;
				break;
			}
			}
		}
		}
	}

	void DrawQuad(Vec2 pos, Vec2 size, Color color, bool filled, float lineWidth)
	{
		if (filled == false)
		{
			DrawNoFilledQuad(pos, size, color, lineWidth);
			return;
		}

		glEnable(GL_BLEND);
		glColor4fv(color.ptr());
		glBegin(GL_QUADS);

		glVertex2f(pos.x, pos.y);
		glVertex2f(pos.x + size.x, pos.y);
		glVertex2f(pos.x + size.x, pos.y + size.y);
		glVertex2f(pos.x, pos.y + size.y);

		glEnd();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glDisable(GL_BLEND);
	}

	void DrawNoFilledQuad(Vec2 pos, Vec2 size, Color color, float lineWidth)
	{
		//Drawing side quads
		DrawQuad(pos, Vec2(lineWidth, size.y), color);
		DrawQuad(pos + Vec2(size.x - lineWidth, 0),  Vec2(lineWidth, size.y), color); 

		//Drawing top/bottom quads
		DrawQuad(pos + Vec2(lineWidth, 0), Vec2(size.x - lineWidth * 2, lineWidth), color);
		DrawQuad(pos + Vec2(lineWidth, size.y - lineWidth), Vec2(size.x - lineWidth * 2, lineWidth), color);
	}

	void DrawImage(Vec2 pos, Vec2 size, int texture_id, Color color)
	{
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glEnable(GL_TEXTURE_2D);

		glEnable(GL_BLEND);

		glBegin(GL_QUADS);
		glColor4fv(color.ptr());
		glTexCoord2f(0.0, 1.0);	glVertex2f(pos.x, pos.y);
		glTexCoord2f(1.0, 1.0);	glVertex2f(pos.x + size.x, pos.y);
		glTexCoord2f(1.0, 0.0);	glVertex2f(pos.x + size.x, pos.y + size.y);
		glTexCoord2f(0.0, 0.0);	glVertex2f(pos.x, pos.y + size.y);
		glEnd();
		glColor4fv(Color::White().ptr());
		glDisable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}

	void AddItem(UI_Item* item)
	{
		items.push_back(item);

		if (item->GetID() == -1)
			item->SetID(Math::RandUInt());
		if (item->GetParent() == nullptr)
		{
			item->SetParent(window_item, nullptr, false);
		}
	}

	std::string GenUniqueName(UI_Item* parent, const char* name)
	{
		uint dup_count = 0;
		bool dup_found = true;
		std::string current_name = name;
		while (dup_found == true)
		{
			dup_found = false;
			for (uint i = 0; i < parent->ChildCount(); ++i)
			{
				if (parent->GetChild(i)->GetName() == current_name)
				{
					dup_found = true;
					dup_count++;
					current_name = std::string(name).append(" (").append(std::to_string(dup_count)).append(")");
				}
			}
		}
		return current_name;
	}

	UI_Item* GetItem(uint item_id) //TODO: move to a map [id, item]
	{
		for (uint i = 0; i < items.size(); ++i)
		{
			if (items[i]->GetID() == item_id)
				return items[i];
		}
		return nullptr;
	}

	void Update()
	{
		std::vector<UI_Item*>::iterator it;
		for (it = items.begin(); it != items.end(); ++it)
		{
			Item_Event last_event = (*it)->GetLastEvent();
			if (IsMouseHoveringItem(*it))
			{
				if (last_event == Mouse_Exit)
					(*it)->InternalOnItemEvent(Mouse_Enter);
				if (last_event == Mouse_Down && IsMouseIdle(SDL_BUTTON_LEFT))
					(*it)->InternalOnItemEvent(Mouse_Up);
				if ((last_event == Mouse_Enter || last_event == Mouse_Up) && GetMouseState(SDL_BUTTON_LEFT) == KEY_DOWN)
					(*it)->InternalOnItemEvent(Mouse_Down);
			}
			else //Mouse is not hovering item
			{
				if (last_event != Mouse_Exit)
					(*it)->InternalOnItemEvent(Mouse_Exit);
			}
			(*it)->Update(1);
		}
	}

	void DeleteItem(UI_Item* item)
	{
		for (uint i = 0; i < to_remove_items.size(); ++i)
		{
			if (to_remove_items[i] == item)
				return;
		}

		to_remove_items.push_back(item);
		item->RemoveFromParent();

		while (item->ChildCount() > 0)
		{
			DeleteItem(item->GetChild(0));
		}
	}

	void RemoveToDeleteItems()
	{
		while (to_remove_items.size() > 0)
		{
			UI_Item* item = *to_remove_items.begin();
			std::vector<UI_Item*>::iterator it = items.begin();
			while (it != items.end())
			{
				if (*it == *to_remove_items.begin())
				{
					items.erase(it);
					to_remove_items.erase(to_remove_items.begin());
					break;
				}
				else
					++it;
			}
			delete item;
			item = nullptr;
		}
	}

	void SaveScene(const char* path)
	{
		SDL_RWops *rw = SDL_RWFromFile(path, "w");
		if (rw != nullptr)
		{
			Config file;
			Config_Array arr = file.SetArray("Items");

			for (uint i = 1; i < items.size(); ++i)
			{
				items[i]->InternalSave(arr.AddNode());

			}

			char* buff = nullptr;
			uint size = file.Serialize(&buff);

			if (SDL_RWwrite(rw, buff, 1, size) != size)
			{
				LOG("[Warning] Could not fully save scene");
			}
			SDL_RWclose(rw);
		}
	}

	void LoadScene(const char* path)
	{
		SDL_RWops* rw = SDL_RWFromFile(path, "r");
		if (rw != nullptr)
		{
			uint size = SDL_RWsize(rw);
			char* buf = new char[size];

			if (SDL_RWread(rw, buf, 1, size) == size)
			{
				std::vector<int> parent_ids;
				std::vector<UI_Item*> new_items;

				ClearScene();
				Config file(buf);
				Config_Array arr = file.GetArray("Items");
				for (uint i = 0; i < arr.GetSize(); ++i)
				{
					Config node = arr.GetNode(i);
					Item_Type type = (Item_Type)(int)node.GetNumber("Item_Type");
					UI_Item* item = nullptr;

					if (type != Item_Type::Item)
					{
						switch (type)
						{
						case(Item_Type::Image):
						{
							item = new UI_Image();
							break;
						}
						case(Item_Type::Button):
						{
							item = new UI_Button();
							break;
						}
						case(Item_Type::Text):
						{
							item = new UI_Text();
							break;
						}
						case (Item_Type::Panel):
						{
							item = new UI_Panel();
							break;
						}
						}
						item->InternalLoad(node);
						AddItem(item);
						parent_ids.push_back(node.GetNumber("Parent ID", -1));
						new_items.push_back(item);
					}
				}

				//Set all parent hierarchy after all items are created
				for (uint i = 0; i < parent_ids.size(); ++i)
				{
					bool set = false;
					UI_Item* parent = GetItem(parent_ids[i]);
					new_items[i]->SetParent(parent ? parent : window_item, nullptr, false);
				}
				ConnectLoadedItems();
			}
			SDL_RWclose(rw);
		}
	}

	void GenerateDefaultApSets()
	{
		for (uint i = 1; i < items.size(); ++i)
		{
			items[i]->SaveDefaultApSet();
		}
	}

	void ConnectLoadedItems()
	{
		for (uint i = 1; i < items.size(); ++i)
		{
			std::vector<Signal_Event>& events = items[i]->GetSignalEvents();
			for (uint e = 0; e < events.size(); e++)
			{
				UI_Item* to_connect = GetItem(events[e].item_id);
				if (to_connect != nullptr)
					to_connect->ConnectItemWithSignal(items[i], events[e].signal_name, events[e]);
				else
					events[e].SetLinkError();
			}
		}
	}

	void ClearScene()
	{
		while (window_item->ChildCount() > 0)
		{
			UI_Item* item = window_item->GetChild(0);
			window_item->RemoveChild(window_item->GetChild(0));
			delete item;
		}
		items.clear();
		items.push_back(window_item);
	}

	void SetCanvasSize(Vec2 size)
	{
		window_item->SetSize(size);
		Vec2 pos = (screen_size - size) / 2;
		window_item->SetPos(pos);
	}
}

