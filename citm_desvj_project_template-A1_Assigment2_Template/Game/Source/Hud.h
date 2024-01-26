#ifndef __HUD_H__
#define __HUD_H__

#include "Module.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "Timer.h"

struct SDL_Texture;

class Hud : public Module
{
public:

	Hud(App* app, bool start_enabled);

	virtual ~Hud();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void DrawTimer();

	bool playerDeadHud = false;
	bool spacePressed = false;
private:

	Timer timer;
	SDL_Texture* lifeBarTexture;
	SDL_Texture* deadScreenTexture;

	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;

};

#endif // __HUD_H__