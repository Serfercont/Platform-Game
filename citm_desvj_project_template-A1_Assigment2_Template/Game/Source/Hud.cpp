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

	lifeBarTexture = app->tex->Load("Assets/Textures/ghost_shadow.png");
	deadScreenTexture = app->tex->Load("Assets/UI/GameOver.png");

	timer = Timer();
	timer.Start();

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;


	return true;
}

bool Hud::Update(float dt)
{
	//if (app->scene->player->health == 100) app->render->DrawTexture(lifeBarTexture, 0, 0, NULL, SDL_FLIP_NONE, 0);
	SDL_Rect RectfondoInicial{ 0,0,1244,780 };
	DrawTimer();

	if (playerDeadHud && !spacePressed)
	{
		app->scene->pause = true;
		app->render->DrawTexture(deadScreenTexture, -100, 0, &RectfondoInicial, SDL_FLIP_NONE, 0);
		if (app->input->GetKey(SDL_SCANCODE_SPACE)==KEY_DOWN &&!spacePressed)
		{
			playerDeadHud = false;
			spacePressed = true;
			
			app->scene->pause = false;
		}
	}


	return true;
}

void Hud::DrawTimer()
{
	int time = 100000 - timer.ReadMSec();
	int minutes = time / 60000;
	int seconds = (time / 1000) % 60;

	std::ostringstream timerTextStream;
	timerTextStream << std::setfill('0') << std::setw(2) << minutes << ":"
		<< std::setfill('0') << std::setw(2) << seconds;

	std::string timerTextStr = timerTextStream.str();
	const char* timerTextCStr = timerTextStr.c_str();

	//app->render->DrawText(timerTextCStr, 200, 25, 100, 25);
}

bool Hud::CleanUp()
{
	LOG("Freeing Hud");

	return true;
}
