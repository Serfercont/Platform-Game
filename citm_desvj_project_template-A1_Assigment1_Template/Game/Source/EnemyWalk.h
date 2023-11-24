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

	/*bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);*/
};

#endif // __ENEMYWALK_H__