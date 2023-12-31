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

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	configNode = config;
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	/*for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}*/

	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
	}
	/*if (config.child("enemyFly")) {
		enemyFly = (EnemyFly*)app->entityManager->CreateEntity(EntityType::ENEMYFLY);
		enemyFly->parameters = config.child("enemyFly");
	}*/
	/*if (config.child("enemyWalk")) {
		enemyWalk = (EnemyWalk*)app->entityManager->CreateEntity(EntityType::ENEMYWALK);
		enemyWalk->parameters = config.child("enemyWalk");
	}*/
	for (pugi::xml_node itemNode = config.child("enemyWalk"); itemNode; itemNode = itemNode.next_sibling("enemyWalk"))
	{
		EnemyWalk* item = (EnemyWalk*)app->entityManager->CreateEntity(EntityType::ENEMYWALK);
		item->parameters = itemNode;

	}
	WolfList;

	for (pugi::xml_node itemNode = config.child("enemyFly"); itemNode; itemNode = itemNode.next_sibling("enemyFly"))
	{
		EnemyFly* item = (EnemyFly*)app->entityManager->CreateEntity(EntityType::ENEMYFLY);
		item->parameters = itemNode;

	}
	EyeList;

	app->entityManager->GetWolves(WolfList);
	app->entityManager->GetEyes(EyeList);
	if (config.child("map")) {
		//Get the map name from the config file and assigns the value in the module
		app->map->name = config.child("map").attribute("name").as_string();
		app->map->path = config.child("map").attribute("path").as_string();
	}
	
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	
	//Music is commented so that you can add your own music
	app->audio->PlayMusic(configNode.child("mainmusic").attribute("path").as_string());

	//Get the size of the window
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
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	float camSpeed = 0.35; 

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += (int)ceil(camSpeed * dt);

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= (int)ceil(camSpeed * dt);

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{

	}
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	// Renders the image in the center of the screen 
	SDL_Rect Rectfondo0{0,0,1536*4,216*4};
	SDL_Rect Rectfondo1{ 0,0,1536 * 4,216 * 4 };
	SDL_Rect Rectfondo2{ 0,0,1536 * 3,216 * 3 };
	SDL_Rect Rectfondo3{ 0,0,1536 * 3,216 * 3 };
	SDL_Rect Rectfondo4{ 0,0,1536 * 32,216 * 32 };
	app->render->DrawTexture(fondo0, -100, 0,&Rectfondo0,0.2f);
	app->render->DrawTexture(fondo1, -150, -100, &Rectfondo1, 0.4f);
	app->render->DrawTexture(fondo2, -150, 110, &Rectfondo2, 0.6f);
	app->render->DrawTexture(fondo3, -150, 110, &Rectfondo3, 0.7f);
	app->render->DrawTexture(fondo4, -150,0, &Rectfondo4, 0.4f);

	/*const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
	}*/

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

bool Scene::LoadState(pugi::xml_node node)
{
	player->position.x = node.child("player").attribute("x").as_int();
	player->position.y = node.child("player").attribute("y").as_int();
	player->pbody->SetPosition(player->position.x, player->position.y);

	player->godMode = node.child("pconditions").attribute("godmode").as_bool();
	player->isAlive = node.child("pconditions").attribute("isAlive").as_bool();
	player->isjumpping = node.child("pconditions").attribute("isjumpping").as_bool();
	player->isWalking = node.child("pconditions").attribute("isWalking").as_bool();
	player->right = node.child("pconditions").attribute("right").as_bool();
	
	player->pbody->body->SetTransform({ PIXEL_TO_METERS(player->position.x), PIXEL_TO_METERS(player->position.y) },0);
	
	//Wolf Load 
	for (int Wolfcount = 0; Wolfcount < WolfList.Count(); Wolfcount++) {

		Entity* wolf = WolfList.At(Wolfcount)->data;

		// Convert the current count to a string for constructing XML attribute names.
		std::string count = std::to_string(Wolfcount + 1);

		// Update the position of the slime entity based on XML attributes.
		wolf->position.x = node.child(("Wolf" + count).c_str()).child("wolfPosition").attribute("x").as_int();
		wolf->position.y = node.child(("Wolf" + count).c_str()).child("wolfPosition").attribute("y").as_int();
		wolf->isAlive = node.child(("Wolf" + count).c_str()).child("wolfPosition").attribute("isAlive").as_bool();
		wolf->tp = true;
	}
	//Flying Eye Load
	for (int Eyecount = 0; Eyecount < EyeList.Count(); Eyecount++) {

		Entity* eye = WolfList.At(Eyecount)->data;

		// Convert the current count to a string for constructing XML attribute names.
		std::string count = std::to_string(Eyecount + 1);

		// Update the position of the slime entity based on XML attributes.
		eye->position.x = node.child(("Eye" + count).c_str()).child("eyePosition").attribute("x").as_int();
		eye->position.y = node.child(("Eye" + count).c_str()).child("eyePosition").attribute("y").as_int();
		eye->isAlive = node.child(("Eye" + count).c_str()).child("eyePosition").attribute("isAlive").as_bool();
		eye->tp = true;
	}
	return true;
}

bool Scene::SaveState(pugi::xml_node node)
{
	pugi::xml_node posNode = node.append_child("player");
	posNode.append_attribute("x").set_value(player->position.x);
	posNode.append_attribute("y").set_value(player->position.y);

	//Save data of the Walking Enemy
	for (int wolfCount = 0; wolfCount < WolfList.Count(); wolfCount++) {
		std::string count = std::to_string(wolfCount + 1);
		Entity* wolf = WolfList.At(wolfCount)->data;
		pugi::xml_node enemyNode = node.append_child(("Wolf" + count).c_str()).append_child("wolfPosition");
		enemyNode.append_attribute("x").set_value(wolf->position.x);
		enemyNode.append_attribute("y").set_value(wolf->position.y);
		enemyNode.append_attribute("isAlive").set_value(wolf->isAlive);
	}
	//Save data of the Flying Enemy
	for (int eyeCount = 0; eyeCount < EyeList.Count(); eyeCount++) {
		std::string count = std::to_string(eyeCount + 1);
		Entity* eye = EyeList.At(eyeCount)->data;
		pugi::xml_node enemyEyeNode = node.append_child(("Eye" + count).c_str()).append_child("eyePosition");
		enemyEyeNode.append_attribute("x").set_value(eye->position.x);
		enemyEyeNode.append_attribute("y").set_value(eye->position.y);
		enemyEyeNode.append_attribute("isAlive").set_value(eye->isAlive);
	}


	pugi::xml_node pconditionsNode = node.append_child("pconditions");
	pconditionsNode.append_attribute("godmode").set_value(player->godMode);
	pconditionsNode.append_attribute("isAlive").set_value(player->isAlive);
	pconditionsNode.append_attribute("isjumpping").set_value(player->isjumpping);
	pconditionsNode.append_attribute("isWalking").set_value(player->isWalking);
	pconditionsNode.append_attribute("right").set_value(player->right);

	return true;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(fondo0);
	app->tex->UnLoad(fondo1);
	app->tex->UnLoad(fondo2);
	app->tex->UnLoad(fondo3);
	app->tex->UnLoad(fondo4);

	return true;
}
