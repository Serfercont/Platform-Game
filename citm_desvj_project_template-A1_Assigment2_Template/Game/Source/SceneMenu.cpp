#include "SceneMenu.h"
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

#include "Defs.h"
#include "Log.h"

SceneMenu::SceneMenu(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("sceneMenu");
}

// Destructor
SceneMenu::~SceneMenu()
{}

// Called before render is available
bool SceneMenu::Awake(pugi::xml_node& config)
{
	configNode = config;
	LOG("Loading SceneMenu");
	bool ret = true;


	return ret;
}

// Called before the first frame
bool SceneMenu::Start()
{
	LOG("Loading SceneMenu Assets");
	bool ret = true;
	MainMenu = app->tex->Load("Assets/Textures/Fondo Juego.png");
	

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture


	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	return true;
}

// Called each loop iteration
bool SceneMenu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneMenu::Update(float dt)
{
	//Draw
	SDL_Rect RectfondoInicial{ 0,0,windowW,windowH };
	app->render->DrawTexture(MainMenu, 0, 0, NULL, SDL_FLIP_NONE, 0);
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->fade->FadeToBlackScene(this, (Module*)app->scene, 60);
		//goTimer = true;


	}

	return true;
}

// Called each loop iteration
bool SceneMenu::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}


// Called before quitting
bool SceneMenu::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

