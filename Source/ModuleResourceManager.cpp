#include "ModuleResourceManager.h"

ModuleResourceManager::ModuleResourceManager() { }

ModuleResourceManager::~ModuleResourceManager() { }

std::string ModuleResourceManager::NewGuuid() {
	std::string uuid(xg::newGuid().str());
	return uuid;
}
