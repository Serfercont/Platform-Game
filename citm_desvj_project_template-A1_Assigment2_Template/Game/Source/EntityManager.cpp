#include "EntityManager.h"
#include "Player.h"
#include "EnemyWalk.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* entity = nullptr; 

	switch (type)
	{
	case EntityType::PLAYER:
		entity = new Player();
		break;
	case EntityType::ITEM:
		entity = new Item();
		break;
	case EntityType::ENEMYWALK:
		entity = new EnemyWalk();
		break;
	case EntityType::ENEMYFLY:
		entity = new EnemyFly();
		break;
	case EntityType::RECOVER:
		entity = new Recover();
		break;
	default:
		break;
	}

	entities.Add(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.Del(item);
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}
void EntityManager::GetWolves(List<Entity*>& wolvesList) const
{
	wolvesList.Clear();

	ListItem<Entity*>* entity;

	for (entity = entities.start; entity != NULL; entity = entity->next)
	{
		if (entity->data->type == EntityType::ENEMYWALK)
		{
			wolvesList.Add(entity->data);
		}
	}
}
void EntityManager::GetEyes(List<Entity*>& eyesList) const
{
	eyesList.Clear();

	ListItem<Entity*>* entity;

	for (entity = entities.start; entity != NULL; entity = entity->next)
	{
		if (entity->data->type == EntityType::ENEMYFLY)
		{
			eyesList.Add(entity->data);
		}
	}
}
void EntityManager::GetItems(List<Entity*>& itemsList) const
{
	itemsList.Clear();

	ListItem<Entity*>* entity;

	for (entity = entities.start; entity != NULL; entity = entity->next)
	{
		if (entity->data->type == EntityType::ITEM)
		{
			itemsList.Add(entity->data);
		}
	}
}
void EntityManager::GetRecovers(List<Entity*>& itemsList) const
{
	itemsList.Clear();

	ListItem<Entity*>* entity;

	for (entity = entities.start; entity != NULL; entity = entity->next)
	{
		if (entity->data->type == EntityType::RECOVER)
		{
			itemsList.Add(entity->data);
		}
	}
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Update(dt);
	}

	return ret;
}
