#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include "Module.h"
#include "Globals.h"
#include "Model.h"
#include "imgui.h"
#include "GL/glew.h"
#include "IL/ilut.h"

class ModuleTextures : public Module
{
	public:
		ModuleTextures();
		~ModuleTextures();

		bool			Init() override;
		Texture* const  Load(const char* path);
		void			DrawGUI();
		void			CreateComponentTexture();

	public:
		bool mipmaping = false;
		int filterType = GL_LINEAR;
		int mipMapMode = GL_NEAREST_MIPMAP_NEAREST;
		int wrapMode = GL_CLAMP;
};		   

#endif //__MODULETEXTURES_H__