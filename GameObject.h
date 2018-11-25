#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <list>
#include "imgui.h"
#include "assert.h"
#include "Globals.h"
#include "assimp/matrix4x4.h"

class Component;
class ComponentTransform;
enum class ComponentType;

class GameObject
{
	public:
		GameObject();
		GameObject(const char* goName, const aiMatrix4x4& transform);
		GameObject(const char* goName, const aiMatrix4x4& transform, GameObject* goParent);
		~GameObject();

		void Update();
		void Draw();
		void DrawHierarchy(GameObject* goSelected);
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
