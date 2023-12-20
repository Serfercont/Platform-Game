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


	idleAnim.LoadAnimations("flyIdle");
	hitAnim.LoadAnimations("flyHit");
	attackAnim.LoadAnimations("flyAttack");
	deadAnim1.LoadAnimations("flyDeath1");
	deadAnim2.LoadAnimations("flyDeath2");

	texture = app->tex->Load(texturePath);
	currentAnimation = &idleAnim;

	tileTex = app->tex->Load("Assets/Maps/tileSelection.png");

	pbody = app->physics->CreateCircle(position.x + 16, position.y - 10, 16, bodyType::DYNAMIC);
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
	

	//LOG("LAST PATH X: %d enemy x: %d", destiny.x, origin.x);
	int dist = sqrt(pow(destiny.x - origin.x, 2) + pow(destiny.y - origin.y, 2));
	int distX = destiny.x - origin.x;
	int distY = destiny.y - origin.y;
	//LOG("dist: %d", dist);
	if (dist > 12 && isAlive)
	{
		currentAnimation = &idleAnim;
	}
	if (dist < 12 && isAlive)
	{
		//currentAnimation = &idleAnim;
		app->map->pathfinding->CreatePath(origin, destiny);
		lastPath = *app->map->pathfinding->GetLastPath();
	}
	if (dist <= 2)
	{
		attack = true;

	}
	//cuando acaba la anim, acaba de atacar
	if (currentAnimation == &attackAnim && currentAnimation->HasFinished()) {
		attack = false;
		attackAnim.Reset();
		currentAnimation->loopCount = 0;
	}
	//ataque
	if (attack && currentAnimation == &attackAnim && currentAnimation->GetCurrentFrameCount() >= 2 && !attackBody)
	{
		if (right)
		{
			damage = app->physics->CreateRectangleSensor(position.x + 25, position.y, 20, 20, bodyType::KINEMATIC);
			damage->listener = this;
			damage->ctype = ColliderType::ENEMYDAMAGE;
			attackBody = true;
		}
		else
		{
			damage = app->physics->CreateRectangleSensor(position.x - 30, position.y, 20, 20, bodyType::KINEMATIC);
			damage->listener = this;
			damage->ctype = ColliderType::ENEMYDAMAGE;
			attackBody = true;
		}
	}
	if (attack && currentAnimation == &attackAnim && currentAnimation->GetCurrentFrameCount() >= 7 && attackBody)
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
	//caida muerte
	if (fall == true)
	{
		velocity.x = 0;
		velocity.y++;
		isAlive = false;
		if (currentAnimation != &deadAnim1)
		{
			currentAnimation = &deadAnim1;
		}
	}
	//muerte
	if (die== true)
	{
		velocity.x = 0;
		if (currentAnimation != &deadAnim2)
		{
			currentAnimation = &deadAnim2;
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
	//pathfinding y attackAnim
	

		if (lastPath.Count() > 1 && isAlive)
		{
			iPoint nextTilePath = { lastPath.At(1)->x, lastPath.At(1)->y };

			int positionTilesX = position.x / 32;
			int positionTilesY = position.y / 32;

			if (nextTilePath.x < positionTilesX && nextTilePath.x != positionTilesX)
			{
				right = false;
				velocity.x = -2;
				if (attack)
				{
					currentAnimation = &attackAnim;
				}
				else
				{
					currentAnimation = &idleAnim;
				}
			}
			else if (nextTilePath.x > positionTilesX && nextTilePath.x != positionTilesX)
			{
				right = true;
				velocity.x = 2;
				if (attack)
				{
					currentAnimation = &attackAnim;
				}
				else
				{
					currentAnimation = &idleAnim;
				}

			}
			else if (nextTilePath.y > positionTilesY && nextTilePath.y !=positionTilesY)
			{
				velocity.y = 2;
				velocity.x = 0;
			}
			else if (nextTilePath.y < positionTilesY && nextTilePath.y != positionTilesY)
			{
				velocity.y = -2;
				velocity.x = 0;
			}
		}


	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);

	pbody->body->SetLinearVelocity(velocity);
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	if (right)
	{
		app->render->DrawTexture(texture, position.x - 20, position.y - 40, &rect);
	}
	else
	{
		app->render->DrawTexture(texture, flipPos.x - 20, position.y - 40, &rect, 1.0f, 0, INT_MAX, INT_MAX, SDL_FLIP_HORIZONTAL);
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
	switch (physB->ctype) {

	case ColliderType::DAMAGE:
		fall = true;
		break;
	case ColliderType::PLATFORM:
		if (!isAlive)
		{
			die = true;
			fall = false;
		}
		break;

	}

}