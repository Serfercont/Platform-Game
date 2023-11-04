#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 25

class Animation
{
public:
	pugi::xml_document file;
	bool loop = true;
	int loopCount = 0;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];

private:
	float currentFrame = 0.0f;
	int totalFrames = 0;
	

public:

	void PushBack(const SDL_Rect& rect)
	{
		frames[totalFrames++] = rect;
	}

	void Reset()
	{
		currentFrame = 0;
	}

	bool HasFinished()
	{
		return !loop && loopCount > 0;
	}

	void Update()
	{
		currentFrame += speed;
		if (currentFrame >= totalFrames)
		{
			currentFrame = (loop) ? 0.0f : totalFrames - 1;
			++loopCount;
		}
	}

	SDL_Rect& GetCurrentFrame()
	{
		return frames[(int)currentFrame];
	}
	void LoadAnimations(const char* name)
	{
		pugi::xml_parse_result result = file.load_file("config.xml");
		
		if (result != NULL)
		{
			pugi::xml_node animation_name = file.child("config").child("animations").child(name);
			loop = animation_name.attribute("loop").as_bool();
			speed = animation_name.attribute("speed").as_float();
			for (pugi::xml_node animation = animation_name.child("frame"); animation; animation = animation.next_sibling("frame"))
			{
				PushBack({ animation.attribute("x").as_int(), animation.attribute("y").as_int(), animation.attribute("width").as_int(), animation.attribute("height").as_int() });
			}
		}
	}

};

#endif