#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleEditor.h"
#include "ModuleCamera.h"
#include "ModuleProgram.h"
#include "ModuleTextures.h"
#include "ModuleTime.h"
#include "ModuleInput.h"
#include "ModuleModel.h"
#include "Timer.h"

Application::Application() {

	modules.push_back(time = new ModuleTime());
	modules.push_back(window = new ModuleWindow());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(input = new ModuleInput());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(program = new ModuleProgram());
	modules.push_back(editor = new ModuleEditor());
	modules.push_back(model = new ModuleModel());

}

Application::~Application() {

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it) {
		delete(*it);
	}

}

bool Application::Init() {

	bool ret = true;
	Timer initTimer;
	initTimer.Start();

	for(std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it) { 
		ret = (*it)->Init();
	}

	LOG("Modules initialized in %d ms", initTimer.Stop());

	App->model->Load("./Models/BakerHouse/BakerHouse.fbx");

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

	editor->AddFPSCount(1 / App->time->deltaTime);
	return ret;
}

bool Application::CleanUp() {

	Timer cleanUpTimer;
	bool ret = true;

	for (std::list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it) {
		ret = (*it)->CleanUp();
	}

	LOG("Cleaned modules in %d ms", cleanUpTimer.Stop());
	return ret;
}
