
#ifndef __SCENEDEATH_H__
#define __SCENEDEATH_H__

#include "Module.h"


struct SDL_Texture;

class SceneDeath : public Module
{
public:

	SceneDeath(App* app, bool start_enabled = true);

	// Destructor
	virtual ~SceneDeath();

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




private:

	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	float timer = 0.0f;
	bool goTimer = false;



	pugi::xml_node configNode;

	SDL_Texture* GameOver;

};

#endif // __SCENEINTRO_H__


