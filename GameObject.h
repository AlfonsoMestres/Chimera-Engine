#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "assert.h"
#include "Globals.h"
#include "Component.h"

#include <vector>

class Component;
enum class componentType;

class GameObject
{
	public:
		GameObject(const char* goName);
		~GameObject();

		void Update();
		Component* CreateComponent(componentType type);

	public:
		const char* name = nullptr;
		GameObject* parent = nullptr;
		std::vector<Component*> components;
		std::vector<GameObject*> children;
};

#endif
