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

Recover::Recover() : Entity(EntityType::RECOVER)
{
	name.Create("recover");
}

Recover::~Recover() {}

bool Recover::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	potionAnim.LoadAnimations("potionAnim");
	currentAnimation = &potionAnim;
	return true;
}

bool Recover::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircleSensor(position.x + 16, position.y + 16, 10, bodyType::KINEMATIC);
	pbody->ctype = ColliderType::RECOVER;
	pbody->listener = this;

	return true;
}

bool Recover::Update(float dt)
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);

	if (pop)
	{
		pbody->body->SetActive(false);
		app->entityManager->DestroyEntity(this);
		app->physics->world->DestroyBody(pbody->body);
	}
	potionAnim.Update();
	return true;
}

bool Recover::CleanUp()
{
	return true;
}

void Recover::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		pop = true;
		break;
	}
}
