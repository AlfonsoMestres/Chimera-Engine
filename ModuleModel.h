#ifndef __MODULEMODEL_H__
#define __MODULEMODEL_H__

#include "CustomFile.h"
#include "Module.h"
#include "Model.h"

struct Texture;

class ModuleModel : public Module
{
	public:
		ModuleModel();
		~ModuleModel();

		unsigned	Load(CustomFile& path);
		void		DrawModels();
		void		ApplyTexture(Texture& texture);
		void		DeleteModels();
		void		DrawGUI();

	public:
		std::list<Model>	models;
		bool				checkersTexture = false;
		Texture				checkTexture = Texture(0, 0, 0);

};

#endif //__MODULEMODEL_H__
