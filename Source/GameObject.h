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

#include "document.h"
#include "prettywriter.h"

class Config;
class Component;
class ComponentMesh;
class ComponentMaterial;
class ComponentTransform;
enum class ComponentType;

class GameObject
{
	public:
		GameObject();
		GameObject(const char* goName, GameObject* goParent);
		GameObject(const char* goName, const math::float4x4& transform);
		GameObject(const char* goName, const math::float4x4& transform, GameObject* goParent);
		GameObject(const GameObject& duplicateGameObject);
		~GameObject();

		void					Update();
		void					Draw(const math::Frustum& frustum) const;
		void					CleanUp();
		void					DrawProperties();
		void					DrawHierarchy(GameObject* goSelected);
		void					DrawBBox() const;

		Component*				AddComponent(ComponentType type);
		void					RemoveComponent(Component* component);
		Component*				GetComponent(ComponentType type) const;
		std::vector<Component*> GetComponents(ComponentType type) const;

		void					ComputeBBox();

		math::float4x4			GetLocalTransform() const;
		math::float4x4			GetGlobalTransform() const;
		void					ModelTransform(unsigned shader) const;

		bool Save(Config* config);
		void Load(Config* config, rapidjson::Value& value);

	public:
		const char *			uuid = nullptr;
		const char *			parentUuid = nullptr;
		bool					enabled = true;
		bool					drawGOBBox = false;
		bool					duplicating = false;
		const char*				name = DEFAULT_GO_NAME;
		GameObject*				parent = nullptr;
		std::list<GameObject*>	goChilds;

		math::AABB				bbox;

		ComponentTransform*		transform = nullptr;
		ComponentMesh*			mesh = nullptr;
		ComponentMaterial*		material = nullptr;
		std::list<Component*>	components;

		bool					toBeDeleted = false;
		bool					toBeCopied = false;
		bool					moveGOUp = false;
		bool					moveGODown = false;
		bool					staticGo = false;

};

#endif
