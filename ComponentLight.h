#ifndef __COMPONENTLIGHT_H__
#define __COMPONENTLIGHT_H__

#include "Component.h"

class ComponentLight : public Component
{
	public:
		ComponentLight(GameObject* goContainer);
		~ComponentLight();
};

#endif
