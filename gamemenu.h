#pragma once

#include "menu.h"

class GameMenu : public Menu
{
	bool paused = false;
	float player_speed = 500.0f;
	float player_size = 16.0f * 4.0f;
	Vector2 ball_start_pos = Vector2(400.0f, 300.0f);
	float ball_speed = 450.0f;
	float ball_radius = 8.0f;

	float player1_pos;
	float player2_pos;
	Vector2 ball_pos;
	Vector2 ball_dir;
	int player1_score;
	int player2_score;

public:

	void ResetBall();
	void ResetGame();
	void UpdatePlayer(float dt, int index, float &pos);

	void UpdateBall(float dt);
	void DrawPlayer(Vector2 pos);

	void Work(float dt);
};