#pragma once

#include "gamemenu.h"

extern vector<Controls::AliasMappig> controlsMapping;

void GameMenu::ResetBall()
{
	ball_pos = ball_start_pos;
	ball_dir.x = rnd_range(-1.0f, 1.0f);
	ball_dir.y = rnd_range(-1.0f, 1.0f);
	ball_dir.Normalize();
}

void GameMenu::ResetGame()
{
	player1_pos = 300.0f - player_size * 0.5f;
	player2_pos = 300.0f - player_size * 0.5f;

	player1_score = 0;
	player2_score = 0;
	ResetBall();
	paused = false;
}

void GameMenu::UpdatePlayer(float dt, int index, float &pos)
{
	if (controls.GetAliasState(controlsMapping[index + 0].alias, Controls::Active))
	{
		pos -= dt * player_speed;

		if (pos < 0.0f)
		{
			pos = 0.0f;
		}
	}

	if (controls.GetAliasState(controlsMapping[index + 1].alias, Controls::Active))
	{
		pos += dt * player_speed;

		if (pos > 600.0f - player_size)
		{
			pos = 600.0f - player_size;
		}
	}
}

void GameMenu::UpdateBall(float dt)
{
	ball_pos += ball_dir * ball_speed * dt;

	if (ball_pos.y < ball_radius)
	{
		ball_pos.y = ball_radius;
		ball_dir.y = -ball_dir.y;
	}

	if (ball_pos.y > 600 - ball_radius)
	{
		ball_pos.y = 600 - ball_radius;
		ball_dir.y = -ball_dir.y;
	}

	if (player1_pos < ball_pos.y &&
		ball_pos.y < player1_pos + player_size && ball_pos.x < 15.0f + ball_radius)
	{
		ball_pos.x = 16.0f + ball_radius;
		ball_dir.x = 1.0;
		ball_dir.y = (ball_pos.y - (player1_pos + player_size * 0.5f)) / player_size;
		ball_dir.Normalize();
	}

	if (player2_pos < ball_pos.y &&
		ball_pos.y < player2_pos + player_size && ball_pos.x > 785.0f - ball_radius)
	{
		ball_pos.x = 784.0f - ball_radius;
		ball_dir.x = -1.0;
		ball_dir.y = (ball_pos.y - (player2_pos + player_size * 0.5f)) / player_size;
		ball_dir.Normalize();
	}

	if (ball_pos.x < 0)
	{
		player2_score++;
		ResetBall();
	}

	if (ball_pos.x > 800)
	{
		player1_score++;
		ResetBall();
	}
}

void GameMenu::DrawPlayer(Vector2 pos)
{
	for (int i = 0; i < 4; i++)
	{
		render.DebugPrintText(pos + Vector2(0, i * 16.0f), COLOR_WHITE, "8");
	}
}

void GameMenu::Work(float dt)
{
	if (paused)
	{
		Menu::Work(dt);
	}
	else
	{
		UpdatePlayer(dt, 0, player1_pos);
		UpdatePlayer(dt, 2, player2_pos);
		UpdateBall(dt);

		if (controls.GetAliasState(alias_pause_game))
		{
			paused = true;
		}
	}

	DrawPlayer(Vector2(3, player1_pos));
	DrawPlayer(Vector2(785, player2_pos));

	render.DebugPrintText(ball_pos - Vector2(ball_radius), COLOR_WHITE, "O");

	char str[16];
	StringUtils::Printf(str, 16, "%i", player1_score);
	render.DebugPrintText(Vector2(375, 20.0f), COLOR_WHITE, str);
	render.DebugPrintText(Vector2(398, 20.0f), COLOR_WHITE, ":");
	StringUtils::Printf(str, 16, "%i", player2_score);
	render.DebugPrintText(Vector2(415, 20.0f), COLOR_WHITE, str);
}