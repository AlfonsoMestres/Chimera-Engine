#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleEditor.h"
#include "ModuleCamera.h"
#include "ModuleProgram.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleModelLoader.h"


Application::Application()
{
	// Order matters: they will Init/start/update in this order
	modules.push_back(input = new ModuleInput());
	modules.push_back(window = new ModuleWindow());

	modules.push_back(camera = new ModuleCamera());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(program = new ModuleProgram());
	modules.push_back(textures = new ModuleTextures());

	modules.push_back(editor = new ModuleEditor());

	modules.push_back(modelLoader = new ModuleModelLoader());
}

Application::~Application()
{
	for(std::list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		RELEASE(*it);
}

bool Application::Init()
{
	bool ret = true;

	lastTickTime = 0;
	deltaTime = 0;

	for(std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	Tick();	

	for(std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	for(std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update();

	for(std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(std::list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}

void Application::Tick()
{
	++frameCounter;
	unsigned ticksNow = SDL_GetTicks();
	deltaTime = (float)(ticksNow - lastTickTime) * 0.001;
	lastTickTime = ticksNow;
	auxTimer += deltaTime;
	if (auxTimer >= 1.0f) {
		FPS = frameCounter;
		auxTimer = 0;
		frameCounter = 0;
	}
}