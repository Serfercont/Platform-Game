#include "FadeToBlack.h"
#include "App.h"
#include "Render.h"
#include "Scene.h"
#include "Map.h"
#include "EntityManager.h"

#include "SDL/include/SDL_render.h"

FadeToBlack::FadeToBlack(App* app, bool start_enabled) : Module(app,start_enabled)
{

	screenRect = { 0, 0, 2000, 2000 };
	name.Create("fadetoblack");
}

FadeToBlack::~FadeToBlack()
{

}

bool FadeToBlack::Start()
{
	// Enable blending mode for transparency
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

bool FadeToBlack::Update(float dt)
{
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return true;

	if (currentStep == Fade_Step::TO_BLACK)
	{
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			moduleToDisable->Disable();
			moduleToEnable->Enable();
			currentStep = Fade_Step::FROM_BLACK;

		}
		if (app->scene->active)
		{
			app->map->Enable();
			app->entityManager->Enable();
		}
	}
	else
	{
		--frameCount;
		if (frameCount <= 0)
		{
			currentStep = Fade_Step::NONE;
			fadeDone = true;
		}
		if (!active)
		{
			active = true;
			if (levelIdx==1) app->scene->GoLvl1();
			else if (levelIdx == 2) app->scene->GoLvl2();
			
		}
	}

	return true;
}

bool FadeToBlack::PostUpdate()
{
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return true;

	float fadeRatio = (float)frameCount / (float)maxFadeFrames;

	// Render the black square with alpha on the screen
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
	SDL_RenderFillRect(app->render->renderer, &screenRect);

	return true;
}

bool FadeToBlack::FadeToBlackFunction(int levelIdx, float frames)
{
	bool ret = false;

	// If we are already in a fade process, ignore this call
	if (currentStep == Fade_Step::NONE)
	{
		currentStep = Fade_Step::TO_BLACK;
		frameCount = 0;
		maxFadeFrames = frames;
		this->levelIdx = levelIdx;
		ret = true;
	}
	active = false;
	fadeDone = false;

	return ret;
}
bool FadeToBlack::FadeToBlackScene(Module* moduleToDisable, Module* moduleToEnable, float frames)
{
	bool ret = false;

	
	if (currentStep == Fade_Step::NONE) {
		currentStep = Fade_Step::TO_BLACK;
		frameCount = 0;
		maxFadeFrames = static_cast<Uint32>(frames);

		this->moduleToDisable = moduleToDisable;
		this->moduleToEnable = moduleToEnable;

		
		ret = true;
	}


	return ret;
}
