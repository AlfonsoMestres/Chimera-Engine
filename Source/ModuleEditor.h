#ifndef __MODULEEDITOR_H__
#define __MODULEEDITOR_H__

#include "Module.h"
#include "Globals.h"
#include "Dock.h"
#include "DockAbout.h"
#include "DockConsole.h"
#include "DockConfig.h"
#include "DockScene.h"
#include "DockTime.h"
#include "DockAssets.h"
#include "DockHierarchy.h"
#include "DockInspector.h"
#include "DockCamera.h"
#include "DockLight.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <vector>

class Dock;
class DockConsole;
class DockScene;
class DockConfig;
class DockTime;
class DockHierarchy;
class DockInspector;
class DockCamera;
class DockLight;
class DockAssets;

class ModuleEditor : public Module
{
	public:
		ModuleEditor();
		~ModuleEditor();

		bool				Init();
		update_status		PreUpdate() override;
		update_status		Update() override;
		bool				CleanUp() override;

		// ImgUI info
		void				AddFPSCount(float fps, float ms) const;
		void				AddGameFPSCount(float fps, float ms) const;
		void				CreateDockSpace();
		void				PrintDocks();
		void				RenderGUI();
		bool				SceneFocused() const;

		// SDL events
		void				ProcessInputEvent(SDL_Event* event) const;

	public:
		//Docking
		DockAbout* about = nullptr;
		DockConsole* console = nullptr;
		DockScene* scene = nullptr;
		DockConfig* config = nullptr;
		DockTime* time = nullptr;
		DockHierarchy* hierarchy = nullptr;
		DockInspector* inspector = nullptr;
		DockCamera* camera = nullptr;
		DockLight* light = nullptr;
		DockAssets* assets = nullptr;

		const std::string FilePickerID = "###FilePicker";

	private:
		std::list<Dock*> docks;

};

#endif