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
	position.x = app->scene->player->position.x;
	position.y = app->scene->player->position.y;


	//pbody->body->SetActive(false);

	return true;
}

bool Ability::Update(float dt)
{
	
	position.x = app->scene->player->position.x+50;
	position.y = app->scene->player->position.y+8;
	flipPos.x = position.x - 32;
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && app->scene->player->powerUp)
	{
		LOG("W pulsada");
		if (app->scene->player->right)
		{
			damage = app->physics->CreateRectangleSensor(position.x + 50, position.y + 8, 20, 40, bodyType::KINEMATIC);
			damage->listener = this;
			damage->ctype = ColliderType::DAMAGE;
			currentAnimation = &attack;
			SDL_Rect rect = currentAnimation->GetCurrentFrame();
			app->render->DrawTexture(texture, position.x, position.y, &rect);
		}
		else
		{
			damage = app->physics->CreateRectangleSensor(position.x - 50, position.y + 8, 20, 40, bodyType::KINEMATIC);
			damage->listener = this;
			damage->ctype = ColliderType::DAMAGE;
		}
		
	}
	if (pop)
	{
		attacking = false;
		damage->body->SetActive(false);
		app->entityManager->DestroyEntity(this);
		app->physics->world->DestroyBody(damage->body);
	}

	return true;
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
