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
#include "Timer.h"


Ability::Ability() : Entity(EntityType::ABILITY)
{
	name.Create("ability");
}

Ability::~Ability() {}

bool Ability::Awake() {

	
	texturePath = parameters.attribute("texturepath").as_string();
	timer = Timer();

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
	position.x = app->scene->player->position.x;
	position.y = app->scene->player->position.y;
	flipPos.x = position.x-32;
	flipPos.y = position.y;
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && app->scene->player->powerUp && !attacking)
	{
		timer.Start();
		timerPaused = false;
		
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
			currentAnimation = &attack;
			derecha = false;
		}
		attacking = true;
	}

	if (attacking && damageAb && derecha)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		position.x = METERS_TO_PIXELS(damageAb->body->GetTransform().p.x);
		position.y = METERS_TO_PIXELS(damageAb->body->GetTransform().p.y);
		app->render->DrawTexture(texture, position.x-50, position.y-16, &rect);
	}
	else if(attacking && damageAb && !derecha)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		position.x = METERS_TO_PIXELS(damageAb->body->GetTransform().p.x);
		position.y = METERS_TO_PIXELS(damageAb->body->GetTransform().p.y);
		app->render->DrawTexture(texture, position.x-50, position.y-16, &rect, 1.0f, 0, INT_MAX, INT_MAX, SDL_FLIP_HORIZONTAL);
	}

	if (attacking)
	{
		b2Vec2 currentVelocity = damageAb->body->GetLinearVelocity();
		if (!derecha)
		{
			currentVelocity.x = -speed * 16;
			damageAb->body->SetLinearVelocity(currentVelocity);
		}
		else
		{
			currentVelocity.x = speed * 16;
			damageAb->body->SetLinearVelocity(currentVelocity);
		}
		
	}

	if (timer.ReadSec() >=3 && !timerPaused) {
		timerPaused = true;
		pop = true;
	}
	
	if (pop) {
		if (damageAb) {
			attacking = false;
			damageAb->body->SetActive(false);
			damageAb->body->GetWorld()->DestroyBody(damageAb->body);
			damageAb = nullptr;
		}
		pop = false;
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

