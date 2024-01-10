#ifndef __POWERUP_H__
#define __POWERUP_H__

#include "Entity.h"
#include "Point.h"
#include "EntityManager.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class PowerUp : public Entity
{
public:

	PowerUp();
	virtual ~PowerUp();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	bool isPicked = false;

private:

	bool pop = false;
	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;
};

#endif // __POWERUP_H__