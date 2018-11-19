#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "GameObject.h"

class GameObject;

enum class componentType {
	CAMERA,
	TRANSFORM,
	MESH,
	MATERIAL
};

class Component
{
	public:
		Component();
		~Component();

		virtual void Enable();
		virtual void Update();
		virtual void Disable();

		componentType type;
		bool active = false;
		GameObject* fatherGO = nullptr;

};

#endif
