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
	float enemyPositionX;
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
	Animation hitAnim;
	Animation attackAnim;
	Animation deadAnim1;
	Animation deadAnim2;

	uint eyeDeath;
	bool attack;
	bool fall=false;
	bool die = false;
	bool isAlive=true;
	bool attackBody = false;
private:
	PhysBody* damage;
};

#endif // __ENEMYWALK_H__