#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
struct SDL_Texture;

class Item : public Entity
{
public:

	Item();
	virtual ~Item();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();
	
	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	bool isPicked = false;

private:
	Animation* currentAnimation = nullptr;
	Animation coinAnim;
	bool pop= false;
	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;
};

#endif // __ITEM_H__