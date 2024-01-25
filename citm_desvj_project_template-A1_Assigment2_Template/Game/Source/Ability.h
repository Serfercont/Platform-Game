#ifndef __ABILITY_H__
#define __ABILITY_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "EntityManager.h"
#include "Timer.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Ability : public Entity
{
public:

	Ability();
	virtual ~Ability();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	bool isPicked = false;
	Animation* currentAnimation = nullptr;
	Animation attack;
	bool attacking = false;
	float speed = 0.6f;
	bool derecha = false;
	Timer timer;
	bool timerPaused = false;

private:

	bool pop = false;
	SDL_Texture* texture;
	const char* texturePath;

	iPoint flipPos;

	PhysBody* damageAb;
	
};

#endif // __ABILITY_H__