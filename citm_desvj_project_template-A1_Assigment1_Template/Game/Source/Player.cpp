#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include <map>

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	vidas = parameters.attribute("vidas").as_int();

	return true;
}

bool Player::Start() {

	idleAnim.LoadAnimations("idle");

	walkAnim.LoadAnimations("walk");
	
	atack1Anim.LoadAnimations("attack1");
	deadAnim.LoadAnimations("dead");
	jumpAnim.LoadAnimations("jump");



	//idleAnim.PushBack({ 98, 468, 47, 65 });
	//idleAnim.PushBack({ 165, 468, 47, 65 });
	//idleAnim.PushBack({ 232, 468, 47, 65 });
	//idleAnim.PushBack({ 299, 468, 47, 65 });
	//idleAnim.speed = 0.1f;
	//idleAnim.loop = true;

	//walkAnim.PushBack({94,44,52,66});
	//walkAnim.PushBack({160,44,52,66});
	//walkAnim.PushBack({231,44,52,66});
	//walkAnim.PushBack({305,44,52,66});
	//walkAnim.PushBack({377,44,52,66});
	//walkAnim.PushBack({429,44,52,66});
	//walkAnim.PushBack({499,44,52,66});
	//walkAnim.PushBack({567,44,52,66});
	//walkAnim.speed = 0.1f;
	//walkAnim.loop = true;

	//atack1Anim.PushBack({ 77,823,87,76 });
	//atack1Anim.PushBack({ 154,823,87,76 });
	//atack1Anim.PushBack({ 238,823,87,76 });
	//atack1Anim.PushBack({ 328,823,87,76 });
	//atack1Anim.PushBack({400,823,87,76});
	//atack1Anim.speed = 0.1f;
	//atack1Anim.loop = true;

	//deadAnim.PushBack({ 91,743,65,66 });
	//deadAnim.PushBack({ 164,743,65,66 });
	//deadAnim.PushBack({ 246,743,65,66 });
	//deadAnim.PushBack({ 325,743,65,66 });
	//deadAnim.PushBack({ 402,743,65,66 });
	//deadAnim.PushBack({ 478,743,65,66 });
	//deadAnim.speed = 0.1f;
	//deadAnim.loop = false;

	//jumpAnim.PushBack({ 97,197,56,66 });
	//jumpAnim.PushBack({ 175,197,56,66 });
	//jumpAnim.PushBack({ 247,197,56,66 });
	//jumpAnim.PushBack({ 324,197,75,66 });
	//jumpAnim.PushBack({ 400,197,75,66 });
	//jumpAnim.PushBack({ 480,197,75,66 });
	//jumpAnim.speed = 0.1f;
	//jumpAnim.loop = true;
	//
	//initilize textures
	texture = app->tex->Load(texturePath);
	currentAnimation = &idleAnim;

	/*pbody = app->physics->CreateRectangle(position.x, position.y, 44,64, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;*/

	pbody = app->physics->CreateCircle(position.x + 16, position.y-10, 25, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	//pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	
	return true;
}

bool Player::Update(float dt)
{
	flipPos.x = position.x - 10;
	b2Vec2 currentVelocity = pbody->body->GetLinearVelocity();
	//b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);

	//currentAnimation = &idleAnim;
	
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE && isAlive)
	{
		currentVelocity.x = 0;
		currentAnimation = &idleAnim;
	}

	if (app->input->GetKey(SDL_SCANCODE_W && isAlive) == KEY_REPEAT) {
		currentAnimation = &atack1Anim;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && isAlive ) {
		right = false;
		isWalking = true;
		//vel = b2Vec2(-speed*dt, -GRAVITY_Y);
		currentVelocity.x = -speed * dt;
		currentAnimation = &walkAnim;
		if (!position.x--)
		{
			isWalking = false;
			currentAnimation = &idleAnim;
		}
		//currentAnimation = &animations["walk"];
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && isAlive) {
		right = true;
		isWalking = true;
		currentVelocity.x = +speed * dt;
		//vel = b2Vec2(speed*dt, -GRAVITY_Y);
		currentAnimation = &walkAnim; 
		
	}
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !isjumpping && isAlive && !checkColumn) {
		isjumpping = true;
		currentAnimation = &jumpAnim;
		currentVelocity.y = -0.5 * dt;
		pbody->body->SetLinearVelocity(currentVelocity);
	}
	if (isjumpping)
	{
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE && isAlive)
		{
			currentVelocity.x = 0;
			currentAnimation = &jumpAnim;
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && isAlive)
		{
			right = false;
			currentVelocity.x = -speed * dt;
			currentAnimation = &jumpAnim;
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && isAlive)
		{
			right = true;
			currentVelocity.x = +speed * dt;
			currentAnimation = &jumpAnim;
		}
	}
	

	//Set the velocity of the pbody of the player

	if (isjumpping == false)
	{
		currentVelocity.y = -GRAVITY_Y;
	}

	if (vidas==0)
	{
		isAlive = false;
		currentAnimation = &deadAnim;
		
	}
	

	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(currentVelocity);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 22;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 32;

	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	if (right)
	{
		app->render->DrawTexture(texture, position.x, position.y, &rect);
	}
	else
	{
		
		app->render->DrawTexture(texture, flipPos.x, position.y, &rect,1.0f,0, INT_MAX,INT_MAX,SDL_FLIP_HORIZONTAL);
	}
	//Movimiento camara 
	app->render->camera.x = -position.x+400;
	currentAnimation->Update();

	app->render->camera.y = -position.y+300;
	currentAnimation->Update();

	return true;
}

bool Player::CleanUp()
{

	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::PLATFORM:
		isjumpping = false;
		checkColumn = false;
		currentAnimation = &idleAnim;
		LOG("Collision PLATFORM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	case ColliderType::SPIKES:
		//vidas=vidas - 1;
		currentAnimation = &deadAnim;
		break;
	case ColliderType::COLUMN:
		checkColumn = true;
		break;
	}
}