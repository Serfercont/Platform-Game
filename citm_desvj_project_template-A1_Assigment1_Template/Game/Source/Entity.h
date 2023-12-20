#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"

class PhysBody;

enum class EntityType
{
	PLAYER,
	ITEM,
	ENEMYWALK,
	ENEMYFLY,
	UNKNOWN
};

class Entity
{
public:

	Entity(EntityType type) : type(type), active(true) {}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&)
	{
		return true;
	}

	void Entity::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Entity::Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	virtual void OnCollision(PhysBody* physA, PhysBody* physB) {

	};

public:

	SString name;
	EntityType type;
	bool active = true;
	pugi::xml_node parameters; 
	const char* texturePath;
	const char* knightAttack;
	const char* knightWalk;
	const char* knightDie;
	const char* knightJump;
	const char* wolfAttack;
	const char* wolfDeath;
	const char* EyeDeath;

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	iPoint position;       
	bool renderable = true;
	bool isAlive = true;
};

#endif // __ENTITY_H__