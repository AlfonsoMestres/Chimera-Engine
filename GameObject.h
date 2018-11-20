#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <list>
#include "assert.h"
#include "Globals.h"
#include "Component.h"
#include "ComponentTransform.h"

class Component;
class ComponentTransform;
enum class ComponentType;

class GameObject
{
	public:
		GameObject();
		GameObject(const char* goName);
		GameObject(const char* goName, GameObject* goParent);
		~GameObject();

		void Update();
		void Draw();
		Component* AddComponent(ComponentType type);
		void RemoveComponent(Component* component);

	public:
		const char* name = nullptr;
		GameObject* parent = nullptr;
		std::list<Component*> components;
		std::list<GameObject*> goChilds;

		ComponentTransform* transform = nullptr;
};

#endif
