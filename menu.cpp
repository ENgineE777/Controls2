
#include "menu.h"

int Menu::alias_menu_up = 1;
int Menu::alias_menu_down = -1;
int Menu::alias_menu_act = -1;
int Menu::alias_add_hotkey = -1;
int Menu::alias_pause_game = -1;
int Menu::alias_stop_edit = -1;

void Menu::Work(float dt)
{
	DrawElements();

	if (controls.GetAliasState(alias_menu_down))
	{
		sel_elemenet++;
		if (sel_elemenet >= items.size())
		{
			sel_elemenet=0;
		}
	}

	if (controls.GetAliasState(alias_menu_up))
	{
		sel_elemenet--;
		if (sel_elemenet < 0)
		{
			sel_elemenet = items.size() - 1;
		}
	}

	if (controls.GetAliasState(alias_menu_act) && items[sel_elemenet].action)
	{
		items[sel_elemenet].action();
	}
}

void Menu::DrawElements()
{
	for (auto& item : items)
	{
		int index = &item - &items[0];

		Color color = COLOR_WHITE;
		if (index == sel_elemenet)
		{
			color = COLOR_GREEN;
		}

		render.DebugPrintText(item.pos, color, item.text.c_str());
	}
}