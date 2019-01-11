#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleEditor.h"
#include "ModuleCamera.h"
#include "ModuleProgram.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleDebugDraw.h"
#include "ModuleScene.h"
#include "ModuleLibrary.h"
#include "ModuleFileSystem.h"
#include "ModuleTime.h"

Application::Application() {

	modules.push_back(fileSystem = new ModuleFileSystem());
	modules.push_back(window = new ModuleWindow());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(scene = new ModuleScene());
	modules.push_back(library = new ModuleLibrary());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(input = new ModuleInput());
	modules.push_back(program = new ModuleProgram());
	modules.push_back(editor = new ModuleEditor());
	modules.push_back(debug = new ModuleDebugDraw());
	modules.push_back(time = new ModuleTime());

}

Application::~Application() {

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it) {
		delete(*it);
	}
	modules.clear();
}

bool Application::Init() {

	bool ret = true;

	for(std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it) { 
		ret = (*it)->Init();
	}

	return ret;
}

update_status Application::Update() {

	update_status ret = UPDATE_CONTINUE;

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it) {
		ret = (*it)->PreUpdate();
	}

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it) {
		ret = (*it)->Update();
	}

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it) {
		ret = (*it)->PostUpdate();
	}

	FinishUpdate();

	return ret;
}

void Application::FinishUpdate() {
	int ms_cap = 1000 / time->maxFps;
	if (time->frameTimer.Read() < ms_cap) {
		SDL_Delay(ms_cap - time->frameTimer.Read());
	}
}

bool Application::CleanUp() {
	bool ret = true;

	for (std::list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it) {
		ret = (*it)->CleanUp();
	}

	return ret;
}
