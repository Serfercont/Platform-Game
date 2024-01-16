#include "Scene2.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "Physics.h"
#include "EnemyWalk.h"
#include "Recover.h"
#include "PowerUp.h"

#include "Defs.h"
#include "Log.h"

Scene2::Scene2() : Module()
{
	name.Create("scene");
}

// Destructor
Scene2::~Scene2()
{}

// Called before render is available
bool Scene2::Awake(pugi::xml_node& config)
{
	
	return true;
}

// Called before the first frame
bool Scene2::Start()
{
	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file

	//Music is commented so that you can add your own music
	
	return true;
}

// Called each loop iteration
bool Scene2::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene2::Update(float dt)
{
	

	/*const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
	}*/

	return true;
}

// Called each loop iteration
bool Scene2::PostUpdate()
{
	bool ret = true;

	

	return ret;
}

bool Scene2::LoadState(pugi::xml_node node)
{
	
	return true;
}
bool Scene2::SaveState(pugi::xml_node node)
{

	return true;
}

// Called before quitting
bool Scene2::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(fondo0);
	app->tex->UnLoad(fondo1);
	app->tex->UnLoad(fondo2);
	app->tex->UnLoad(fondo3);
	app->tex->UnLoad(fondo4);

	return true;
}
