#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <list>
#include "Globals.h"
#include "Timer.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleEditor;
class ModuleCamera;
class ModuleInput;
class ModuleScene;
class ModuleProgram;
class ModuleTime;
class ModuleModel;
class ModuleScene;

class Application
{
	public:

		Application();
		~Application();

		bool Init();
		void PreUpdate();
		void FinishUpdate();
		update_status Update();
		bool CleanUp();

	public:
		ModuleRender* renderer = nullptr;
		ModuleWindow* window = nullptr;
		ModuleInput* input = nullptr;
		ModuleTextures* textures = nullptr;
		ModuleEditor* editor = nullptr;
		ModuleCamera* camera = nullptr;
		ModuleProgram* program = nullptr;
		ModuleTime* time = nullptr;
		ModuleModel* model = nullptr;
		ModuleScene* scene = nullptr;

		Timer	gameTime;
		bool	gamePaused = false;
		float	deltaTime = 0.0f;
		float	gameDeltaTime = 0.0f;
		bool	gameModeEnabled = false;
		int		framerateCap = 60;
		int		gameframerateCap = 60;
		bool	counting = false;

	private:
		std::list<Module*>	modules;
		Timer				msTimer;

};

extern Application* App;

#endif