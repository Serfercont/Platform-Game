#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "EnemyWalk.h"
#include "EnemyFly.h"
#include "Ability.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene(App* app, bool start_enabled = true);

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	Player* player;
	Ability* ability;
	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

	void GoLvl1();
	void GoLvl2();

private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	EnemyWalk* enemyWalk;
	
	EnemyFly* enemyFly;
	SDL_Texture* fondo0;
	SDL_Texture* fondo1;
	SDL_Texture* fondo2;
	SDL_Texture* fondo3;
	SDL_Texture* fondo4;
	SDL_Texture* mouseTileTex = nullptr;
	pugi::xml_node configNode;
	List<Entity*> WolfList;
	List<Entity*> EyeList;
	List<Entity*> ItemList;
	List<Entity*> RecoverList;
	List<Entity*> PowerUpList;

	iPoint spawn1= { 85,650 };
	iPoint spawn2 = { 5200,300 };

public:
	bool level1=false;
	bool level2=true;
};

#endif // __SCENE_H__