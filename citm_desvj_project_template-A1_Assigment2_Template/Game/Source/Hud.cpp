#include "Hud.h"

#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "GuiManager.h"
#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

Hud::Hud(App* app, bool start_enabled) : Module(app,start_enabled)
{
	name.Create("hud");
}

Hud::~Hud()
{}

bool Hud::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneMenu");
	bool ret = true;


	return ret;
}

bool Hud::Start()
{

	deadScreenTexture = app->tex->Load("Assets/UI/respawn.png");
	GameOver = app->tex->Load("Assets/UI/GameOver.png");
	corazon1 = app->tex->Load("Assets/UI/1corazon.png");
	corazon2 = app->tex->Load("Assets/UI/2corazones.png");
	corazon3 = app->tex->Load("Assets/UI/3corazones.png");
	moneda0 = app->tex->Load("Assets/UI/moneda0.png");
	moneda1 = app->tex->Load("Assets/UI/moneda1.png");
	moneda2 = app->tex->Load("Assets/UI/moneda2.png");
	moneda3 = app->tex->Load("Assets/UI/moneda3.png");
	moneda4 = app->tex->Load("Assets/UI/moneda4.png");
	moneda5 = app->tex->Load("Assets/UI/moneda5.png");
	moneda6 = app->tex->Load("Assets/UI/moneda6.png");
	moneda7 = app->tex->Load("Assets/UI/moneda7.png");
	moneda8 = app->tex->Load("Assets/UI/moneda8.png");
	moneda9 = app->tex->Load("Assets/UI/moneda9.png");
	moneda10 = app->tex->Load("Assets/UI/moneda10.png");


	timer = Timer();
	timer.Start();

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;


	return true;
}

bool Hud::Update(float dt)
{
	if (app->scene->player->health>=3)
	{
		app->render->DrawTexture(corazon3, 0, 0, NULL, SDL_FLIP_NONE, 0);
	}
	else if (app->scene->player->health==2)
	{
		app->render->DrawTexture(corazon2, 0, 0, NULL, SDL_FLIP_NONE, 0);
	}
	else if (app->scene->player->health == 1)
	{
		app->render->DrawTexture(corazon1, 0, 0, NULL, SDL_FLIP_NONE, 0);
	}

	DrawTimer();

	if (app->scene->player->score==0)
	{
		app->render->DrawTexture(moneda0, 600, 40, NULL, SDL_FLIP_NONE, 0);
	}
	else if (app->scene->player->score == 1)
	{
		app->render->DrawTexture(moneda1, 600, 40, NULL, SDL_FLIP_NONE, 0);

	}
	else if (app->scene->player->score == 2)
	{
		app->render->DrawTexture(moneda2, 600, 40, NULL, SDL_FLIP_NONE, 0);
	}
	else if (app->scene->player->score == 3)
	{
		app->render->DrawTexture(moneda3, 600, 40, NULL, SDL_FLIP_NONE, 0);
	}
	else if (app->scene->player->score == 4)
	{
		app->render->DrawTexture(moneda4, 600, 40, NULL, SDL_FLIP_NONE, 0);
	}
	else if (app->scene->player->score == 5)
	{
		app->render->DrawTexture(moneda5, 600, 40, NULL, SDL_FLIP_NONE, 0);
	}
	else if (app->scene->player->score == 6)
	{
		app->render->DrawTexture(moneda6, 600, 40, NULL, SDL_FLIP_NONE, 0);
	}
	else if (app->scene->player->score == 7)
	{
		app->render->DrawTexture(moneda7, 600, 40, NULL, SDL_FLIP_NONE, 0);
	}
	else if (app->scene->player->score == 8)
	{
		app->render->DrawTexture(moneda8, 600, 40, NULL, SDL_FLIP_NONE, 0);
	}
	else if (app->scene->player->score == 9)
	{
		app->render->DrawTexture(moneda9, 600, 40, NULL, SDL_FLIP_NONE, 0);
	}
	else if (app->scene->player->score == 10)
	{
		app->render->DrawTexture(moneda10, 600, 40, NULL, SDL_FLIP_NONE, 0);
	}
	

	if (playerDeadHud && !spacePressed && app->scene->player->deathCounter != 5)
	{
		app->scene->pause = true;
		app->render->DrawTexture(deadScreenTexture, 0, 0, NULL, SDL_FLIP_NONE, 0);
		if (app->input->GetKey(SDL_SCANCODE_SPACE)==KEY_DOWN &&!spacePressed)
		{
			playerDeadHud = false;
			spacePressed = true;
			
			app->scene->pause = false;
		}
	}

	if (app->scene->player->deathCounter==5)
	{
		app->scene->pause = true;
		app->render->DrawTexture(GameOver, 0, 0, NULL, SDL_FLIP_NONE, 0);
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !spacePressed)
		{
			SDL_Quit();
		}
	}


	return true;
}

void Hud::DrawTimer()
{
	/*NO FUNCIONA EL TIMER*/
	int time = 100000 - timer.ReadMSec();
	int minutes = time / 60000;
	int seconds = (time / 1000) % 60;

	std::ostringstream timerTextStream;
	timerTextStream << std::setfill('0') << std::setw(2) << minutes << ":"
		<< std::setfill('0') << std::setw(2) << seconds;

	std::string timerTextStr = timerTextStream.str();
	const char* timerTextCStr = timerTextStr.c_str();

	app->render->DrawText(timerTextCStr, 200, 25, 100, 25);
}

bool Hud::CleanUp()
{
	LOG("Freeing Hud");

	return true;
}
