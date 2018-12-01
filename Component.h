#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "GameObject.h"

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
		virtual ~Component();

		bool			DrawComponentState();
		virtual void	DrawProperties() { };

		virtual void	Enable() { enabled = true; };
		virtual void	Update();
		virtual void	Disable() { enabled = false; };
		void			Remove();

		bool			enabled = true;
		ComponentType	componentType = ComponentType::EMPTY;
		GameObject*		goContainer = nullptr;

};

#endif
