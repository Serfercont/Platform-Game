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
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && app->scene->player->powerUp && !attacking)
	{
		LOG("W pulsada");
		if (app->scene->player->right)
		{
			damageAb = app->physics->CreateRectangleSensor(position.x + 50, position.y + 8, 20, 40, bodyType::KINEMATIC);
			damageAb->listener = this;
			damageAb->ctype = ColliderType::DAMAGE;
			currentAnimation = &attack;
			derecha = true;
		}
		else
		{
			damageAb = app->physics->CreateRectangleSensor(position.x - 50, position.y + 8, 20, 40, bodyType::KINEMATIC);
			damageAb->listener = this;
			damageAb->ctype = ColliderType::DAMAGE;
			derecha = false;
		}
		attacking = true;
	}

	if (attacking && damageAb)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		b2Vec2 pos = damageAb->body->GetPosition();
		app->render->DrawTexture(texture, pos.x, pos.y, &rect);
	}

	if (attacking)
	{
		if (!derecha)
		{
			b2Vec2 currentVelocity = damageAb->body->GetLinearVelocity();
			currentVelocity.x = -speed * 16;
			damageAb->body->SetLinearVelocity(currentVelocity);
		}
		else
		{
			b2Vec2 currentVelocity = damageAb->body->GetLinearVelocity();
			currentVelocity.x = speed * 16;
			damageAb->body->SetLinearVelocity(currentVelocity);
		}
		
	}
	
	if (pop)
	{
		if (damageAb)
		{
			attacking = false;
			damageAb->body->SetActive(false);
			// No necesitas destruir el cuerpo aquí, puedes hacerlo durante la limpieza.
			damageAb->body->GetWorld()->DestroyBody(damageAb->body);
			damageAb = NULL;
		}
		pop = false; // Restablecer la bandera después de limpiar.
	}


	return true;
}

bool Ability::CleanUp()
{
	if (damageAb)
	{
		damageAb->body->GetWorld()->DestroyBody(damageAb->body);
		damageAb = nullptr;
	}

	app->tex->UnLoad(texture);

	return true;
}


void Ability::OnCollision(PhysBody* physA, PhysBody* physB) {
	if (!pop) {
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
		case ColliderType::PLATFORM:
			pop = true;
			break;
		}
	}
}

