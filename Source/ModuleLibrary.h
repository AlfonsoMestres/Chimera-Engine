#ifndef __MODULELIBRARY_H__
#define __MODULELIBRARY_H__

#include "Module.h"
#include "map"
#include "vector"
#include "string"

class ModuleLibrary : public Module
{
	public:
		ModuleLibrary();
		~ModuleLibrary();

		bool Init() override;
		update_status Update() override;
		bool CleanUp() override;

		void UpdateMeshesList();
		void UpdateTexturesList();
		void UpdateScenesList();

	public:
		bool					 toBeDeleted = false;
		bool			  		 removeHead = false;
		std::vector<std::string> fileMeshesList;
		std::vector<std::string> fileTexturesList;
		std::vector<std::string> fileScenesList;

};

#endif
