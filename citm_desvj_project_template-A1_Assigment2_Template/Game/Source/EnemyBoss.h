#ifndef __ENEMYBOSS_H__
#define __ENEMYBOSS_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "DynArray.h"

struct SDL_Texture;

class EnemyBoss : public Entity
{
public:
	EnemyBoss();

	virtual ~EnemyBoss();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:
	float speed = 0.2f;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;
	PhysBody* damage;
	SDL_Texture* tileTex;
	b2Transform initialTransform;

	//flipping pos
	iPoint flipPos;
	bool right;

	//pathfinding
	iPoint origin;
	iPoint destiny;
	//DynArray<iPoint> lastPath;
	b2Vec2 velocity;

	Animation* currentAnimation = nullptr;
	Animation walkIdleAnim;
	Animation attack1Anim;
	Animation attack2Anim;
	Animation invocaAnim;
	Animation deadAnim;

	bool attack;
	bool die = false;
	//bool isAlive=true;
	bool isAttacking = false;
	bool attackBody = false;

	DynArray<iPoint> lastPath;
};

#endif // __ENEMYBOSS_H__
