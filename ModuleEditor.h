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

public:
	ImGuiIO io;
};

#endif