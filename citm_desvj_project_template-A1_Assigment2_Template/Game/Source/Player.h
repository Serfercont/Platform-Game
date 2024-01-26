#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void Attack();
	void AttackPowerUp();

public:
	float speed = 0.4f;
	int health = 3;
	int score = 0;
	int deathCounter = 0;
	SDL_Texture* texture = NULL;
	SDL_Texture* powerUpText = NULL;
	PhysBody* pbody;
	PhysBody* damage;

	bool isjumpping = false;
	bool isWalking = false;
	int pickCoinFxId;
	bool right;
	bool left;
	bool isAlive=true;
	bool checkColumn;
	bool godMode=false;
	int InitPosX;
	int InitPosY;
	bool spike=false;
	bool powerUp= false;
	bool isAttacking = false;
	bool attackCreated;
	bool scene2Active = false;

	
	bool check1 = false;
	bool check2 = false;
	bool check3 = false;
	bool check4 = false;
	bool check5 = false;

	bool check1Played = false;
	bool check2Played = false;
	bool check3Played = false;
	bool check4Played = false;
	bool check5Played = false;

	iPoint flipPos;

	uint audioWalk;
	uint audioAttack;
	uint audioDie;
	uint audioJump;
	uint audioWin;
	uint audioCheck;

	//std::map<std::string, Animation> animations;

	Animation* currentAnimation = nullptr;
	Animation idleAnim;
	Animation walkAnim;
	Animation atack1Anim;
	Animation atack2Anim;
	Animation atack3Anim;
	Animation attackPower;
	Animation jumpAnim;
	Animation deadAnim;
	Animation defendAnim;
	Animation hurtAnim;
	Animation runAnim;
	SDL_Texture* mouseTileTex = nullptr;

	pugi::xml_node configNode;
};

#endif // __PLAYER_H__