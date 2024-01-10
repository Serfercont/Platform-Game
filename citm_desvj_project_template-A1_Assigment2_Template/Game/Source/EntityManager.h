#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "List.h"

class EntityManager : public Module
{
public:

	EntityManager();

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	void GetWolf(List<Entity*>& skeletonsList) const;

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type);

	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);

	void GetWolves(List<Entity*>& WolvesList) const;
	void GetEyes(List<Entity*>& EyeList) const;
	void GetItems(List<Entity*>& ItemsList) const;
	void GetRecovers(List<Entity*>& ItemsList) const;

public:

	List<Entity*> entities;

};

#endif // __ENTITYMANAGER_H__
