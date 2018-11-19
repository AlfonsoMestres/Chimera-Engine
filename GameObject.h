#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"
#include "Component.h"

#include <vector>

class Component;

class GameObject
{
	public:
		GameObject();
		~GameObject();

		const char* name = nullptr;
		GameObject* parent = nullptr;
		std::vector<Component*> components;
		std::vector<GameObject*> children;
};

#endif
