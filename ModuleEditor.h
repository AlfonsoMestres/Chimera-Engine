#ifndef __MODULEEDITOR_H__
#define __MODULEEDITOR_H__

#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

class ModuleEditor : public Module
{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();
	void HandleInputs(SDL_Event& event);

public:
	ImGuiIO io;
	bool showAboutMenu = false;
	bool showHardwareMenu = false;
	bool requestedExit = false;
	bool showSceneConfig = false;
	bool showTextureConfig = false;

};

#endif