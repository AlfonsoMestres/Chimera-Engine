#ifndef __COMPONENTTRANSFORM_H__
#define __COMPONENTTRANSFORM_H__

#include "Component.h"

class ComponentTransform : public Component
{
	public:
		ComponentTransform(GameObject* goContainer);
		~ComponentTransform();
};

#endif
