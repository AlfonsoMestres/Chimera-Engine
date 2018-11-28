#ifndef __MODULESCENE_H__
#define __MODULESCENE_H__

#include <list>
#include "Module.h"
#include "GameObject.h"
#include "assimp/matrix4x4.h"

class GameObject;
enum class ComponentType;

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

		GameObject* CreateGameObject(const char* goName, GameObject* goParent, const aiMatrix4x4& transform = aiMatrix4x4());

	public:
		GameObject* root = nullptr;
		GameObject* goSelected = nullptr;
};

#endif
