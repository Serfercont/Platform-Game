
#ifndef __SCENEINTRO_H__
#define __SCENEINTRO_H__

#include "Module.h"


	struct SDL_Texture;

	class SceneIntro : public Module
	{
	public:

		SceneIntro();

		// Destructor
		virtual ~SceneIntro();

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
		
		
		pugi::xml_node configNode;
		
	};

#endif // __SCENEINTRO_H__


