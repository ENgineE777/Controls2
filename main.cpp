#include <windows.h>
#include <Windowsx.h>

#include "Support/Timer.h"
#include "controlsmenu.h"
#include "gamemenu.h"

HWND hwnd;

const char g_szClassName[] = "LectureClass";

vector<Controls::AliasMappig> controlsMapping;

Menu start_menu;
ControlsMenu controls_menu;
GameMenu game_menu;

Menu* cur_menu = &start_menu;

void ShowControls()
{
	cur_menu = &controls_menu;
}

void HideControls()
{
	cur_menu = &start_menu;

	controls_menu.SaveMapping();
	controls.LoadAliases("settings/controls/game_pc");
}

void ShowGame()
{
	cur_menu = &game_menu;
	game_menu.ResetGame();
}

void HideGame()
{
	cur_menu = &start_menu;
}

void Init(int width, int height)
{
	controls.Init("settings/controls/hardware_pc", true);
	controls.LoadAliases("settings/controls/user_pc");
	controls.LoadAliases("settings/controls/game_pc");

	controls.SetWindow(&hwnd);

	render.Init("DX11", width, height, &hwnd);

	Menu::alias_menu_up = controls.GetAlias("Menu.Up");
	Menu::alias_menu_down = controls.GetAlias("Menu.Down");
	Menu::alias_menu_act = controls.GetAlias("Menu.Action");
	Menu::alias_add_hotkey = controls.GetAlias("Menu.AddHotkey");
	Menu::alias_pause_game = controls.GetAlias("Menu.PauseGame");
	Menu::alias_stop_edit = controls.GetAlias("Menu.StopEdit");

	controlsMapping.push_back(Controls::AliasMappig("Player1.Up"));
	controlsMapping.push_back(Controls::AliasMappig("Player1.Down"));
	controlsMapping.push_back(Controls::AliasMappig("Player2.Up"));
	controlsMapping.push_back(Controls::AliasMappig("Player2.Down"));
	controlsMapping.push_back(Controls::AliasMappig("Menu.AddHotkey"));

	start_menu.items.push_back(Menu::Item(Vector2(365, 200), "Start", ShowGame));
	start_menu.items.push_back(Menu::Item(Vector2(350, 250), "Controls", ShowControls));

	controls_menu.items.push_back(Menu::Item(Vector2(300, 100), "Up", nullptr, 0));
	controls_menu.items.push_back(Menu::Item(Vector2(300, 150), "Down", nullptr, 1));
	controls_menu.items.push_back(Menu::Item(Vector2(300, 300), "Up", nullptr, 2));
	controls_menu.items.push_back(Menu::Item(Vector2(300, 350), "Down", nullptr, 3));
	controls_menu.items.push_back(Menu::Item(Vector2(370, 450), "Back", HideControls));

	game_menu.items.push_back(Menu::Item(Vector2(350, 200), "Resume", ShowGame));
	game_menu.items.push_back(Menu::Item(Vector2(365, 250), "Back", HideGame));
}

void Draw(float dt)
{
	if (cur_menu)
	{
		cur_menu->Work(dt);
	}

	render.GetDevice()->Clear(true, COLOR_GRAY, true, 1.0f);

	render.Execute(dt);
	controls.Update(dt);

	render.GetDevice()->Present();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);

	switch (msg)
	{
		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
		break;
		case WM_MOUSEMOVE:
		{
			controls.OverrideMousePos(xPos, yPos);
			break;
		}

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	MSG Msg;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"Controls sample",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	Init(800, 600);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg;
	bool done = false;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			float dt = Timer::CountDeltaTime();

			Draw(dt);
		}
	}

	return 0;
}