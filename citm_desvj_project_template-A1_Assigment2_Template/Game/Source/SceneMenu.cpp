#include "SceneMenu.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Map.h"	
#include "EntityManager.h"	
#include "Render.h"
#include "Window.h"
#include "Physics.h"
#include "FadeToBlack.h"
#include "Scene.h"
#include "GuiManager.h"

#include "Defs.h"
#include "Log.h"

SceneMenu::SceneMenu(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("sceneMenu");
}

// Destructor
SceneMenu::~SceneMenu()
{}

// Called before render is available
bool SceneMenu::Awake(pugi::xml_node& config)
{
	configNode = config;
	LOG("Loading SceneMenu");
	bool ret = true;


	return ret;
}

// Called before the first frame
bool SceneMenu::Start()
{
	LOG("Loading SceneMenu Assets");
	bool ret = true;
	MainMenu = app->tex->Load("Assets/UI/MainMenu.png");
	credits=app->tex->Load("Assets/UI/credits.png");
	playOff = app->tex->Load("Assets/UI/PlayOFF.png");
	playOn = app->tex->Load("Assets/UI/PlayON.png");
	contiuneOff = app->tex->Load("Assets/UI/ContinueOFF.png");
	continueOn = app->tex->Load("Assets/UI/ContinueON.png");
	settingsOff = app->tex->Load("Assets/UI/SettingsOFF.png");
	settingsOn = app->tex->Load("Assets/UI/SettingsON.png");
	creditsOff = app->tex->Load("Assets/UI/CreditsOFF.png");
	creditsOn = app->tex->Load("Assets/UI/CreditsON.png");
	exitOff = app->tex->Load("Assets/UI/ExitOFF.png");
	exitOn = app->tex->Load("Assets/UI/ExitON.png");
	goBackON= app->tex->Load("Assets/UI/backOn.png");
	goBackOFF = app->tex->Load("Assets/UI/backOff.png");
	

	playButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, NULL, playOff, playOn, NULL, { 610, 310, 288, 73 }, this);
	continueButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, NULL, contiuneOff, continueOn, NULL, { 610, 392,288, 73 }, this);
	settingsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, NULL, settingsOff, settingsOn, NULL, { 610, 474, 288, 73 }, this);
	creditsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, NULL, creditsOff, creditsOn, NULL, { 610, 556, 288, 73 }, this);
	exitButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, NULL, exitOff, exitOn, NULL, { 610, 638, 288, 73 }, this);
	goBackButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, NULL, goBackOFF, goBackON, NULL, { 610, 638, 288, 73 }, this);

	playButton->state = GuiControlState::NORMAL;
	continueButton->state = GuiControlState::NORMAL;
	settingsButton->state = GuiControlState::NORMAL;
	creditsButton->state = GuiControlState::NORMAL;
	exitButton->state = GuiControlState::NORMAL;
	goBackButton->state = GuiControlState::HIDDEN;
	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture


	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	return true;
}

// Called each loop iteration
bool SceneMenu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneMenu::Update(float dt)
{
	//Draw
	SDL_Rect RectfondoInicial{ 0,0,windowW,windowH };
	if (onMenu)
	{
		app->render->DrawTexture(MainMenu, 0, 0, &RectfondoInicial, SDL_FLIP_NONE, 0);
	}
	
	if (playButton->state == GuiControlState::PRESSED)
	{
		app->fade->FadeToBlackScene(this, (Module*)app->scene, 60);
		app->guiManager->Disable();
		app->map->Enable();
		app->entityManager->Enable();
	}
	else if (exitButton->state==GuiControlState::PRESSED)
	{
		SDL_Quit();
	}
	else if (continueButton->state==GuiControlState::PRESSED)
	{
		/*app->fade->FadeToBlackScene(this, (Module*)app->scene, 60);
		app->scene->player->position.x = app->scene->player->InitPosX;
		app->scene->player->position.y= app->scene->player->InitPosY;
		app->LoadRequest();*/
	}
	else if (creditsButton->state == GuiControlState::PRESSED)
	{

		onCredits = true;
		onMenu = false;



	}
	if (onCredits)
	{
		playButton->state = GuiControlState::HIDDEN;
		continueButton->state = GuiControlState::HIDDEN;
		settingsButton->state = GuiControlState::HIDDEN;
		creditsButton->state = GuiControlState::HIDDEN;
		exitButton->state = GuiControlState::HIDDEN;
		goBackButton->state = GuiControlState::NORMAL;
		onMenu = false;
		app->render->DrawTexture(credits, 0, 0, NULL, SDL_FLIP_NONE, 0);

		if (goBackButton->state == GuiControlState::PRESSED)
		{
			onMenu = true;
			onCredits = false;
		}
	}
	
	
	

	return true;
}

// Called each loop iteration
bool SceneMenu::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}


// Called before quitting
bool SceneMenu::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

