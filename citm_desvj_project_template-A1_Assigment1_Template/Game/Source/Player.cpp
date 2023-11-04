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

	InitPosX = position.x;
	InitPosY = position.y;

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

	
	
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		godMode = !godMode;
	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && godMode == true) {
		currentVelocity.y = currentVelocity.y - 0.5;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && godMode == true) {
		currentVelocity.y = currentVelocity.y + 0.5;

	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE && isAlive)
	{
		currentVelocity.x = 0;
		currentAnimation = &idleAnim;
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && godMode==false) {
		currentAnimation = &atack1Anim;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && godMode == false) {
		
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && isAlive ) {
		right = false;
		isWalking = true;
		//vel = b2Vec2(-speed*dt, -GRAVITY_Y);
		currentVelocity.x = -speed * 16;
		currentAnimation = &walkAnim;
		//currentAnimation = &animations["walk"];
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && isAlive) {
		right = true;
		isWalking = true;
		currentVelocity.x = +speed * 16;
		//vel = b2Vec2(speed*dt, -GRAVITY_Y);
		currentAnimation = &walkAnim; 
		
	}
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !isjumpping && isAlive && !checkColumn) {
		isjumpping = true;
		currentAnimation = &jumpAnim;
		currentVelocity.y = -speed * 16;
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
			currentVelocity.x = -speed * 16;
			currentAnimation = &jumpAnim;
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && isAlive)
		{
			right = true;
			currentVelocity.x = +speed * 16;
			currentAnimation = &jumpAnim;
		}

		
	}
	

	//Set the velocity of the pbody of the player

	if (isjumpping == false && !godMode)
	{
		currentVelocity.y = -GRAVITY_Y;
	}

	if (vidas==0)
	{
		isAlive = false;
		currentAnimation = &deadAnim;
		
	}
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		position.x = 700;
		position.y = 1350;
		pbody->SetPosition(position.x, position.y);
		
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
	app->render->camera.x = -position.x+400;
	currentAnimation->Update();

	/*app->render->camera.y = -position.y+300;
	currentAnimation->Update();*/

	return true;
}

bool Player::CleanUp()
{
	//app->tex->UnLoad(texture);

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
		if (godMode == true)
		{

		}else
		vidas=vidas - 1;
		isjumpping = false;
		currentAnimation = &deadAnim;

		position.x = 700;
		position.y = 1350;
		pbody->SetPosition(position.x, position.y);

		break;
	case ColliderType::COLUMN:
		checkColumn = true;
		break;
	}
}