#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <list>
#include <vector>
#include "imgui.h"
#include "assert.h"
#include "Globals.h"
#include "assimp/matrix4x4.h"
#include "Math/float4x4.h"
#include "Geometry/AABB.h"

class Component;
class ComponentTransform;
enum class ComponentType;

class GameObject
{
	public:
		GameObject();
		GameObject(const std::string goName, const aiMatrix4x4& transform, const char* fileLocation);
		GameObject(const std::string goName, const aiMatrix4x4& transform, GameObject* goParent, const char* fileLocation);
		~GameObject();

		void					Update();
		void					Draw();
		void					DrawHierarchy(GameObject* goSelected);
		std::string				GetFileFolder() const;

		Component*				AddComponent(ComponentType type);
		void					RemoveComponent(Component* component);
		Component*				GetComponent(ComponentType type) const;
		std::vector<Component*> GetComponents(ComponentType type) const;

		AABB&					ComputeBBox() const;

		math::float4x4			GetLocalTransform() const;
		math::float4x4			GetGlobalTransform() const;
		void					ModelTransform(unsigned shader) const;

	public:
		const char*				filePath = nullptr;
		std::string				name = "GameObject";
		GameObject*				parent = nullptr;
		std::vector<Component*>	components;
		std::list<GameObject*>	goChilds;

		ComponentTransform*		transform = nullptr;
		AABB&					bbox;
};

#endif
