#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "document.h"
#include "prettywriter.h"

class Config;
class GameObject;

enum class ComponentType {
	EMPTY,
	CAMERA,
	TRANSFORM,
	MESH,
	MATERIAL,
	LIGHT
};

class Component
{
	public:
		Component(GameObject* goContainer, ComponentType type);
		Component(const Component& duplicateComponent);
		virtual ~Component();

		bool				DrawComponentState();
		virtual void		DrawProperties(bool enabled) { };
		virtual Component*	Duplicate() { return nullptr; };

		virtual void		Enable() { enabled = true; };
		virtual void		Update();
		virtual void		Disable() { enabled = false; };
		void				Remove();

		virtual void Save(Config* config) { };
		virtual void Load(Config* config, rapidjson::Value& value) { };

	public:
		bool				enabled = true;
		ComponentType		componentType = ComponentType::EMPTY;
		const char*			uuid = "";
		const char*			parentUuid = "";
		GameObject*			goContainer = nullptr;

};

#endif
