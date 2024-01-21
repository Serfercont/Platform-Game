#ifndef __ABILITY_H__
#define __ABILITY_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "EntityManager.h"
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
	void Move();

public:

	bool isPicked = false;
	Animation* currentAnimation = nullptr;
	Animation attack;
	bool attacking = false;
	float speed = 0.4f;
	bool derecha = false;

private:

	bool pop = false;
	SDL_Texture* texture;
	const char* texturePath;

	iPoint flipPos;

	PhysBody* damageAb;
	
};

#endif // __ABILITY_H__