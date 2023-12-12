#ifndef __ENEMYWALK_H__
#define __ENEMYWALK_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

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

public:
	float speed = 0.4f;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;


	Animation* currentAnimation = nullptr;
	Animation idleAnim;
};

#endif // __ENEMYWALK_H__