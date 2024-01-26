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
#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene(App* app, bool start_enabled) : Module(app,start_enabled)
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
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}
	ItemList;

	for (pugi::xml_node itemNode = config.child("recover"); itemNode; itemNode = itemNode.next_sibling("recover"))
	{
		Recover* item = (Recover*)app->entityManager->CreateEntity(EntityType::RECOVER);
		item->parameters = itemNode;
	}
	RecoverList;

	for (pugi::xml_node itemNode = config.child("powerUp"); itemNode; itemNode = itemNode.next_sibling("powerUp"))
	{
		PowerUp* item = (PowerUp*)app->entityManager->CreateEntity(EntityType::POWERUP);
		item->parameters = itemNode;
	}
	PowerUpList;

	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
	}
	if (config.child("finalBoss")) {
		enemyBoss = (EnemyBoss*)app->entityManager->CreateEntity(EntityType::ENEMYBOSS);
		enemyBoss->parameters = config.child("finalBoss");
	}
	if (config.child("ability")) {
		ability = (Ability*)app->entityManager->CreateEntity(EntityType::ABILITY);
		ability->parameters = config.child("ability");
	}
	
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
	app->entityManager->GetItems(ItemList);
	app->entityManager->GetRecovers(RecoverList);
	app->entityManager->GetPowerUps(PowerUpList);
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

	parallax1 = app->tex->Load(configNode.child("parallax21").attribute("path").as_string());
	parallax2 = app->tex->Load(configNode.child("parallax22").attribute("path").as_string());
	parallax3 = app->tex->Load(configNode.child("parallax23").attribute("path").as_string());
	parallax4 = app->tex->Load(configNode.child("parallax24").attribute("path").as_string());
	parallax5 = app->tex->Load(configNode.child("parallax25").attribute("path").as_string());
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

	//app->fade->FadeToBlackFunction(2, 60);

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

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += (int)ceil(camSpeed * dt);

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= (int)ceil(camSpeed * dt);

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

	SDL_Rect Rectparallax0{ 0,0,1600,6400 };
	SDL_Rect Rectparallax1{ 0,0,1600 ,6400};
	SDL_Rect Rectparallax2{ 0,0,1600,6400};
	SDL_Rect Rectparallax3{ 0,0,1600 ,6400};
	SDL_Rect Rectparallax4{ 0,0,1600,6400};
	
	
	if (player->scene2Active)
	{
		app->tex->UnLoad(fondo0);
		app->tex->UnLoad(fondo1);
		app->tex->UnLoad(fondo2);
		app->tex->UnLoad(fondo3);
		app->tex->UnLoad(fondo4);
		app->render->DrawTexture(parallax5, 4000, 0, &Rectparallax4, 0);
		app->render->DrawTexture(parallax4, 5120, 0, &Rectparallax3, 0);
		app->render->DrawTexture(parallax3, 5120, 0, &Rectparallax2, 0);
		app->render->DrawTexture(parallax2, 5120, 0, &Rectparallax1, 0);
		app->render->DrawTexture(parallax1, 5120, 0, &Rectparallax0, 0);
	}
	else if (!player->scene2Active)
	{
		app->render->DrawTexture(fondo0, -100, 0, &Rectfondo0, 0.2f);
		app->render->DrawTexture(fondo1, -150, -100, &Rectfondo1, 0.4f);
		app->render->DrawTexture(fondo2, -150, 110, &Rectfondo2, 0.6f);
		app->render->DrawTexture(fondo3, -150, 110, &Rectfondo3, 0.7f);
		app->render->DrawTexture(fondo4, -150, 0, &Rectfondo4, 0.4f);
	}


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

void Scene::GoLvl1()
{
	player->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(spawn1.x), PIXEL_TO_METERS(spawn1.y)), 0);
	level1 = true;
	level2 = false;
	player->scene2Active = false;
	app->render->camera.x =0;
	app->render->camera.y = -30;
}

void Scene::GoLvl2() 
{
	player->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(spawn2.x), PIXEL_TO_METERS(spawn2.y)), 0);
	level1 = false;
	level2 = true;
	player->scene2Active = true;
	app->render->camera.x = -5124;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	// Release all items
	for (ListItem<Entity*>* item = ItemList.start; item != nullptr; item = item->next)
	{
		app->entityManager->DestroyEntity(item->data);
	}
	ItemList.Clear();

	// Release all recovers
	for (ListItem<Entity*>* item = RecoverList.start; item != nullptr; item = item->next)
	{
		app->entityManager->DestroyEntity(item->data);
	}
	RecoverList.Clear();

	// Release all power-ups
	for (ListItem<Entity*>* item = PowerUpList.start; item != nullptr; item = item->next)
	{
		app->entityManager->DestroyEntity(item->data);
	}
	PowerUpList.Clear();

	// Release the player
	if (player != nullptr)
	{
		app->entityManager->DestroyEntity(player);
		player = nullptr;
	}

	// Release the ability (if it exists)
	if (ability != nullptr)
	{
		app->entityManager->DestroyEntity(ability);
		ability = nullptr;
	}

	// Release all wolves
	for (ListItem<Entity*>* item = WolfList.start; item != nullptr; item = item->next)
	{
		app->entityManager->DestroyEntity(item->data);
	}
	WolfList.Clear();

	// Release all flying eyes
	for (ListItem<Entity*>* item = EyeList.start; item != nullptr; item = item->next)
	{
		app->entityManager->DestroyEntity(item->data);
	}
	EyeList.Clear();

	// Release the map
	//app->map->CleanUp();

	// Unload textures
	app->tex->UnLoad(fondo0);
	app->tex->UnLoad(fondo1);
	app->tex->UnLoad(fondo2);
	app->tex->UnLoad(fondo3);
	app->tex->UnLoad(fondo4);

	app->tex->UnLoad(parallax1);
	app->tex->UnLoad(parallax2);
	app->tex->UnLoad(parallax3);
	app->tex->UnLoad(parallax4);
	app->tex->UnLoad(parallax5);
	return true;
}
