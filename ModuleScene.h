#ifndef __MODULESCENE_H__
#define __MODULESCENE_H__

#include "Module.h"
#include "GameObject.h"

class GameObject;

class ModuleScene : public Module
{
	public:
		ModuleScene();
		~ModuleScene();

		GameObject* CreateGameObject(const char* goName);

	public:
		GameObject* root = nullptr;
};

#endif
