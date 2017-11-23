
#include "controlsmenu.h"
#include "Support/json/JSONWriter.h"

extern vector<Controls::AliasMappig> controlsMapping;

void ControlsMenu::Work(float dt)
{
	if (sel_mapping == -1)
	{
		Menu::Work(dt);

		if (controls.GetAliasState(alias_menu_act))
		{
			sel_mapping = items[sel_elemenet].data;

			if (sel_mapping != -1)
			{
				first_key = true;
			}
		}
	}
	else
	{
		make_hotkey = controls.GetAliasState(alias_add_hotkey, Controls::Active);

		DrawElements();

		if (controls.GetAliasState(alias_stop_edit))
		{
			sel_mapping = -1;
		}
		else
		{
			int device_index;
			const char* key = controls.GetActivatedKey(device_index);

			if (key && !controlsMapping[4].IsContainHAlias(key))
			{
				bool allow = true;

				if (first_key)
				{
					controlsMapping[sel_mapping].bindedNames.clear();
					first_key = false;
				}
				else
				{
					allow = !controlsMapping[sel_mapping].IsContainHAlias(key);
				}

				if (allow)
				{
					Controls::AliasMappig::BindName bndName;
					bndName.name = key;
					bndName.device_index = device_index;

					if (first_key || !make_hotkey)
					{
						vector<Controls::AliasMappig::BindName> names;
						names.push_back(bndName);

						controlsMapping[sel_mapping].bindedNames.push_back(names);
					}
					else
					{
						controlsMapping[sel_mapping].bindedNames.back().push_back(bndName);
					}
				}
			}
		}
	}

	if (sel_mapping != -1)
	{
		render.DebugPrintText(Vector2(180, 510), COLOR_YELLOW, "Hold Left CONTROL to create key combination");
		render.DebugPrintText(Vector2(200, 550), COLOR_YELLOW, "Press ESCAPE to stop adding keys to alias");
	}

	render.DebugPrintText(Vector2(360, 50), COLOR_WHITE, "Player 1");
	render.DebugPrintText(Vector2(360, 250), COLOR_WHITE, "Player 2");

	for (auto& item : items)
	{
		int index = &item - &items[0];

		if (item.data != -1)
		{
			Color color = COLOR_WHITE;
			if (index == sel_elemenet)
			{
				color = COLOR_GREEN;
			}

			char text[1024];
			text[0] = 0;

			if (item.data != sel_mapping || !first_key)
			{
				for (auto& bindedName : controlsMapping[item.data].bindedNames)
				{
					if (text[0] != 0)
					{
						StringUtils::Cat(text, 1024, ", ");
					}

					for (auto& bndName : bindedName)
					{
						int index = &bndName - &bindedName[0];

						if (index != 0)
						{
							StringUtils::Cat(text, 1024, " + ");
						}

						StringUtils::Cat(text, 1024, bndName.name.c_str());
					}
				}
			}

			if (item.data == sel_mapping)
			{
				if (text[0] != 0)
				{
					if (!make_hotkey)
					{
						StringUtils::Cat(text, 1024, ", ");
					}
					else
					{
						StringUtils::Cat(text, 1024, " + ");
					}
				}

				StringUtils::Cat(text, 1024, "_");
			}

			render.DebugPrintText(item.pos + Vector2(80, 0), color, text);
		}
	}
}

void ControlsMenu::SaveMapping()
{
	JSONWriter* writer = new JSONWriter();

	writer->Start("settings/controls/game_pc");

	writer->StartArray("Aliases");

	for (auto cntrl : controlsMapping)
	{
		writer->StartBlock(nullptr);

		writer->Write("name", cntrl.name.c_str());

		writer->StartArray("AliasesRef");

		for (auto& bindedName : cntrl.bindedNames)
		{
			writer->StartBlock(nullptr);

			writer->StartArray("names");

			for (auto& bndName : bindedName)
			{
				writer->Write(nullptr, bndName.name.c_str());
				writer->Write(nullptr, bndName.device_index);
			}

			writer->FinishArray();

			writer->FinishBlock();
		}

		writer->FinishArray();

		writer->FinishBlock();
	}

	writer->FinishArray();

	writer->Release();
}