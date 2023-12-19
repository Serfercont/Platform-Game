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
	//LOG("dist: %d", dist);
	if (dist > 12 && isAlive)
	{
		currentAnimation = &idleAnim;
	}
	if (dist < 12 && dist>2 && isAlive)
	{
		app->map->pathfinding->CreatePath(origin, destiny);
		lastPath = *app->map->pathfinding->GetLastPath();
	}
	if (dist <= 2)
	{
 		attack = true;
		//currentAnimation = &attackAnim;
	}
	if (currentAnimation == &attackAnim && currentAnimation->HasFinished()) { // Reiniciar el ataque
		attack = false;
		attackAnim.Reset();
		currentAnimation->loopCount = 0;
	}
	if (attack && currentAnimation == &attackAnim && currentAnimation->GetCurrentFrameCount() >= 2 && !attackBody)
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
	LOG("current frame %i", currentAnimation->GetCurrentFrameCount());
	if (attack && currentAnimation == &attackAnim && currentAnimation->GetCurrentFrameCount() >= 4 && attackBody)
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

	if (lastPath.Count() > 0 && isAlive)
	{
		iPoint* nextPathTile;
		nextPathTile = lastPath.At(lastPath.Count() - 1);

		if (nextPathTile->x < origin.x)
		{
			right = false;
			if (attack)
			{
				currentAnimation = &attackAnim;
				velocity.x = -2;
			}
			else
			{
				currentAnimation = &idleAnim;
				velocity.x = -2;
			}
		}
		else
		{
			right = true;
			if (attack)
			{
				currentAnimation = &attackAnim;
				velocity.x = +2;
			}
			else
			{
				currentAnimation = &idleAnim;
				velocity.x = +2;
			}
		}
		if (nextPathTile->x == origin.x) {
			lastPath.Pop(*nextPathTile);
		}
		if (nextPathTile->y < origin.y)
		{
			currentAnimation = &idleAnim;
			velocity.y = -2;
		}
		else
		{
			currentAnimation = &idleAnim;
			velocity.y = +2;
		}
	}

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);

	pbody->body->SetLinearVelocity(velocity);
	//enemyPbody->body->SetTransform({ pbody->body->GetPosition().x, pbody->body->GetPosition().y - PIXEL_TO_METERS(10) }, 0);

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
		//currentAnimation = &deadAnim1;
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