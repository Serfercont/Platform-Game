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
	name.Create("scene2");
}

// Destructor
Scene2::~Scene2()
{}

// Called before render is available
bool Scene2::Awake(pugi::xml_node& config)
{
	configNode = config;
	LOG("Loading Scene2");
	bool ret = true;
	
	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
	}
	if (config.child("map")) {
		//Get the map name from the config file and assigns the value in the module
		app->map->name = config.child("map").attribute("name").as_string();
		app->map->path = config.child("map").attribute("path").as_string();
	}
	return true;
}

// Called before the first frame
bool Scene2::Start()
{
	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file

	//Music is commented so that you can add your own music
	fondo0 = app->tex->Load(configNode.child("background").attribute("path").as_string());
	fondo1 = app->tex->Load(configNode.child("background1").attribute("path").as_string());
	fondo2 = app->tex->Load(configNode.child("background2").attribute("path").as_string());
	fondo3 = app->tex->Load(configNode.child("background3").attribute("path").as_string());
	fondo4 = app->tex->Load(configNode.child("background4").attribute("path").as_string());
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(img, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

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
	float camSpeed = 0.35;

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += (int)ceil(camSpeed * dt);

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= (int)ceil(camSpeed * dt);

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= (int)ceil(camSpeed * dt);

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += (int)ceil(camSpeed * dt);

	
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

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

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

	return true;
}
