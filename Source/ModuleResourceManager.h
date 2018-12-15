#ifndef __MODULERESOURCEMANAGER_H__
#define __MODULERESOURCEMANAGER_H__

#include "Module.h"
#include "Crossguid/crossguid/guid.hpp"

class ModuleResourceManager : public Module
{
	public:
		ModuleResourceManager();
		~ModuleResourceManager();
		
		std::string NewGuuid();

};

#endif