#include "EnemyFly.h"
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
#include "EntityManager.h"
#include "Map.h"
#include <map>
#include <chrono>
#include <thread>

EnemyFly::EnemyFly() : Entity(EntityType::ENEMYFLY)
{
	name.Create("EnemyFly");
}

EnemyFly::~EnemyFly()
{
}

bool EnemyFly::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	return true;
}

bool EnemyFly::Start() {


	idleAnim.LoadAnimations("enemyIdle");
	/*walkAnim.LoadAnimations("enemyWalk");
	runAnim.LoadAnimations("enemyRun");
	attackAnim.LoadAnimations("enemyAttack");*/
	texture = app->tex->Load(texturePath);
	currentAnimation = &idleAnim;

	tileTex = app->tex->Load("Assets/Maps/tileSelection.png");

	pbody = app->physics->CreateCircle(position.x + 16, position.y - 10, 25, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::ENEMYFLY;
	pbody->listener = this;
	initialTransform = pbody->body->GetTransform();

	return true;
}

bool EnemyFly::Update(float dt)
{
	flipPos.x = position.x - 10;
	origin = app->map->WorldToMap(position.x, position.y);
	destiny = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);

	velocity.y = 5;

	LOG("LAST PATH X: %d enemy x: %d", destiny.x, origin.x);
	int dist = sqrt(pow(destiny.x - origin.x, 2) + pow(destiny.y - origin.y, 2));

	if (dist < 10)
	{
		//currentAnimation = &idleAnim;
		app->map->pathfinding->CreatePath(origin, destiny);
		lastPath = *app->map->pathfinding->GetLastPath();
		if (dist <= 2 && !attack)
		{
			attack = true;
			//currentAnimation = &attackAnim;
		}
	}
	if (attack && isAlive)
	{

	}

	if (!isAlive)
	{
		pbody->body->SetActive(false);
		app->entityManager->DestroyEntity(this);
		app->physics->world->DestroyBody(pbody->body);
	}

	if (lastPath.Count() > 0)
	{
		iPoint* nextPathTile;
		nextPathTile = lastPath.At(lastPath.Count() - 1);

		if (nextPathTile->x < origin.x)
		{
			right = false;
			//currentAnimation = &runAnim;
			velocity.x = -3;
		}
		else
		{
			right = true;
			//currentAnimation = &runAnim;
			velocity.x = +3;
		}
		if (nextPathTile->x == origin.x) {
			lastPath.Pop(*nextPathTile);
		}
	}

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);

	pbody->body->SetLinearVelocity(velocity);
	//enemyPbody->body->SetTransform({ pbody->body->GetPosition().x, pbody->body->GetPosition().y - PIXEL_TO_METERS(10) }, 0);

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	if (right)
	{
		app->render->DrawTexture(texture, position.x - 50, position.y - 47, &rect);
	}
	else
	{

		app->render->DrawTexture(texture, flipPos.x - 50, position.y - 47, &rect, 1.0f, 0, INT_MAX, INT_MAX, SDL_FLIP_HORIZONTAL);
	}
	currentAnimation->Update();

	if (app->physics->debug)
	{
		const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			app->render->DrawTexture(tileTex, pos.x, pos.y);
		}
	}
	return true;
}

bool EnemyFly::CleanUp()
{
	return true;
}

void EnemyFly::OnCollision(PhysBody* physA, PhysBody* physB) {


}