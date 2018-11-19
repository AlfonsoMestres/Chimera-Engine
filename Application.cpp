#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleEditor.h"
#include "ModuleCamera.h"
#include "ModuleProgram.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleModel.h"
#include "ModuleScene.h"

Application::Application() {

	modules.push_back(window = new ModuleWindow());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(input = new ModuleInput());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(program = new ModuleProgram());
	modules.push_back(editor = new ModuleEditor());
	modules.push_back(model = new ModuleModel());
	modules.push_back(scene = new ModuleScene());

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

	msTimer.Start();
	gameModeEnabled = false;
	counting = false;
	gameDeltaTime = 0;
	
	/*App->model->Load("./Models/BakerHouse/BakerHouse.fbx");*/

	return ret;
}

void Application::PreUpdate() {

	deltaTime = (float)msTimer.Read() / 1000.0f;

	if (!gameModeEnabled || gamePaused) {
		gameDeltaTime = 0;
	} else {
		gameDeltaTime = deltaTime / ((float)gameframerateCap / (float)framerateCap);
	}

	msTimer.Start();

	if (gameModeEnabled && !counting) {
		counting = true;
		gameTime.Start();
	}

}

void Application::FinishUpdate() {

	int ms_cap = 1000 / framerateCap;
	if (msTimer.Read() < ms_cap)
		SDL_Delay(ms_cap - msTimer.Read());

	App->editor->AddFPSCount(1 / deltaTime, deltaTime * 1000);

	if (!gameModeEnabled || gamePaused) {
		App->editor->AddFPSCount(0, 0);
	} else {
		App->editor->AddFPSCount(1 / gameDeltaTime, gameDeltaTime * 1000);
	}

	if (!gameModeEnabled && counting) {
		gameTime.Stop();
		counting = false;
	}

}


update_status Application::Update() {

	PreUpdate();
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

bool Application::CleanUp() {

	Timer cleanUpTimer;
	bool ret = true;

	for (std::list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it) {
		ret = (*it)->CleanUp();
	}

	LOG("Cleaned modules in %d ms", cleanUpTimer.Stop());
	return ret;
}
