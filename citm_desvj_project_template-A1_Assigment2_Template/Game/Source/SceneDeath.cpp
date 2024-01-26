#include "SceneDeath.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Map.h"	
#include "EntityManager.h"	
#include "Render.h"
#include "Window.h"
#include "Physics.h"
#include "FadeToBlack.h"
#include "GuiManager.h"

#include "Defs.h"
#include "Log.h"

SceneDeath::SceneDeath(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("sceneDeath");
}

// Destructor
SceneDeath::~SceneDeath()
{}

// Called before render is available
bool SceneDeath::Awake(pugi::xml_node& config)
{
	configNode = config;
	LOG("Loading SceneDeath");
	bool ret = true;


	return ret;
}

// Called before the first frame
bool SceneDeath::Start()
{
	LOG("Loading SceneDeath Assets");
	bool ret = true;
	GameOver = app->tex->Load("Assets/UI/GameOver.png");
	app->audio->PlayMusic(configNode.child("gameOver").attribute("path").as_string());

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture


	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	return true;
}

// Called each loop iteration
bool SceneDeath::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneDeath::Update(float dt)
{
	//Draw
	SDL_Rect RectfondoInicial{ 0,0,windowW,windowH };
	app->render->DrawTexture(GameOver, 0, 0, NULL, SDL_FLIP_NONE, 0);
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		SDL_Quit();
		//goTimer = true;


	}

	return true;
}

// Called each loop iteration
bool SceneDeath::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}


// Called before quitting
bool SceneDeath::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

