#ifndef __ENEMYFLY_H__
#define __ENEMYFLY_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "DynArray.h"

struct SDL_Texture;

class EnemyFly : public Entity
{
public:
	EnemyFly();

	virtual ~EnemyFly();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:
	float speed = 0.2f;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;
	SDL_Texture* tileTex;
	b2Transform initialTransform;

	//flipping pos
	iPoint flipPos;
	bool right;

	//pathfinding
	iPoint origin;
	iPoint destiny;
	DynArray<iPoint> lastPath;
	b2Vec2 velocity;

	Animation* currentAnimation = nullptr;
	Animation idleAnim;

	bool attack;
	bool isAlive;
};

#endif // __ENEMYWALK_H__