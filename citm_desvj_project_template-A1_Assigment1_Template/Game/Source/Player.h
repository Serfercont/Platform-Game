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

public:
	float speed = 0.4f;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;
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

	iPoint flipPos;

	//std::map<std::string, Animation> animations;

	Animation* currentAnimation = nullptr;
	Animation idleAnim;
	Animation walkAnim;
	Animation atack1Anim;
	Animation atack2Anim;
	Animation atack3Anim;
	Animation jumpAnim;
	Animation deadAnim;
	Animation defendAnim;
	Animation hurtAnim;
	Animation runAnim;
	SDL_Texture* mouseTileTex = nullptr;
};

#endif // __PLAYER_H__