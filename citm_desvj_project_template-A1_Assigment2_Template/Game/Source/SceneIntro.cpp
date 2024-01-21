#include "SceneIntro.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Physics.h"


#include "Defs.h"
#include "Log.h"

SceneIntro::SceneIntro() : Module()
{
	name.Create("scene");
}

// Destructor
SceneIntro::~SceneIntro()
{}

// Called before render is available
bool SceneIntro::Awake(pugi::xml_node& config)
{
	configNode = config;
	LOG("Loading SceneIntro");
	bool ret = true;

	
	return ret;
}

// Called before the first frame
bool SceneIntro::Start()
{
	
	//app->audio->PlayMusic(configNode.child("mainmusic").attribute("path").as_string());

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	return true;
}

// Called each loop iteration
bool SceneIntro::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneIntro::Update(float dt)
{

	/*if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		;*/

	

	return true;
}

// Called each loop iteration
bool SceneIntro::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}


// Called before quitting
bool SceneIntro::CleanUp()
{
	LOG("Freeing scene");
	
	return true;
}
