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
#include "Map.h"
#include <map>
#include <chrono>
#include <thread>

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

	return true;
}

bool Player::Start() {

	idleAnim.LoadAnimations("idle");
	walkAnim.LoadAnimations("walk");
	atack1Anim.LoadAnimations("attack1");
	deadAnim.LoadAnimations("dead");
	jumpAnim.LoadAnimations("jump");

	InitPosX = position.x;
	InitPosY = position.y;

	//initilize textures
	texture = app->tex->Load(texturePath);
	currentAnimation = &idleAnim;

	pbody = app->physics->CreateCircle(position.x + 16, position.y-10, 25, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;
	

	return true;
}

bool Player::Update(float dt)
{
	flipPos.x = position.x - 10;
	b2Vec2 currentVelocity = pbody->body->GetLinearVelocity();

	//currentAnimation = &idleAnim;
	
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		godMode = !godMode;		
	}

	if (godMode)
	{
		pbody->body->GetFixtureList()->SetSensor(true);
		pbody->body->SetGravityScale(0);
	}
	else
	{
		pbody->body->GetFixtureList()->SetSensor(false);
		pbody->body->SetGravityScale(1);
		currentVelocity.y += 0.5;
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && godMode == true) {
		currentVelocity.y = currentVelocity.y - 0.5;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && godMode == true) {
		currentVelocity.y = currentVelocity.y + 0.5;
	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE && isAlive && godMode && !isAttacking)
	{
		currentVelocity.y = 0;
		currentAnimation = &idleAnim;
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE && isAlive && !isAttacking)
	{
		currentVelocity.x = 0;
		currentAnimation = &idleAnim;

		
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && godMode == false && !powerUp && !isAttacking && isAlive) {
		Attack();
	}

	if (currentAnimation==&atack1Anim && currentAnimation->HasFinished())
	{
		if (damage)
		{
			isAttacking = false;
			damage->body->SetActive(false);
			damage->body->GetWorld()->DestroyBody(damage->body);
			damage = NULL;
		}
		isAttacking = false;
		currentAnimation->loopCount = 0;
		currentAnimation->Reset();
	}
	
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && godMode == false) {
		
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && isAlive && !isAttacking) {
		right = false;
		isWalking = true;
		currentVelocity.x = -speed * 16;
		currentAnimation = &walkAnim;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && isAlive && !isAttacking) {
		right = true;
		isWalking = true;
		currentVelocity.x = +speed * 16;
	
		currentAnimation = &walkAnim; 
		
	}
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !isjumpping && isAlive && !checkColumn && !isAttacking) {
		isjumpping = true;
		currentAnimation = &jumpAnim;
		currentVelocity.y = -17;
		pbody->body->SetLinearVelocity(currentVelocity);
		
	}
	//que hace si est� tocando con el pincho
	if (spike == true)
	{
		currentVelocity.x = 0;
		isAlive = false;
		isAttacking = false;
		if (currentAnimation!=&deadAnim)
		{
			currentAnimation = &deadAnim;
			currentAnimation->loopCount = 0;
			currentAnimation->Reset();
		}
		if (currentAnimation->HasFinished())
		{
			//respawn
			if (damage)
			{
				damage->body->SetActive(false);
				damage->body->GetWorld()->DestroyBody(damage->body);
				damage = NULL;
			}
			spike = false;
			isAlive = true;
			position.x = 700;
			position.y = 1350;
			pbody->SetPosition(position.x, position.y);
			currentAnimation = &idleAnim;
			app->render->camera.x = 0;
		}
	}

	if (isjumpping)
	{
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE && isAlive &&!isAttacking)
		{
			currentVelocity.x = 0;
			currentAnimation = &jumpAnim;
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && isAlive && !isAttacking)
		{
			isWalking = true;
			right = false;
			currentVelocity.x = -speed * 16;
			currentAnimation = &jumpAnim;
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && isAlive && !isAttacking)
		{
			isWalking = true;
			right = true;
			currentVelocity.x = +speed * 16;
			currentAnimation = &jumpAnim;
		}
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && isAlive && !isAttacking && !isWalking)
		{
			currentVelocity.x = 0;
			Attack();
		}		
	}
	//Set the velocity of the pbody of the player

	if (isjumpping == false && !godMode)
	{
		currentVelocity.y = -GRAVITY_Y;
	}
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		position.x = 700;
		position.y = 1350;
		pbody->SetPosition(position.x, position.y);
		app->render->camera.x = 0;
		
	}

	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(currentVelocity);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 22;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 42;

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


	if (app->render->camera.x - position.x + 400 <= -24 && app->render->camera.x - position.x + 400 >= -7532) {
		app->render->camera.x = -position.x + 400;
	
	}
		currentAnimation->Update();

	return true;
}

void Player::Attack()
{
	isAttacking = true;
	currentAnimation = &atack1Anim;
	currentAnimation->loopCount = 0;
	currentAnimation->Reset();
	if (right)
	{

		damage = app->physics->CreateRectangleSensor(position.x + 70, position.y + 30, 20, 40, bodyType::KINEMATIC);
		damage->listener = this;
		damage->ctype = ColliderType::DAMAGE;
	}
	else if (!right)
	{

		damage = app->physics->CreateRectangleSensor(position.x - 15, position.y + 30, 20, 40, bodyType::KINEMATIC);
		damage->listener = this;
		damage->ctype = ColliderType::DAMAGE;
	}
	if (isAttacking)
	{
		currentAnimation->Reset();
		currentAnimation = &atack1Anim;
		currentAnimation->loopCount = 0;
	}
}

bool Player::CleanUp()
{
	app->tex->UnLoad(texture);
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
		
		LOG("Collision PLATFORM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	case ColliderType::SPIKES:
		if (!godMode)
		{
			spike = true;
		}
		
		break;
	case ColliderType::COLUMN:
		checkColumn = true;
		break;
	}
}