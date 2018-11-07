#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleEditor;
class ModuleCamera;
class ModuleInput;
class ModuleScene;
class ModuleProgram;
class ModuleModelLoader;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	void Tick();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleTextures* textures = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleInput* input = nullptr;
    ModuleScene* exercise = nullptr;
	ModuleProgram* program = nullptr;
	ModuleModelLoader* modelLoader = nullptr;

	float lastTickTime = 0.0f;
	float deltaTime = 0.0f;
	float auxTimer = 0.0f;
	int frameCounter = 0;
	int FPS = 0;

private:
	std::list<Module*> modules;

};

extern Application* App;

#endif