#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include "GL/glew.h"
#include "IL/ilut.h"

struct Texture {
	int id = 0;
	int width = 0;
	int height = 0;
	Texture(int id, int width, int height) : id(id), width(width), height(height) { }
};

class ModuleTextures : public Module
{
	public:
		ModuleTextures();
		~ModuleTextures();

		bool			Init() override;
		Texture* const  Load(const char* path);
		void			LoadDefaulTextures();
		void			DrawGUI();

	public:
		bool mipmaping = false;
		int filterType = GL_LINEAR;
		int mipMapMode = GL_NEAREST_MIPMAP_NEAREST;
		int wrapMode = GL_CLAMP;
		Texture*	defaultTexture = nullptr;
		Texture*	noCameraSelectedTexture = nullptr;
};		   

#endif //__MODULETEXTURES_H__