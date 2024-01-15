#ifndef __MODULEFADETOBLACK_H__
#define __MODULEFADETOBLACK_H__

#include "Module.h"
#include "SDL\include\SDL_rect.h"
#include "Textures.h"

class FadeToBlack : public Module
{
public:
	//Constructor
	FadeToBlack();

	//Destructor
	virtual ~FadeToBlack();

	// Called when the module is activated
	// Enables the blending mode for transparency
	bool Start();

	// Called at the middle of the application loop
	// Updates the fade logic
	bool Update(float dt);

	// Called at the end of the application loop
	// Performs the render call of a black rectangle with transparency
	bool PostUpdate();

	// Called from another module
	// Starts the fade process which has two steps, fade_out and fade_in
	// After the first step, the modules should be switched
	bool FadeToBlackFunction(Module* toDisable, Module* toEnable, bool fadeOut, bool fadeIn, float frames = 60);

private:

	enum Fade_Step
	{
		NONE,
		TO_BLACK,
		FROM_BLACK
	} currentStep = Fade_Step::NONE;

	// A frame count system to handle the fade time and ratio
	Uint32 frameCount = 0;
	Uint32 maxFadeFrames = 0;

	// The rectangle of the screen, used to render the black rectangle
	SDL_Rect screenRect;

	// The modules that should be switched after the first step
	Module* moduleToEnable = nullptr;
	Module* moduleToDisable = nullptr;

	struct fadeout
	{
		SDL_Texture* fade = nullptr;
	}fadeout[8];

	struct fadein
	{
		SDL_Texture* fade = nullptr;
	}fadein[8];

	int t = 0;
	bool fOut = false;
	bool fIn = false;
};

#endif //__MODULEFADETOBLACK_H__