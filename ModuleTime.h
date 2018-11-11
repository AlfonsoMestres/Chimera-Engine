#ifndef __MODULETIME_H__
#define __MODULETIME_H__

#include "Module.h"

class ModuleTime : public Module 
{
	public:
		ModuleTime();
		~ModuleTime();

		update_status PreUpdate() override;

	public:
		float deltaTime = 0.0f;	
		float lastFrame = 0.0f;
};

#endif //__MODULETIME_H__