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
	SDL_Texture* GameOver;

	SDL_Texture* corazon;
	SDL_Texture* moneda0;
	SDL_Texture* moneda1;
	SDL_Texture* moneda2;
	SDL_Texture* moneda3;
	SDL_Texture* moneda4;
	SDL_Texture* moneda5;
	SDL_Texture* moneda6;
	SDL_Texture* moneda7;
	SDL_Texture* moneda8;
	SDL_Texture* moneda9;
	SDL_Texture* moneda10;

	SDL_Texture* corazon1;
	SDL_Texture* corazon2;
	SDL_Texture* corazon3;



	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;

};

#endif // __HUD_H__