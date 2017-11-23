#pragma once

#include "Services/Controls/Controls.h"
#include "Services/Render/Render.h"
#include "Support/Support.h"

class Menu
{
public:

	typedef void(*MunuItemAction)();

	static int alias_menu_up;
	static int alias_menu_down;
	static int alias_menu_act;
	static int alias_add_hotkey;
	static int alias_pause_game;
	static int alias_stop_edit;

	int sel_elemenet = 0;

	struct Item
	{
		Vector2 pos;
		std::string text;
		int data = -1;
		MunuItemAction action;

		Item(Vector2 pos, const char* text, MunuItemAction action, int data = -1)
		{
			this->pos = pos;
			this->text = text;
			this->action = action;
			this->data = data;
		}
	};

	std::vector<Item> items;

	virtual void Work(float dt);
	void DrawElements();
};