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
#include "FadeToBlack.h"
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
	powerUpTexture = parameters.attribute("powerUpTexture").as_string();
	
	knightAttack= parameters.attribute("audioAttack").as_string();
	knightDie= parameters.attribute("audioDeath").as_string();
	knightWalk= parameters.attribute("audioWalk").as_string();
	knightJump = parameters.attribute("audioJump").as_string();
	knightWin = parameters.attribute("audioWin").as_string();

	return true;
}

bool Player::Start() {

	idleAnim.LoadAnimations("idle");
	walkAnim.LoadAnimations("walk");
	atack1Anim.LoadAnimations("attack1");
	deadAnim.LoadAnimations("dead");
	jumpAnim.LoadAnimations("jump");
	attackPower.LoadAnimations("ability");
	

	InitPosX = position.x;
	InitPosY = position.y;

	//initilize textures
	texture = app->tex->Load(texturePath);
	powerUpText = app->tex->Load(powerUpTexture);

	audioAttack = app->audio->LoadFx(knightAttack);
	audioDie = app->audio->LoadFx(knightDie);
	audioWalk = app->audio->LoadFx(knightWalk);
	audioJump = app->audio->LoadFx(knightJump);
	audioWin = app->audio->LoadFx(knightWin);


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

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && powerUp)
	{
		currentAnimation = &atack1Anim;
	}
	//currentAnimation = &idleAnim;
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		godMode = !godMode;		
	}
	if (position.x>=4400 && position.x<=4450)
	{
		app->fade->FadeToBlackFunction(2, 60.0f);
		bool audio=true;
		if (audio)
		{
			app->audio->PlayFx(audioWin);
			audio = false;
		}
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

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && isAlive && !isAttacking && !isjumpping) {
		right = false;
		isWalking = true;
		currentVelocity.x = -speed * 16;
		currentAnimation = &walkAnim;
		app->audio->PlayFx(audioWalk);
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && isAlive && !isAttacking && !isjumpping) {
		right = true;
		isWalking = true;
		currentVelocity.x = +speed * 16;
		currentAnimation = &walkAnim; 
		app->audio->PlayFx(audioWalk);
		
	}
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !isjumpping && isAlive && !checkColumn && !isAttacking) {
		isjumpping = true;
		currentAnimation = &jumpAnim;
		app->audio->PlayFx(audioJump);
		currentVelocity.y = -17;
		pbody->body->SetLinearVelocity(currentVelocity);
		
	}

	if (position.x >=2878 && position.x<=2953)
	{
		if (!check1)
		{
			app->SaveRequest();
			check1 = true;
			check2 = false;
			check3 = false;
			check4 = false;
			check5 = false;
		}
	}
	if (position.x >=5734 && position.x <=5844 && position.y>=1428 && position.y<=1637)
	{
		if (!check2)
		{
			app->SaveRequest();
			check1 = false;
			check2 = true;
			check3 = false;
			check4 = false;
			check5 = false;
		}
	}
	if (position.x >= 5849 && position.x <= 5940 && position.y >= 2626 && position.y <= 2821)
	{
		if (!check3)
		{
			app->SaveRequest();
			check1 = false;
			check2 = false;
			check3 = true;
			check4 = false;
			check5 = false;
		}
	}
	if (position.x >= 5215 && position.x <= 5284 && position.y >= 3809 && position.y <= 4005)
	{
		if (!check4)
		{
			app->SaveRequest();
			check1 = false;
			check2 = false;
			check3 = false;
			check4 = true;
			check5 = false;
		}
	}
	if (position.x >= 6280 && position.x <= 6393 && position.y >= 5365 && position.y <= 5564)
	{
		if (!check5) 
		{
			app->SaveRequest();
			check1 = false;
			check2 = false;
			check3 = false;
			check4 = false;
			check5 = true;
		}
	}
	//si esta tocando con el pincho
	if (spike == true || health==0)
	{
		//app->map->Disable();
		currentVelocity.x = 0;
		isAlive = false;
		isAttacking = false;
		if (currentAnimation!=&deadAnim)
		{
			app->audio->PlayFx(audioDie);
			currentAnimation = &deadAnim;
			currentAnimation->loopCount = 0;
			currentAnimation->Reset();
		}
		if (currentAnimation->HasFinished())
		{
			if (!isAlive)
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
				if (!scene2Active && !check1)
				{
					position.x = 700;
					position.y = 1350;
					app->render->camera.x = 0;
				}
				else if (!scene2Active && check1)
				{
					position.x = 3478;
					position.y = 1350;
					app->render->camera.x = 0;
				}
				else if (scene2Active && check1)
				{
					position.x = 5850;
					position.y = 1350;
					app->render->camera.x = -5124;
				}
				else if (scene2Active && check2)
				{
					position.x = 6374;
					position.y = 2280;
				}
				else if (scene2Active && check3)
				{
					position.x = 6471;
					position.y = 3463;
					app->render->camera.x = -5124;
				}
				else if (scene2Active && check4)
				{
					position.x = 5837;
					position.y = 4647;
					app->render->camera.x = -5124;
				}
				else if (scene2Active && check5)
				{
					position.x = 6902;
					position.y = 6206;
					app->render->camera.x = -5124;
				}
				
				
				pbody->SetPosition(position.x, position.y);
				currentAnimation = &idleAnim;
				health = 3;
				
			}
		}
		//app->fade->FadeToBlackScene((Module*)app->scene, (Module*)app->scenedeath, 60);
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
		if (!powerUp)
		{
			app->render->DrawTexture(texture, position.x, position.y, &rect);
		}
		else
		{
			app->render->DrawTexture(powerUpText, position.x, position.y, &rect);
		}
		
	}
	else
	{
		if (!powerUp)
		{
			app->render->DrawTexture(texture, flipPos.x, position.y, &rect, 1.0f, 0, INT_MAX, INT_MAX, SDL_FLIP_HORIZONTAL);
		}
		else
		{
			app->render->DrawTexture(powerUpText, flipPos.x, position.y, &rect, 1.0f, 0, INT_MAX, INT_MAX, SDL_FLIP_HORIZONTAL);
		}
		
	}
	//Movimiento camara 
	//LOG("camera y: %d", app->render->camera.y);
	/*LOG("player x: %d", position.x);
	LOG("player y: %d", position.y);*/
	//LOG("camera y - player y: %d", app->render->camera.y - position.y + 400);*/

	if (app->render->camera.x - position.x + 400 <= -24 && app->render->camera.x - position.x + 400 >= -7532 && !scene2Active) {
		app->render->camera.x = -position.x + 400;
	}

	else if (app->render->camera.x - position.x + 400 <= -10260 && app->render->camera.x - position.x + 400 >= -11384 && scene2Active) {
		app->render->camera.x = -position.x + 400;
		app->render->camera.y = -position.y + 400;
		
	}
	else if (app->render->camera.y - position.y <= 103 && app->render->camera.y - position.y >= -11600 && scene2Active)
	{
		app->render->camera.y = -position.y+400;
	}
	
	currentAnimation->Update();

	return true;
}

void Player::Attack()
{
	isAttacking = true;
	app->audio->PlayFx(audioAttack);
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
		score = score ++;

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
		if (!godMode && isAlive)
		{
			spike = true;
		}
		break;
	case ColliderType::COLUMN:
		if (isAlive)
		{
			checkColumn = true;
		}
		
		break;
	case ColliderType::ENEMYDAMAGE:
		if (!godMode && isAlive)
		{
			health = health - 1;
		}
		
		break;
	case ColliderType::POWERUP:

		powerUp = true;
		break;
	case ColliderType::RECOVER:
		if (health<3)
		{
			health = health + 1;
		}
		break;	
	}
}