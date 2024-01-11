#include "Ability.h"
#include "Recover.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Player.h"


Ability::Ability() : Entity(EntityType::ABILITY)
{
	name.Create("ability");
}

Ability::~Ability() {}

bool Ability::Awake() {

	
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Ability::Start() {

	//initilize textures
	attack.LoadAnimations("ability");
	
	texture = app->tex->Load(texturePath);

	return true;
}

bool Ability::Update(float dt)
{
	position.x = app->scene->player->position.x;
	position.y = app->scene->player->position.y;

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && app->scene->player->powerUp)
	{
		LOG("W pulsada");
		if (app->scene->player->right)
		{
			pbody = app->physics->CreateCircleSensor(position.x + 16, position.y + 16, 10, bodyType::KINEMATIC);
			pbody->ctype = ColliderType::ABILITY;
			pbody->listener = this;
			currentAnimation = &attack;
			SDL_Rect rect = currentAnimation->GetCurrentFrame();
			app->render->DrawTexture(texture, position.x, position.y, &rect);
		}
		else
		{
			pbody = app->physics->CreateCircleSensor(position.x - 16, position.y + 16, 10, bodyType::KINEMATIC);
			pbody->ctype = ColliderType::ABILITY;
			pbody->listener = this;
			currentAnimation = &attack;
			SDL_Rect rect = currentAnimation->GetCurrentFrame();
			app->render->DrawTexture(texture, flipPos.x, position.y, &rect, 1.0f, 0, INT_MAX, INT_MAX, SDL_FLIP_HORIZONTAL);
		}
		
	}
	

	if (pop)
	{
		pbody->body->SetActive(false);
		app->entityManager->DestroyEntity(this);
		app->physics->world->DestroyBody(pbody->body);
	}

	return true;
}
void Ability:: Move()
{
	b2Vec2 currentVelocity = pbody->body->GetLinearVelocity();
	if (app->scene->player->right)
	{
		currentAnimation = &attack;
		currentVelocity.x = +speed * 10;
	}
}

bool Ability::CleanUp()
{
	return true;
}

void Ability::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::ENEMYFLY:
		pop = true;
		break;
	case ColliderType::ENEMYWALK:
		pop = true;
		break;
	case ColliderType::COLUMN:
		pop = true;
		break;
	}
}
