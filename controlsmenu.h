#pragma once

#include "menu.h"

class ControlsMenu : public Menu
{
	int sel_mapping = -1;
	bool first_key = false;
	bool make_hotkey = false;

public:

	virtual void Work(float dt);
	void SaveMapping();
};