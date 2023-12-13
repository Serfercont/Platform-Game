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
#include "DynArray.h"
#include "Map.h"
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
	currentAnimation = &idleAnim;
	origin = app->map->WorldToMap(position.x, position.y);
	destiny = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);

	b2Vec2 currentVelocity = pbody->body->GetLinearVelocity();
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
	currentVelocity.y += 0.5;

	int dist = sqrt(pow(destiny.x - origin.x, 2) + pow(destiny.y - origin.y, 2));

	if (dist<4)
	{
		currentVelocity.y = 0;
		pbody->body->SetLinearVelocity(currentVelocity);
	}
	else if (dist>=4 && dist<=5)
	{
		app->map->pathfinding->CreatePath(origin, destiny);
		const DynArray<iPoint>* Lastpath = app->map->pathfinding->GetLastPath();
	}
	
	return true;
}

bool EnemyWalk::CleanUp()
{
	return true;
}

void EnemyWalk::OnCollision(PhysBody* physA, PhysBody* physB) {

	
	}