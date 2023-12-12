#include "EnemyWalk.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include <map>
#include <chrono>
#include <thread>

EnemyWalk::EnemyWalk() : Entity(EntityType::ENEMYWALK)
{
	name.Create("EnemyWalk");
}

EnemyWalk::~EnemyWalk()
{
}

bool EnemyWalk::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	return true;
}

bool EnemyWalk::Start() {


	idleAnim.LoadAnimations("enemyIdle");
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateCircle(position.x + 16, position.y - 10, 25, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMYWALK;
	
	return true;
}

bool EnemyWalk::Update(float dt)
{

	/*const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
	}*/
	return true;
}

bool EnemyWalk::CleanUp()
{
	return true;
}

void EnemyWalk::OnCollision(PhysBody* physA, PhysBody* physB) {

	
	}