#pragma once
#include<list>
#include "Module.h"
#include "Globals.h"
#include "GL/glew.h"
#include "IL/il.h"
#include "IL/ilut.h"
#include "IL/ilu.h"
#include "SDL.h"
#include "SDL_image/include/SDL_image.h"

struct SDL_Texture;

class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool			Init();
	bool			CleanUp();
	unsigned	 	Load(const char* path);
	void			ReloadTexture(const char* newPath, unsigned texture);
	void			Unload(unsigned texture);
	void			SetNewParameter(const char* newPath, unsigned texture, unsigned newTextFilter, unsigned newResizeMethod, unsigned newWrapMethod, unsigned newClampMethod);
	void			SwitchMipMaps(const char* newPath, unsigned texture, bool state);

public:

	char* imgFormat;
	unsigned textFilter = 0;
	unsigned resizeMethod = 0;
	unsigned wrapMethod = 0;
	unsigned clampMethod = 0;
	bool generateMipMaps = false;

};		   