#include "PowerUp.h"
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

PowerUp::PowerUp() : Entity(EntityType::POWERUP)
{
	name.Create("powerUp");
}

PowerUp::~PowerUp() {}

bool PowerUp::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool PowerUp::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	powerUpAnim.LoadAnimations("powerUpAnim");
	pbody = app->physics->CreateCircleSensor(position.x + 16, position.y + 16, 10, bodyType::KINEMATIC);
	pbody->ctype = ColliderType::POWERUP;
	pbody->listener = this;
	
	currentAnimation = &powerUpAnim;
	return true;
}

bool PowerUp::Update(float dt)
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y,&rect);

	if (pop)
	{
		pbody->body->SetActive(false);
		app->entityManager->DestroyEntity(this);
		app->physics->world->DestroyBody(pbody->body);
	}
	powerUpAnim.Update();
	return true;
}

bool PowerUp::CleanUp()
{
	return true;
}

void PowerUp::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		pop = true;
		break;
	}
}
