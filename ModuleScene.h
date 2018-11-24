#ifndef __MODULESCENE_H__
#define __MODULESCENE_H__

#include <list>
#include "Module.h"
#include "GameObject.h"

class GameObject;

class ModuleScene : public Module
{
	public:
		ModuleScene();
		~ModuleScene();

		bool Init() override;
		update_status Update() override;
		bool CleanUp() override;
		void Draw();
		void DrawHierarchy();

		GameObject* CreateGameObject(const char* goName, GameObject* goParent);

	public:
		GameObject* root = nullptr;
		GameObject* goSelected = nullptr;
};

#endif
