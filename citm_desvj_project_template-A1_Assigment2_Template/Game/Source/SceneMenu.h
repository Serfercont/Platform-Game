
#ifndef __SCENEMENU_H__
#define __SCENEMENU_H__

#include "Module.h"


struct SDL_Texture;

class SceneMenu : public Module
{
public:

	SceneMenu(App* app, bool start_enabled = true);

	// Destructor
	virtual ~SceneMenu();

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

	SDL_Texture* MainMenu;

};

#endif // __SCENEINTRO_H__


