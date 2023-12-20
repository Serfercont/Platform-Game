#ifndef __ENEMYWALK_H__
#define __ENEMYWALK_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "DynArray.h"

struct SDL_Texture;

class EnemyWalk : public Entity
{
public:
	EnemyWalk();

	virtual ~EnemyWalk();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);
	void Attack();

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
	Animation idleAnim;
	Animation walkAnim;
	Animation runAnim;
	Animation attackAnim;
	Animation deadAnim;

	uint wolfAttacks;
	uint wolfDeaths;

	bool attack;
	bool die=false;
	//bool isAlive=true;
	bool isAttacking=false;
	bool attackBody=false;

	DynArray<iPoint> lastPath;
};

#endif // __ENEMYWALK_H__