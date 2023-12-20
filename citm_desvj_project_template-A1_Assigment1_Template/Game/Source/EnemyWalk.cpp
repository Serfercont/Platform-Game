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
#include "EntityManager.h"
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

	wolfAttack = parameters.attribute("wolfAttack").as_string();
	wolfDeath = parameters.attribute("wolfDeath").as_string();
	return true;
}

bool EnemyWalk::Start() {


	idleAnim.LoadAnimations("enemyIdle");
	walkAnim.LoadAnimations("enemyWalk");
	runAnim.LoadAnimations("enemyRun");
	attackAnim.LoadAnimations("enemyAttack");
	deadAnim.LoadAnimations("enemyDead");
	texture = app->tex->Load(texturePath);
	currentAnimation = &idleAnim;

	wolfAttacks = app->audio->LoadFx(wolfAttack);
	wolfDeaths = app->audio->LoadFx(wolfDeath);

	tileTex = app->tex->Load("Assets/Maps/tileSelection.png");

	pbody = app->physics->CreateCircle(position.x + 16, position.y - 10, 25, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::ENEMYWALK;
	pbody->listener = this;

	initialTransform = pbody->body->GetTransform();
	
	return true;
}

bool EnemyWalk::Update(float dt)
{
	flipPos.x = position.x - 10;
	origin = app->map->WorldToMap(position.x, position.y);
	destiny = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);
	
	velocity.y = 5;

	//LOG("LAST PATH X: %d enemy x: %d", destiny.x, origin.x);
	int dist = sqrt(pow(destiny.x - origin.x, 2) + pow(destiny.y - origin.y, 2));
	if (dist>12 && isAlive)
	{
		currentAnimation = &idleAnim;
	}
	if (dist<12 && isAlive)
	{
		app->map->pathfinding->CreatePath(origin, destiny);
		lastPath = *app->map->pathfinding->GetLastPath();
	}
	if (dist <= 3)
	{
		attack = true;
	}
	if (currentAnimation == &attackAnim && currentAnimation->HasFinished()) { 
		attack = false;
		attackAnim.Reset();
		currentAnimation->loopCount = 0;
	}

	if (die==true)
	{
		velocity.x = 0;
		isAlive = false;
		if (currentAnimation!=&deadAnim)
		{
			app->audio->PlayFx(wolfDeaths);
			currentAnimation = &deadAnim;
			currentAnimation->loopCount = 0;
			currentAnimation->Reset();
		}
		if (currentAnimation->HasFinished())
		{
			if (damage)
			{
				damage->body->SetActive(false);
				damage->body->GetWorld()->DestroyBody(damage->body);
				damage = NULL;
			}
			pbody->body->SetActive(false);
			app->entityManager->DestroyEntity(this);
			app->physics->world->DestroyBody(pbody->body);
		}
	}

	if (attack && currentAnimation==&attackAnim && currentAnimation->GetCurrentFrameCount()>=4 && !attackBody)
	{
		if (right)
		{
			damage = app->physics->CreateRectangleSensor(position.x + 45, position.y, 20, 40, bodyType::KINEMATIC);
			damage->listener = this;
			damage->ctype = ColliderType::ENEMYDAMAGE;
			attackBody = true;
		}
		else
		{
			damage = app->physics->CreateRectangleSensor(position.x - 30, position.y, 20, 40, bodyType::KINEMATIC);
			damage->listener = this;
			damage->ctype = ColliderType::ENEMYDAMAGE;
			attackBody = true;
		}
	}
	//LOG("current frame %i", currentAnimation->GetCurrentFrameCount());
	if (attack && currentAnimation== &attackAnim && currentAnimation->GetCurrentFrameCount()>=6.8 &&attackBody)
	{
		attack = false;
		attackBody = false;
		if (damage)
		{
			damage->body->SetActive(false);
			damage->body->GetWorld()->DestroyBody(damage->body);
			damage = NULL;
		}
	}
	if (tp)
	{
		pbody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0);
		tp = false;
	}

	if (lastPath.Count() > 0 && isAlive)
	{
		iPoint* nextPathTile;
		nextPathTile = lastPath.At(lastPath.Count() - 1);

		if (nextPathTile->x < origin.x)
		{
			right = false;
			if (attack)
			{
				app->audio->PlayFx(wolfAttacks);
				currentAnimation = &attackAnim;
				velocity.x = -2;
			}
			else
			{
				currentAnimation = &runAnim;
				velocity.x = -2;
			}
		}
		else
		{
			right = true;
			if(attack)
			{
				app->audio->PlayFx(wolfAttacks);
				currentAnimation = &attackAnim;
				velocity.x = +2;
			}
			else
			{
				currentAnimation = &runAnim;
				velocity.x = +2;
			}
			
		}
		if (nextPathTile->x == origin.x) {
			lastPath.Pop(*nextPathTile);
		}
	}
	if (!tp)
	{
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);
	}

	pbody->body->SetLinearVelocity(velocity);
	//enemyPbody->body->SetTransform({ pbody->body->GetPosition().x, pbody->body->GetPosition().y - PIXEL_TO_METERS(10) }, 0);

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	if (right)
	{
		app->render->DrawTexture(texture, position.x-70, position.y-80, &rect);
	}
	else
	{

		app->render->DrawTexture(texture, flipPos.x-50, position.y-80, &rect, 1.0f, 0, INT_MAX, INT_MAX, SDL_FLIP_HORIZONTAL);
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
bool EnemyWalk::CleanUp()
{
	return true;
}

void EnemyWalk::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype) {

	case ColliderType::DAMAGE:
		die = true;
		currentAnimation = &deadAnim;
		break;

	}
}

